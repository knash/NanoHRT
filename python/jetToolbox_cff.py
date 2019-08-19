###############################################
####
####   Jet Substructure Toolbox (jetToolBox)
####   Python function for easy access of 
####   jet substructure tools implemented in CMS
####
####   Alejandro Gomez Espinosa (alejandro.gomez@cern.ch)
####   with several contributions from others
####
###############################################
import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoPFJets_cff import ak4PFJets, ak8PFJetsCHSSoftDrop, ak8PFJetsCHSSoftDropMass, ak8PFJetsCHSPruned, ak8PFJetsCHSPrunedMass, ak8PFJetsCHSTrimmed, ak8PFJetsCHSTrimmedMass, ak8PFJetsCHSFiltered, ak8PFJetsCHSFilteredMass, ak4PFJetsCHS, ca15PFJetsCHSMassDropFiltered, ak8PFJetsCHSConstituents, puppi
from RecoJets.JetProducers.caTopTaggers_cff import hepTopTagPFJetsCHS
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.CATopJetParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection, updateJetCollection
from collections import OrderedDict

def jetToolbox( proc, jetType, jetSequence, outputFile,
		updateCollection='', updateCollectionSubjets='',
		newPFCollection=False, nameNewPFCollection = '',	
		PUMethod='CHS', 
		miniAOD=True,
		runOnMC=True,
		JETCorrPayload='', JETCorrLevels = [ 'None' ], GetJetMCFlavour=True,
		Cut = '', 
		postFix='',
		# blank means default list of discriminators, None means none
		bTagDiscriminators = '',
		bTagInfos = None, 
		rerunivf = False, 
		subjetBTagDiscriminators = '',
		subjetBTagInfos = None, 
		subJETCorrPayload='', subJETCorrLevels = [ 'None' ], GetSubjetMCFlavour=True,
		CutSubjet = '', 
		addPruning=False, zCut=0.1, rCut=0.5, addPrunedSubjets=False,
		addSoftDrop=False, betaCut=0.0,  zCutSD=0.1, addSoftDropSubjets=False,
		addTrimming=False, rFiltTrim=0.2, ptFrac=0.03,
		addFiltering=False, rfilt=0.3, nfilt=3,
		svLabel = cms.InputTag("slimmedSecondaryVertices"),
		muLabel = cms.InputTag("slimmedMuons"),
		elLabel = cms.InputTag("slimmedElectrons"),
		addCMSTopTagger=False,
		addMassDrop=False,
		addHEPTopTagger=False,
		addNsub=False, maxTau=4,
		addNsubSubjets=False, subjetMaxTau=4,
		addPUJetID=False,
		addQGTagger=False, QGjetsLabel='chs',
		addEnergyCorrFunc=False, 
		addEnergyCorrFuncSubjets=False,
		# set this to false to disable creation of jettoolbox.root
		# then you need to associate the jetTask to a Path or EndPath manually in your config
		associateTask=True,
		# 0 = no printouts, 1 = warnings only, 2 = warnings & info, 3 = warnings, info, debug
		verbosity=2,
		):

	runOnData = not runOnMC
	if runOnData:
		GetJetMCFlavour = False
		GetSubjetMCFlavour = False
	
	###############################################################################
	#######  Verifying some inputs and defining variables
	###############################################################################
	if verbosity>=1: print('|---- jetToolbox: Initializing collection... (with postfix '+postFix+')')
	if newPFCollection and verbosity>=2: print('|---- jetToolBox: Using '+ nameNewPFCollection +' as PFCandidates collection')
	supportedJetAlgos = { 'ak': 'AntiKt', 'ca' : 'CambridgeAachen', 'kt' : 'Kt' }
	recommendedJetAlgos = [ 'ak4', 'ak8', 'ca4', 'ca8', 'ca10', 'ca15' ]
	payloadList = [ 'None',
			'AK1PFchs', 'AK2PFchs', 'AK3PFchs', 'AK4PFchs', 'AK5PFchs', 'AK6PFchs', 'AK7PFchs', 'AK8PFchs', 'AK9PFchs', 'AK10PFchs',
			'AK1PFPuppi', 'AK2PFPuppi', 'AK3PFPuppi', 'AK4PFPuppi', 'AK5PFPuppi', 'AK6PFPuppi', 'AK7PFPuppi', 'AK8PFPuppi', 'AK9PFPuppi', 'AK10PFPuppi',  
			'AK1PFSK', 'AK2PFSK', 'AK3PFSK', 'AK4PFSK', 'AK5PFSK', 'AK6PFSK', 'AK7PFSK', 'AK8PFSK', 'AK9PFSK', 'AK10PFSK',  
			'AK1PF', 'AK2PF', 'AK3PF', 'AK4PF', 'AK5PF', 'AK6PF', 'AK7PF', 'AK8PF', 'AK9PF', 'AK10PF' ]
	JECLevels = [ 'L1Offset', 'L1FastJet', 'L1JPTOffset', 'L2Relative', 'L3Absolute', 'L5Falvour', 'L7Parton' ]
	if runOnData:
		JECLevels += ['L2L3Residual']
	jetAlgo = ''
	algorithm = ''
	size = ''
	for TYPE, tmpAlgo in supportedJetAlgos.iteritems(): 
		if TYPE in jetType.lower():
			jetAlgo = TYPE
			algorithm = tmpAlgo
			size = jetType.replace( TYPE, '' )

	jetSize = 0.
	if int(size) in range(0, 20): jetSize = int(size)/10.
	else: raise ValueError('|---- jetToolBox: jetSize has not a valid value. Insert a number between 1 and 20 after algorithm, like: AK8')
	### Trick for uppercase/lowercase algo name
	jetALGO = jetAlgo.upper()+size
	jetalgo = jetAlgo.lower()+size
	if jetalgo not in recommendedJetAlgos and verbosity>=1: print('|---- jetToolBox: CMS recommends the following jet algorithms: '+' '.join(recommendedJetAlgos)+'. You are using '+jetalgo+' .')


	#################################################################################
	####### Toolbox start 
	#################################################################################

	elemToKeep = []
	jetSeq = cms.Sequence()
	genParticlesLabel = ''
	pvLabel = ''
	tvLabel = ''
	toolsUsed = []
	mod = OrderedDict()

	### List of Jet Corrections
	if not set(JETCorrLevels).issubset(set(JECLevels)): 
		if ( 'CHS' in PUMethod ) or  ( 'Plain' in PUMethod ): JETCorrLevels = ['L1FastJet','L2Relative', 'L3Absolute']
		else: JETCorrLevels = [ 'L2Relative', 'L3Absolute']
		if runOnData: JETCorrLevels.append('L2L3Residual')
	if not set(subJETCorrLevels).issubset(set(JECLevels)): 
		if ( 'CHS' in PUMethod ) or  ( 'Plain' in PUMethod ): subJETCorrLevels = ['L1FastJet','L2Relative', 'L3Absolute']
		else: subJETCorrLevels = [ 'L2Relative', 'L3Absolute']
		if runOnData: subJETCorrLevels.append('L2L3Residual')


	## b-tag discriminators
	defaultBTagDiscriminators = [
			'pfTrackCountingHighEffBJetTags',
			'pfTrackCountingHighPurBJetTags',
			'pfJetProbabilityBJetTags',
			'pfJetBProbabilityBJetTags',
			'pfSimpleSecondaryVertexHighEffBJetTags',
			'pfSimpleSecondaryVertexHighPurBJetTags',
			'pfCombinedSecondaryVertexV2BJetTags',
			'pfCombinedInclusiveSecondaryVertexV2BJetTags',
			'pfCombinedMVAV2BJetTags'
	]
	if bTagDiscriminators is '': bTagDiscriminators = defaultBTagDiscriminators
	if subjetBTagDiscriminators is '': subjetBTagDiscriminators = defaultBTagDiscriminators

	if updateCollection and 'Puppi' in updateCollection: PUMethod='Puppi'
	mod["PATJetsLabel"] = jetALGO+'PF'+PUMethod
	mod["PATJetsLabelPost"] = mod["PATJetsLabel"]+postFix
	# some substructure quantities don't include the 'PF' in the name
	mod["SubstructureLabel"] = jetALGO+PUMethod+postFix
	if not updateCollection: 

		mod["GenJetsNoNu"] = jetalgo+'GenJetsNoNu'
		#### For MiniAOD
		if miniAOD:

			if verbosity>=2: print('|---- jetToolBox: JETTOOLBOX RUNNING ON MiniAOD FOR '+jetALGO+' JETS USING '+PUMethod)

			genParticlesLabel = 'prunedGenParticles'
			pvLabel = 'offlineSlimmedPrimaryVertices'
			
			tvLabel = 'unpackedTracksAndVertices'
			
			pfCand = nameNewPFCollection if newPFCollection else 'packedPFCandidates'

			if runOnMC:
				## Filter out neutrinos from packed GenParticles
				mod["GenParticlesNoNu"] = 'packedGenParticlesForJetsNoNu'
				_addProcessAndTask(proc, mod["GenParticlesNoNu"],
						cms.EDFilter("CandPtrSelector", 
							src = cms.InputTag("packedGenParticles"), 
							cut = cms.string("abs(pdgId) != 12 && abs(pdgId) != 14 && abs(pdgId) != 16")
							))
				jetSeq += getattr(proc, mod["GenParticlesNoNu"])

				_addProcessAndTask(proc, mod["GenJetsNoNu"],
						ak4GenJets.clone( src = mod["GenParticlesNoNu"],
							rParam = jetSize, 
							jetAlgorithm = algorithm ) ) 
				jetSeq += getattr(proc, mod["GenJetsNoNu"])

			#for Inclusive Vertex Finder
			proc.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')

		#### For AOD
		else:
			if verbosity>=2: print('|---- jetToolBox: JETTOOLBOX RUNNING ON AOD FOR '+jetALGO+' JETS USING '+PUMethod)

			genParticlesLabel = 'genParticles'
			pvLabel = 'offlinePrimaryVertices'
			tvLabel = 'generalTracks'
			elLabel = 'gedGsfElectrons'
			pfCand =  nameNewPFCollection if newPFCollection else 'particleFlow'
			

			if runOnMC:
				proc.load('RecoJets.Configuration.GenJetParticles_cff')
				_addProcessAndTask(proc, mod["GenJetsNoNu"], ak4GenJets.clone(src='genParticlesForJetsNoNu', rParam=jetSize, jetAlgorithm=algorithm))
				jetSeq += getattr(proc, mod["GenJetsNoNu"])
			

		####  Creating PATjets
		tmpPfCandName = pfCand.lower()
		mod["PFJets"] = ""
		if 'Puppi' in PUMethod:
			if ('puppi' in tmpPfCandName): 
				srcForPFJets = pfCand
				if verbosity>=1: print('|---- jetToolBox: Not running puppi algorithm because keyword puppi was specified in nameNewPFCollection, but applying puppi corrections.')
			else: 
				proc.load('CommonTools.PileupAlgos.Puppi_cff')
				puppi.candName = cms.InputTag( pfCand ) 
				if miniAOD:
				  puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')
				  puppi.clonePackedCands = cms.bool(True)
				  puppi.useExistingWeights = cms.bool(True)
				_addProcessAndTask(proc, 'puppi', getattr(proc, 'puppi'))
				jetSeq += getattr(proc, 'puppi' )
				srcForPFJets = 'puppi'

			from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJetsPuppi
			mod["PFJets"] = jetalgo+'PFJetsPuppi'+postFix
			_addProcessAndTask(proc, mod["PFJets"],
					ak4PFJetsPuppi.clone( src = cms.InputTag( srcForPFJets ),
						doAreaFastjet = True, 
						rParam = jetSize, 
						jetAlgorithm = algorithm ) )  
			jetSeq += getattr(proc, mod["PFJets"])

			if JETCorrPayload not in payloadList: JETCorrPayload = 'AK'+size+'PFPuppi'
			if subJETCorrPayload not in payloadList: subJETCorrPayload = 'AK4PFPuppi'

		elif 'SK' in PUMethod:

			if ('sk' in tmpPfCandName): 
				srcForPFJets = pfCand
				if verbosity>=1: print('|---- jetToolBox: Not running softkiller algorithm because keyword SK was specified in nameNewPFCollection, but applying SK corrections.')
			else:
				proc.load('CommonTools.PileupAlgos.softKiller_cfi')
				getattr( proc, 'softKiller' ).PFCandidates = cms.InputTag( pfCand )
				_addProcessAndTask(proc, 'softKiller', getattr(proc, 'softKiller'))
				jetSeq += getattr(proc, 'softKiller' )
				srcForPFJets = 'softKiller'

			from RecoJets.JetProducers.ak4PFJetsSK_cfi import ak4PFJetsSK
			mod["PFJets"] = jetalgo+'PFJetsSK'+postFix
			_addProcessAndTask(proc, mod["PFJets"],
					ak4PFJetsSK.clone(  src = cms.InputTag( srcForPFJets ),
						rParam = jetSize, 
						jetAlgorithm = algorithm ) ) 
			jetSeq += getattr(proc, mod["PFJets"])

			if JETCorrPayload not in payloadList: JETCorrPayload = 'AK'+size+'PFSK'
			if subJETCorrPayload not in payloadList: subJETCorrPayload = 'AK4PFSK'
		
		elif 'CS' in PUMethod: 

			from RecoJets.JetProducers.ak4PFJetsCHSCS_cfi import ak4PFJetsCHSCS
			mod["PFJets"] = jetalgo+'PFJetsCS'+postFix
			_addProcessAndTask(proc, mod["PFJets"],
					ak4PFJetsCHSCS.clone( doAreaFastjet = True, 
						src = cms.InputTag( pfCand ), #srcForPFJets ),
						csRParam = cms.double(jetSize),
						jetAlgorithm = algorithm ) ) 
			if miniAOD: getattr( proc, mod["PFJets"]).src = pfCand
			jetSeq += getattr(proc, mod["PFJets"])

			if JETCorrPayload not in payloadList: JETCorrPayload = 'AK'+size+'PFCS'
			if subJETCorrPayload not in payloadList: subJETCorrPayload = 'AK4PFCS'

		elif 'CHS' in PUMethod: 
			
			if miniAOD:
				if ('chs' in tmpPfCandName): 
					srcForPFJets = pfCand
					if verbosity>=1: print('|---- jetToolBox: Not running CHS algorithm because keyword CHS was specified in nameNewPFCollection, but applying CHS corrections.')
				else: 
					_addProcessAndTask(proc, 'chs', cms.EDFilter('CandPtrSelector', src=cms.InputTag(pfCand), cut=cms.string('fromPV')))
					jetSeq += getattr(proc, 'chs')
					srcForPFJets = 'chs'
			else:
				if ( pfCand == 'particleFlow' ):
					from RecoParticleFlow.PFProducer.particleFlowTmpPtrs_cfi import particleFlowTmpPtrs
					_addProcessAndTask(proc, 'newParticleFlowTmpPtrs', particleFlowTmpPtrs.clone(src=pfCand))
					jetSeq += getattr(proc, 'newParticleFlowTmpPtrs')
					from CommonTools.ParticleFlow.pfNoPileUpJME_cff import pfPileUpJME, pfNoPileUpJME
					proc.load('CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi')
					_addProcessAndTask(proc, 'newPfPileUpJME', pfPileUpJME.clone(PFCandidates='newParticleFlowTmpPtrs'))
					jetSeq += getattr(proc, 'newPfPileUpJME')
					_addProcessAndTask(proc, 'newPfNoPileUpJME', pfNoPileUpJME.clone(topCollection='newPfPileUpJME', bottomCollection='newParticleFlowTmpPtrs'))
					jetSeq += getattr(proc, 'newPfNoPileUpJME')
					srcForPFJets = 'newPfNoPileUpJME'
				else: 
					proc.load('CommonTools.ParticleFlow.pfNoPileUpJME_cff')
					srcForPFJets = 'pfNoPileUpJME'
				
			mod["PFJets"] = jetalgo+'PFJetsCHS'+postFix
			_addProcessAndTask(proc, mod["PFJets"],
					ak4PFJetsCHS.clone( src = cms.InputTag( srcForPFJets ), 
						doAreaFastjet = True, 
						rParam = jetSize, 
						jetAlgorithm = algorithm ) ) 
			jetSeq += getattr(proc, mod["PFJets"])

			if JETCorrPayload not in payloadList: JETCorrPayload = 'AK'+size+'PFchs'
			if subJETCorrPayload not in payloadList: subJETCorrPayload = 'AK4PFchs'

		else: 
			PUMethod = ''
			mod["PFJets"] = jetalgo+'PFJets'+postFix
			_addProcessAndTask(proc, mod["PFJets"],
					ak4PFJets.clone( 
						doAreaFastjet = True, 
						rParam = jetSize, 
						jetAlgorithm = algorithm ) ) 
			if miniAOD: getattr( proc, mod["PFJets"]).src = pfCand
			jetSeq += getattr(proc, mod["PFJets"])
			if JETCorrPayload not in payloadList: JETCorrPayload = 'AK'+size+'PF'
			if subJETCorrPayload not in payloadList: subJETCorrPayload = 'AK4PF'


		if 'None' in JETCorrPayload: JEC = None
		else: JEC = ( JETCorrPayload.replace('CS','chs').replace('SK','chs') , JETCorrLevels, 'None' )   ### temporary
		#else: JEC = ( JETCorrPayload., JETCorrLevels, 'None' ) 
		if verbosity>=2: print('|---- jetToolBox: Applying these corrections: '+str(JEC))

		if addPrunedSubjets or addSoftDropSubjets or addCMSTopTagger:
			if 'None' in subJETCorrPayload: subJEC = None
			else: subJEC = ( subJETCorrPayload.replace('CS','chs').replace('SK','chs') , subJETCorrLevels, 'None' )   ### temporary


		mod["PFJetsConstituents"] = mod["PFJets"]+'Constituents'
		mod["PFJetsConstituentsColon"] = mod["PFJets"]+'Constituents:constituents'
		mod["PFJetsConstituentsColonOrUpdate"] = mod["PFJetsConstituentsColon"] if not updateCollection else updateCollection
		if (PUMethod in [ 'CHS', 'CS', 'Puppi' ]) and miniAOD: _addProcessAndTask(proc, mod["PFJetsConstituents"], cms.EDProducer("MiniAODJetConstituentSelector", src=cms.InputTag(mod["PFJets"]), cut=cms.string(Cut)))
		else: _addProcessAndTask(proc, mod["PFJetsConstituents"], cms.EDProducer("PFJetConstituentSelector", src=cms.InputTag(mod["PFJets"]), cut=cms.string(Cut)))
		jetSeq += getattr(proc, mod["PFJetsConstituents"])

		addJetCollection(
				proc,
				labelName = mod["PATJetsLabel"],
				jetSource = cms.InputTag(mod["PFJets"]),
				postfix = postFix, 
				algo = jetalgo,
				rParam = jetSize,
				runIVF = rerunivf,
				jetCorrections = JEC if JEC is not None else None, 
				pfCandidates = cms.InputTag( pfCand ),  
				svSource = svLabel ,  
				genJetCollection = cms.InputTag(mod["GenJetsNoNu"]),
				pvSource = cms.InputTag( pvLabel ), 
				muSource = muLabel ,
				elSource = elLabel ,
				btagDiscriminators = bTagDiscriminators,
				btagInfos = bTagInfos,
				getJetMCFlavour = GetJetMCFlavour,
				genParticles = cms.InputTag(genParticlesLabel),
				outputModules = ['outputFile']
				)
		patJets = 'patJets'
		patSubJets = ''
		selPatJets = 'selectedPatJets'
		mod["PATJets"] = patJets+mod["PATJetsLabelPost"]
		mod["selPATJets"] = selPatJets+mod["PATJetsLabelPost"]
		getattr(proc, mod["PATJets"]).addTagInfos = cms.bool(True)

		if 'CS' in PUMethod: getattr( proc, mod["PATJets"] ).getJetMCFlavour = False  # CS jets cannot be re-clustered from their constituents
	else:
		if verbosity>=2: print('|---- jetToolBox: JETTOOLBOX IS UPDATING '+updateCollection+' collection')
		genParticlesLabel = 'prunedGenParticles'
		pvLabel = 'offlineSlimmedPrimaryVertices'
		
		tvLabel = 'unpackedTracksAndVertices'

		if not JETCorrPayload: 
			raise ValueError('|---- jetToolBox: updateCollection option requires to add JETCorrPayload.')

		JEC = ( JETCorrPayload, JETCorrLevels, 'None' )   ### temporary
		if verbosity>=2: print('|---- jetToolBox: Applying these corrections: '+str(JEC))
		updateJetCollection(
				proc,
				postfix = postFix,
				jetSource = cms.InputTag( updateCollection ),
				labelName = mod["PATJetsLabel"],
				jetCorrections = JEC, 
				svSource = svLabel ,
				muSource = muLabel ,
				elSource = elLabel ,
				runIVF = rerunivf,   
				btagDiscriminators = bTagDiscriminators,
				btagInfos = bTagInfos,
				)
		mod["PATJetsCorrFactors"] = 'patJetCorrFactors'+mod["PATJetsLabelPost"]
		getattr( proc, mod["PATJetsCorrFactors"] ).payload = JETCorrPayload
		getattr( proc, mod["PATJetsCorrFactors"] ).levels = JETCorrLevels
		patJets = 'updatedPatJets'
		patSubJets = ''
		selPatJets = 'selectedPatJets'
		mod["PATJets"] = patJets+mod["PATJetsLabelPost"]
		mod["selPATJets"] = selPatJets+mod["PATJetsLabelPost"]

		if updateCollectionSubjets:
			if verbosity>=2: print('|---- jetToolBox: JETTOOLBOX IS UPDATING '+updateCollectionSubjets+' collection for subjets/groomers.')
			if 'SoftDrop' in updateCollectionSubjets: updateSubjetLabel = 'SoftDrop'
			else: updateSubjetLabel = 'Pruned'
			mod["PATSubjetsLabel"] = jetALGO+'PF'+PUMethod+postFix+updateSubjetLabel+'Packed'
			updateJetCollection(
					proc,
					jetSource = cms.InputTag( updateCollectionSubjets ),
					labelName = mod["PATSubjetsLabel"],
					jetCorrections = JEC, 
					muSource = muLabel ,
					elSource = elLabel ,
					explicitJTA = True,
					fatJets = cms.InputTag( updateCollection ),
					svSource = svLabel , 
					runIVF = rerunivf, 
					rParam = jetSize, 
					algo = jetALGO,
					btagDiscriminators = subjetBTagDiscriminators,
					btagInfos = subjetBTagInfos,
					)
			mod["PATSubjetsCorrFactors"] = 'patJetCorrFactors'+mod["PATSubjetsLabel"]
			getattr( proc, mod["PATSubjetsCorrFactors"] ).payload = subJETCorrPayload
			getattr( proc, mod["PATSubjetsCorrFactors"] ).levels = subJETCorrLevels

			patSubJets = 'updatedPatJets'+mod["PATSubjetsLabel"]

		if addPrunedSubjets or addSoftDropSubjets or addCMSTopTagger or addMassDrop or addHEPTopTagger or addPruning or addSoftDrop: 
			if verbosity>=1: print('|---- jetToolBox: You are trying to add a groomer variable into a clustered jet collection. THIS IS NOT RECOMMENDED, it is recommended to recluster jets instead using a plain jetToolbox configuration. Please use this feature at your own risk.')

	mod["PFJetsOrUpdate"] = mod["PFJets"] if not updateCollection else updateCollection

	if bTagDiscriminators and verbosity>=2: print('|---- jetToolBox: Adding these btag discriminators: '+str(bTagDiscriminators)+' in the jet collection.')
	if ( (addPrunedSubjets or addSoftDropSubjets) or (updateCollection and updateCollectionSubjets) ) and subjetBTagDiscriminators and verbosity>=2:
		print('|---- jetToolBox: Adding these btag discriminators: '+str(subjetBTagDiscriminators)+' in the subjet collection.')

	#### Groomers
	if addSoftDrop or addSoftDropSubjets: 

		mod["PFJetsSoftDrop"] = mod["PFJets"]+'SoftDrop'
		mod["SoftDropMass"] = mod["PFJets"]+'SoftDropMass'
		_addProcessAndTask(proc, mod["PFJetsSoftDrop"],
			ak8PFJetsCHSSoftDrop.clone( 
				src = cms.InputTag( mod["PFJetsConstituentsColonOrUpdate"] ),
				rParam = jetSize, 
				jetAlgorithm = algorithm, 
				useExplicitGhosts=True,
				R0= cms.double(jetSize),
				zcut=zCutSD, 
				beta=betaCut,
				doAreaFastjet = cms.bool(True),
				writeCompound = cms.bool(True),
				jetCollInstanceName=cms.string('SubJets') ) )
		_addProcessAndTask(proc, mod["SoftDropMass"],
			ak8PFJetsCHSSoftDropMass.clone( src = cms.InputTag( mod["PFJetsOrUpdate"] ), 
				matched = cms.InputTag( mod["PFJetsSoftDrop"] ),
				distMax = cms.double( jetSize ) ) )

		elemToKeep += [ 'keep *_'+mod["SoftDropMass"]+'_*_*'] 
		jetSeq += getattr(proc, mod["PFJetsSoftDrop"] )
		jetSeq += getattr(proc, mod["SoftDropMass"] )
		getattr( proc, mod["PATJets"]).userData.userFloats.src += [ mod["SoftDropMass"] ]
		toolsUsed.append( mod["SoftDropMass"] )

		if addSoftDropSubjets:

			mod["GenJetsNoNuSoftDrop"] = mod["GenJetsNoNu"]+'SoftDrop'
			if runOnMC:
				_addProcessAndTask(proc, mod["GenJetsNoNuSoftDrop"],
						ak4GenJets.clone(
							SubJetParameters,
							useSoftDrop = cms.bool(True),
							rParam = jetSize, 
							jetAlgorithm = algorithm, 
							useExplicitGhosts=cms.bool(True),
							#zcut=cms.double(zCutSD), 
							R0= cms.double(jetSize),
							beta=cms.double(betaCut),
							writeCompound = cms.bool(True),
							jetCollInstanceName=cms.string('SubJets')
							))
				if miniAOD: getattr( proc, mod["GenJetsNoNuSoftDrop"] ).src = mod["GenParticlesNoNu"]
				jetSeq += getattr(proc, mod["GenJetsNoNuSoftDrop"] )

			mod["PATJetsSoftDropLabel"] = mod["PATJetsLabelPost"]+'SoftDrop'
			addJetCollection(
					proc,
					labelName = mod["PATJetsSoftDropLabel"],
					jetSource = cms.InputTag( mod["PFJetsSoftDrop"]),
					algo = jetalgo,
					rParam = jetSize,
					jetCorrections = JEC if JEC is not None else None, 
					pvSource = cms.InputTag( pvLabel ),
					svSource = svLabel ,  
					runIVF = rerunivf, 
					btagDiscriminators = ['None'],
					genJetCollection = cms.InputTag( mod["GenJetsNoNu"]),
					getJetMCFlavour = False, # jet flavor should always be disabled for groomed jets
					genParticles = cms.InputTag(genParticlesLabel),
					outputModules = ['outputFile']
					)
			mod["PATJetsSoftDrop"] = patJets+mod["PATJetsSoftDropLabel"]
			mod["selPATJetsSoftDrop"] = selPatJets+mod["PATJetsSoftDropLabel"]

			_addProcessAndTask(proc, mod["selPATJetsSoftDrop"], selectedPatJets.clone(src=mod["PATJetsSoftDrop"], cut=Cut))

			mod["PATSubjetsSoftDropLabel"] = mod["PATJetsSoftDropLabel"]+'Subjets'
			addJetCollection(
					proc,
					labelName = mod["PATSubjetsSoftDropLabel"],
					jetSource = cms.InputTag( mod["PFJetsSoftDrop"], 'SubJets'),
					algo = jetalgo,  # needed for subjet b tagging
					rParam = jetSize,  # needed for subjet b tagging
					jetCorrections = subJEC if subJEC is not None else None, 
					pfCandidates = cms.InputTag( pfCand ), 
					pvSource = cms.InputTag( pvLabel), 
					svSource = svLabel ,  
					runIVF = rerunivf, 
					muSource =muLabel ,
					elSource = elLabel ,
					btagDiscriminators = subjetBTagDiscriminators,
					btagInfos = subjetBTagInfos,
					genJetCollection = cms.InputTag( mod["GenJetsNoNuSoftDrop"],'SubJets'),
					getJetMCFlavour = GetSubjetMCFlavour,
					genParticles = cms.InputTag(genParticlesLabel),
					explicitJTA = True,  # needed for subjet b tagging
					svClustering = True, # needed for subjet b tagging
					fatJets=cms.InputTag(mod["PFJets"]),             # needed for subjet flavor clustering
					groomedFatJets=cms.InputTag(mod["PFJetsSoftDrop"]), # needed for subjet flavor clustering
					outputModules = ['outputFile']
					) 
			mod["PATSubjetsSoftDrop"] = patJets+mod["PATSubjetsSoftDropLabel"]
			mod["selPATSubjetsSoftDrop"] = selPatJets+mod["PATSubjetsSoftDropLabel"]

			_addProcessAndTask(proc, mod["selPATSubjetsSoftDrop"], selectedPatJets.clone(src=mod["PATSubjetsSoftDrop"], cut=CutSubjet))

			# Establish references between PATified fat jets and subjets using the BoostedJetMerger
			mod["selPATJetsSoftDropPacked"] = mod["selPATJetsSoftDrop"]+'Packed'
			_addProcessAndTask(proc, mod["selPATJetsSoftDropPacked"],
					cms.EDProducer("BoostedJetMerger",
						jetSrc=cms.InputTag(mod["selPATJetsSoftDrop"]),
						subjetSrc=cms.InputTag(mod["selPATSubjetsSoftDrop"])
						))
			jetSeq += getattr(proc, mod["selPATJetsSoftDropPacked"] )
			getattr(proc, mod["PATSubjetsSoftDrop"]).addTagInfos = cms.bool(True)
			elemToKeep += [ 'keep *_'+mod["selPATJetsSoftDropPacked"]+'_SubJets_*' ]
			toolsUsed.append( mod["selPATJetsSoftDropPacked"]+':SubJets' )

			## Pack fat jets with subjets
			mod["packedPATJetsSoftDrop"] = 'packedPatJets'+mod["PATJetsSoftDropLabel"]
			_addProcessAndTask(proc, mod["packedPATJetsSoftDrop"],
				 cms.EDProducer("JetSubstructurePacker",
						jetSrc=cms.InputTag(mod["selPATJets"]),
						distMax = cms.double( jetSize ),
						fixDaughters = cms.bool(False),
						algoTags = cms.VInputTag(
						cms.InputTag(mod["selPATJetsSoftDropPacked"])
						), 
						algoLabels =cms.vstring('SoftDrop')
						)
				 )
			jetSeq += getattr(proc, mod["packedPATJetsSoftDrop"])
			elemToKeep += [ 'keep *_'+mod["packedPATJetsSoftDrop"]+'_*_*' ]
			if verbosity>=2: print('|---- jetToolBox: Creating '+mod["packedPATJetsSoftDrop"]+' collection with SoftDrop subjets.')



	if addPruning or addPrunedSubjets: 

		mod["PFJetsPruned"] = mod["PFJets"]+'Pruned'
		mod["PrunedMass"] =  mod["PFJets"]+'PrunedMass'
		_addProcessAndTask(proc, mod["PFJetsPruned"],
			ak8PFJetsCHSPruned.clone( 
				src = cms.InputTag( mod["PFJetsConstituentsColonOrUpdate"] ),
				rParam = jetSize, 
				jetAlgorithm = algorithm, 
				zcut=zCut, 
				rcut_factor=rCut,
				writeCompound = cms.bool(True),
				doAreaFastjet = cms.bool(True),
				jetCollInstanceName=cms.string('SubJets') ) )
		_addProcessAndTask(proc, mod["PrunedMass"],
			ak8PFJetsCHSPrunedMass.clone( 
				src = cms.InputTag( mod["PFJetsOrUpdate"] ), 
				matched = cms.InputTag( mod["PFJetsPruned"]), 
				distMax = cms.double( jetSize ) ) )

		jetSeq += getattr(proc, mod["PFJetsPruned"])
		jetSeq += getattr(proc, mod["PrunedMass"])
		getattr( proc, mod["PATJets"]).userData.userFloats.src += [ mod["PrunedMass"] ]
		elemToKeep += [ 'keep *_'+mod["PrunedMass"]+'_*_*'] 
		toolsUsed.append( mod["PrunedMass"] )

		if addPrunedSubjets:
			if runOnMC:
				mod["GenJetsNoNuPruned"] = mod["GenJetsNoNu"]+'Pruned'
				_addProcessAndTask(proc, mod["GenJetsNoNuPruned"],
						ak4GenJets.clone(
							SubJetParameters,
							rParam = jetSize,
							usePruning = cms.bool(True),
							writeCompound = cms.bool(True),
							jetCollInstanceName=cms.string('SubJets')
							))
				if miniAOD: getattr( proc, mod["GenJetsNoNuPruned"] ).src = mod["GenParticlesNoNu"]
				jetSeq += getattr(proc, mod["GenJetsNoNuPruned"])

			mod["PATJetsPrunedLabel"] = mod["PATJetsLabelPost"]+'Pruned'
			addJetCollection(
					proc,
					labelName = mod["PATJetsPrunedLabel"],
					jetSource = cms.InputTag( mod["PFJetsPruned"]),
					algo = jetalgo,
					rParam = jetSize,
					jetCorrections = JEC if JEC is not None else None, 
					pvSource = cms.InputTag( pvLabel ),
					svSource = svLabel , 
					runIVF = rerunivf, 
					btagDiscriminators = ['None'],
					genJetCollection = cms.InputTag( mod["GenJetsNoNu"]),
					getJetMCFlavour = False, # jet flavor should always be disabled for groomed jets
					genParticles = cms.InputTag(genParticlesLabel),
					outputModules = ['outputFile']
					)
			mod["PATJetsPruned"] = patJets+mod["PATJetsPrunedLabel"]
			mod["selPATJetsPruned"] = selPatJets+mod["PATJetsPrunedLabel"]

			_addProcessAndTask(proc, mod["selPATJetsPruned"], selectedPatJets.clone(src=mod["PATJetsPruned"], cut=Cut))

			mod["PATSubjetsPrunedLabel"] = mod["PATJetsPrunedLabel"]+'Subjets'
			addJetCollection(
					proc,
					labelName = mod["PATSubjetsPrunedLabel"],
					jetSource = cms.InputTag( mod["PFJetsPruned"], 'SubJets'),
					algo = jetalgo,  # needed for subjet b tagging
					rParam = jetSize,  # needed for subjet b tagging
					jetCorrections = subJEC if subJEC is not None else None, 
					pfCandidates = cms.InputTag( pfCand ),  
					pvSource = cms.InputTag( pvLabel), 
					svSource = svLabel , 
					runIVF = rerunivf, 
					muSource =muLabel ,
					elSource = elLabel ,
					getJetMCFlavour = GetSubjetMCFlavour,
					genParticles = cms.InputTag(genParticlesLabel),
					btagDiscriminators = subjetBTagDiscriminators,
					btagInfos = subjetBTagInfos,
					genJetCollection = cms.InputTag( mod["GenJetsNoNuPruned"],'SubJets'),
					explicitJTA = True,  # needed for subjet b tagging
					svClustering = True, # needed for subjet b tagging
					fatJets=cms.InputTag(mod["PFJets"]),             # needed for subjet flavor clustering
					groomedFatJets=cms.InputTag(mod["PFJetsPruned"]), # needed for subjet flavor clustering
					outputModules = ['outputFile']
					) 
			mod["PATSubjetsPruned"] = patJets+mod["PATSubjetsPrunedLabel"]
			mod["selPATSubjetsPruned"] = selPatJets+mod["PATSubjetsPrunedLabel"]

			_addProcessAndTask(proc, mod["selPATSubjetsPruned"], selectedPatJets.clone(src=mod["PATSubjetsPruned"], cut=CutSubjet))

			## Establish references between PATified fat jets and subjets using the BoostedJetMerger
			mod["selPATJetsPrunedPacked"] = mod["selPATJetsPruned"]+'Packed'
			_addProcessAndTask(proc, mod["selPATJetsPrunedPacked"],
					cms.EDProducer("BoostedJetMerger",
						jetSrc=cms.InputTag(mod["selPATJetsPruned"]),
						subjetSrc=cms.InputTag(mod["selPATSubjetsPruned"]),
						))
			jetSeq += getattr(proc, mod["selPATJetsPrunedPacked"])
			elemToKeep += [ 'keep *_'+mod["selPATJetsPrunedPacked"]+'_SubJets_*' ]
			toolsUsed.append( mod["selPATJetsPrunedPacked"]+':SubJets' )

			## Pack fat jets with subjets
			mod["packedPATJetsPruned"] = 'packedPatJets'+mod["PATSubjetsPrunedLabel"]
			_addProcessAndTask(proc, mod["packedPATJetsPruned"],
				 cms.EDProducer("JetSubstructurePacker",
						jetSrc=cms.InputTag(mod["selPATJets"]),
						distMax = cms.double( jetSize ),
						fixDaughters = cms.bool(False),
						algoTags = cms.VInputTag(
						cms.InputTag(mod["selPATJetsPrunedPacked"])
						), 
						algoLabels =cms.vstring('Pruned')
						)
				 )
			jetSeq += getattr(proc, mod["packedPATJetsPruned"])
			elemToKeep += [ 'keep *_'+mod["packedPATJetsPruned"]+'_*_*' ]
			if verbosity>=2: print('|---- jetToolBox: Creating '+mod["packedPATJetsPruned"]+' collection with Pruned subjets.')


	if addTrimming:

		mod["PFJetsTrimmed"] = mod["PFJets"]+'Trimmed'
		mod["TrimmedMass"] = mod["PFJets"]+'TrimmedMass'
		_addProcessAndTask(proc, mod["PFJetsTrimmed"],
				ak8PFJetsCHSTrimmed.clone( 
					rParam = jetSize, 
					src = cms.InputTag( mod["PFJetsConstituentsColonOrUpdate"] ),
					jetAlgorithm = algorithm,
					rFilt= rFiltTrim,
					trimPtFracMin= ptFrac) ) 
		_addProcessAndTask(proc, mod["TrimmedMass"],
				ak8PFJetsCHSTrimmedMass.clone( 
					src = cms.InputTag( mod["PFJetsOrUpdate"] ), 
					matched = cms.InputTag( mod["PFJetsTrimmed"]), 
					distMax = cms.double( jetSize ) ) )

		elemToKeep += [ 'keep *_'+mod["TrimmedMass"]+'_*_*'] 
		jetSeq += getattr(proc, mod["PFJetsTrimmed"])
		jetSeq += getattr(proc, mod["TrimmedMass"])
		getattr( proc, mod["PATJets"]).userData.userFloats.src += [mod["TrimmedMass"]]
		toolsUsed.append( mod["TrimmedMass"] )

	if addFiltering:

		mod["PFJetsFiltered"] = mod["PFJets"]+'Filtered'
		mod["FilteredMass"] = mod["PFJets"]+'FilteredMass'
		_addProcessAndTask(proc, mod["PFJetsFiltered"],
				ak8PFJetsCHSFiltered.clone( 
					src = cms.InputTag( mod["PFJetsConstituentsColonOrUpdate"] ),
					rParam = jetSize, 
					jetAlgorithm = algorithm,
					rFilt= rfilt,
					nFilt= nfilt ) ) 
		_addProcessAndTask(proc, mod["FilteredMass"],
				ak8PFJetsCHSFilteredMass.clone( 
					src = cms.InputTag( mod["PFJetsOrUpdate"] ), 
					matched = cms.InputTag( mod["PFJetsFiltered"]), 
					distMax = cms.double( jetSize ) ) )
		elemToKeep += [ 'keep *_'+mod["FilteredMass"]+'_*_*'] 
		jetSeq += getattr(proc, mod["PFJetsFiltered"])
		jetSeq += getattr(proc, mod["FilteredMass"])
		getattr( proc, patJets+jetALGO+'PF'+PUMethod+postFix).userData.userFloats.src += [mod["FilteredMass"]]
		toolsUsed.append( mod["FilteredMass"] )

	if addCMSTopTagger :

		if 'CA' in jetALGO : 

			mod["PFJetsCMSTopTag"] = mod["PFJets"].replace(jetalgo,"cmsTopTag")
			_addProcessAndTask(proc, mod["PFJetsCMSTopTag"],
					cms.EDProducer("CATopJetProducer",
						PFJetParameters.clone( 
							src = cms.InputTag( mod["PFJetsConstituentsColonOrUpdate"] ),
							doAreaFastjet = cms.bool(True),
							doRhoFastjet = cms.bool(False),
							jetPtMin = cms.double(100.0)
							),
						AnomalousCellParameters,
						CATopJetParameters.clone( jetCollInstanceName = cms.string("SubJets"),
							verbose = cms.bool(False),
							algorithm = cms.int32(1), # 0 = KT, 1 = CA, 2 = anti-KT
							tagAlgo = cms.int32(0), #0=legacy top
							useAdjacency = cms.int32(2), # modified adjacency
							centralEtaCut = cms.double(2.5), # eta for defining "central" jets
							sumEtBins = cms.vdouble(0,1600,2600), # sumEt bins over which cuts vary. vector={bin 0 lower bound, bin 1 lower bound, ...}
							rBins = cms.vdouble(0.8,0.8,0.8), # Jet distance paramter R. R values depend on sumEt bins.
							ptFracBins = cms.vdouble(0.05,0.05,0.05), # minimum fraction of central jet pt for subjets (deltap)
							deltarBins = cms.vdouble(0.19,0.19,0.19), # Applicable only if useAdjacency=1. deltar adjacency values for each sumEtBin
							nCellBins = cms.vdouble(1.9,1.9,1.9),
							),
						jetAlgorithm = cms.string("CambridgeAachen"),
						rParam = cms.double(jetSize),
						writeCompound = cms.bool(True)
						)
					)
			
			mod["CATopTagInfos"] = "CATopTagInfos"+postFix
			_addProcessAndTask(proc, mod["CATopTagInfos"],
					cms.EDProducer("CATopJetTagger",
						src = cms.InputTag(mod["PFJetsCMSTopTag"]),
						TopMass = cms.double(171),
						TopMassMin = cms.double(0.),
						TopMassMax = cms.double(250.),
						WMass = cms.double(80.4),
						WMassMin = cms.double(0.0),
						WMassMax = cms.double(200.0),
						MinMassMin = cms.double(0.0),
						MinMassMax = cms.double(200.0),
						verbose = cms.bool(False)
						)
					)
			mod["PATJetsCMSTopTagLabel"] = 'CMSTopTag'+PUMethod+postFix
			addJetCollection(
					proc,
					labelName = mod["PATJetsCMSTopTagLabel"],
					jetSource = cms.InputTag(mod["PFJetsCMSTopTag"]),
					jetCorrections = JEC if JEC is not None else None, 
					pfCandidates = cms.InputTag( pfCand ),  
					pvSource = cms.InputTag( pvLabel), 
					svSource = svLabel , 
					runIVF = rerunivf, 
					muSource = muLabel ,
					elSource = elLabel ,
					btagDiscriminators = bTagDiscriminators,
					btagInfos = bTagInfos,
					genJetCollection = cms.InputTag(mod["GenJetsNoNu"]),
					getJetMCFlavour = False, # jet flavor should always be disabled for groomed jets
					genParticles = cms.InputTag(genParticlesLabel)
					)
			mod["PATJetsCMSTopTag"] = patJets+mod["PATJetsCMSTopTagLabel"]
			mod["selPATJetsCMSTopTag"] = selPatJets+mod["PATJetsCMSTopTagLabel"]
			getattr(proc,mod["PATJetsCMSTopTag"]).addTagInfos = True
			getattr(proc,mod["PATJetsCMSTopTag"]).tagInfoSources = cms.VInputTag( cms.InputTag(mod["CATopTagInfos"]))
			_addProcessAndTask(proc, mod["selPATJetsCMSTopTag"], selectedPatJets.clone(src=mod["PATJetsCMSTopTag"], cut=Cut))

			mod["PATSubjetsCMSTopTagLabel"] = mod["PATJetsCMSTopTagLabel"]+'Subjets'
			addJetCollection(
					proc,
					labelName = mod["PATSubjetsCMSTopTagLabel"],
					jetSource = cms.InputTag(mod["PFJetsCMSTopTag"], 'SubJets'),
					algo = jetalgo,  # needed for subjet b tagging
					rParam = jetSize,  # needed for subjet b tagging
					jetCorrections = subJEC if subJEC is not None else None, 
					pfCandidates = cms.InputTag( pfCand ),  
					pvSource = cms.InputTag( pvLabel), 
					svSource = svLabel , 
					runIVF = rerunivf, 
					muSource = muLabel ,
					elSource =  elLabel ,
					btagDiscriminators = bTagDiscriminators,
					btagInfos = bTagInfos,
					genJetCollection = cms.InputTag( mod["GenJetsNoNu"]),
					getJetMCFlavour = GetSubjetMCFlavour,
					explicitJTA = True,  # needed for subjet b tagging
					svClustering = True, # needed for subjet b tagging
					fatJets=cms.InputTag(mod["PFJets"]),             # needed for subjet flavor clustering
					groomedFatJets=cms.InputTag(mod["PATJetsCMSTopTag"]), # needed for subjet flavor clustering
					genParticles = cms.InputTag(genParticlesLabel)
					)
			mod["PATSubjetsCMSTopTag"] = patJets+mod["PATSubjetsCMSTopTagLabel"]
			mod["selPATSubjetsCMSTopTag"] = selPatJets+mod["PATSubjetsCMSTopTagLabel"]

			_addProcessAndTask(proc, mod["selPATSubjetsCMSTopTag"], selectedPatJets.clone(src=mod["PATSubjetsCMSTopTag"], cut=Cut))

			mod["PATJetsCMSTopTagPacked"] = mod["PATJetsCMSTopTag"]+'Packed'
			_addProcessAndTask(proc, mod["PATJetsCMSTopTagPacked"],
					cms.EDProducer("BoostedJetMerger",
						jetSrc=cms.InputTag(mod["PATJetsCMSTopTag"]),
						subjetSrc=cms.InputTag(mod["PATSubjetsCMSTopTag"])
						))
			jetSeq += getattr(proc, mod["PATJetsCMSTopTagPacked"])
			elemToKeep += [ 'keep *_'+mod["PATJetsCMSTopTagPacked"]+'_*_*' ]
			toolsUsed.append( mod["PATJetsCMSTopTagPacked"] )

		else:
			raise ValueError('|---- jetToolBox: CMS recommends CambridgeAachen for CMS Top Tagger, you are using '+algorithm+'. JetToolbox will not run CMS Top Tagger.')

	if addMassDrop :

		if 'CA' in jetALGO :
			mod["PFJetsMassDrop"] = mod["PFJets"]+'MassDropFiltered'
			mod["MassDropFilteredMass"] = mod["PFJetsMassDrop"]+'Mass'
			_addProcessAndTask(proc, mod["PFJetsMassDrop"],
					ca15PFJetsCHSMassDropFiltered.clone( 
						rParam = jetSize,
						src = cms.InputTag( mod["PFJetsConstituentsColonOrUpdate"] ),
						) )
			_addProcessAndTask(proc, mod["MassDropFilteredMass"], ak8PFJetsCHSPrunedMass.clone(src=cms.InputTag(mod["PFJets"]),
				matched = cms.InputTag(mod["PFJetsMassDrop"]), distMax = cms.double( jetSize ) ) )
			elemToKeep += [ 'keep *_'+mod["MassDropFilteredMass"]+'_*_*' ]
			getattr( proc, mod["PATJets"]).userData.userFloats.src += [ mod["MassDropFilteredMass"] ]
			jetSeq += getattr(proc, mod["PFJetsMassDrop"])
			jetSeq += getattr(proc, mod["MassDropFilteredMass"])
			toolsUsed.append( mod["MassDropFilteredMass"] )
		else:
			raise ValueError('|---- jetToolBox: CMS recommends CambridgeAachen for Mass Drop, you are using '+algorithm+'. JetToolbox will not run Mass Drop.')

	if addHEPTopTagger: 
		if ( jetSize >= 1. ) and ( 'CA' in jetALGO ): 

			mod["PFJetsHEPTopTag"] = mod["PFJets"].replace(jetalgo,"hepTopTag")
			mod["PFJetsHEPTopTagMass"] = mod["PFJetsHEPTopTag"]+'Mass'+jetALGO
			_addProcessAndTask(proc, mod["PFJetsHEPTopTag"], hepTopTagPFJetsCHS.clone(src=cms.InputTag(mod["PFJetsConstituentsColonOrUpdate"])))
			_addProcessAndTask(proc, mod["PFJetsHEPTopTagMass"], ak8PFJetsCHSPrunedMass.clone(src=cms.InputTag(mod["PFJets"]),
				matched = cms.InputTag(mod["PFJetsHEPTopTag"]), distMax = cms.double( jetSize ) ) )
			elemToKeep += [ 'keep *_'+mod["PFJetsHEPTopTagMass"]+'_*_*' ]
			getattr( proc, mod["PATJets"]).userData.userFloats.src += [ mod["PFJetsHEPTopTagMass"] ]
			jetSeq += getattr(proc, mod["PFJetsHEPTopTag"])
			jetSeq += getattr(proc, mod["PFJetsHEPTopTagMass"])
			toolsUsed.append( mod["PFJetsHEPTopTagMass"] )
		else:
			raise ValueError('|---- jetToolBox: CMS recommends CambridgeAachen w/ jet cone size > 1.0 for HEPTopTagger, you are using '+algorithm+'. JetToolbox will not run HEP TopTagger.')

	####### Nsubjettiness
	if addNsub:
		from RecoJets.JetProducers.nJettinessAdder_cfi import Njettiness

		rangeTau = range(1,maxTau+1)
		mod["Njettiness"] = 'Njettiness'+mod["SubstructureLabel"]
		_addProcessAndTask(proc, mod["Njettiness"],
				Njettiness.clone( src = cms.InputTag( mod["PFJetsOrUpdate"] ),
					Njets=cms.vuint32(rangeTau),         # compute 1-, 2-, 3-, 4- subjettiness
					# variables for measure definition : 
					measureDefinition = cms.uint32( 0 ), # CMS default is normalized measure
					beta = cms.double(1.0),              # CMS default is 1
					R0 = cms.double( jetSize ),              # CMS default is jet cone size
					Rcutoff = cms.double( 999.0),       # not used by default
					# variables for axes definition :
					axesDefinition = cms.uint32( 6 ),    # CMS default is 1-pass KT axes
					nPass = cms.int32(999),             # not used by default
					akAxesR0 = cms.double(-999.0) ) )        # not used by default

		elemToKeep += [ 'keep *_'+mod["Njettiness"]+'_*_*' ]
		for tau in rangeTau: getattr( proc, mod["PATJets"]).userData.userFloats.src += [mod["Njettiness"]+':tau'+str(tau) ] 
		jetSeq += getattr(proc, mod["Njettiness"])
		toolsUsed.append( mod["Njettiness"] )

	####### Nsubjettiness
	if addNsubSubjets:

		from RecoJets.JetProducers.nJettinessAdder_cfi import Njettiness

		mod["NsubGroomer"] = ''
		mod["NsubSubjets"] = ''
		mod["NsubPATSubjets"] = ''
		if addSoftDropSubjets or updateCollectionSubjets:
			mod["NsubGroomer"] = mod["PFJetsSoftDrop"]
			mod["NsubSubjets"] = mod["PATSubjetsSoftDropLabel"]
			mod["NsubPATSubjets"] = mod["PATSubjetsSoftDrop"]
			if updateCollectionSubjets:
				if verbosity>=2: print('|---- jetToolBox: Using updateCollection option. ASSUMING MINIAOD collection '+ updateCollectionSubjets +' for Nsubjettiness of subjets.')
		elif addPrunedSubjets:
			mod["NsubGroomer"] = mod["PFJetsPruned"]
			mod["NsubSubjets"] = mod["PATSubjetsPrunedLabel"]
			mod["NsubPATSubjets"] = mod["PATSubjetsPruned"]
		else: 
			raise ValueError('|---- jetToolBox: Nsubjettiness of subjets needs a Subjet collection. Or create one using addSoftDropSubjets option, or updateCollection.')

		mod["Nsubjettiness"] = 'Nsubjettiness'+mod["NsubSubjets"]
		rangeTau = range(1,subjetMaxTau+1)
		_addProcessAndTask(proc, mod["Nsubjettiness"],
				Njettiness.clone( src = cms.InputTag( ( mod["NsubGroomer"] if not updateCollectionSubjets else updateCollectionSubjets ), 'SubJets' ), 
					Njets=cms.vuint32(rangeTau),         # compute 1-, 2-, 3-, 4- subjettiness
					# variables for measure definition : 
					measureDefinition = cms.uint32( 0 ), # CMS default is normalized measure
					beta = cms.double(1.0),              # CMS default is 1
					R0 = cms.double( jetSize ),              # CMS default is jet cone size
					Rcutoff = cms.double( 999.0),       # not used by default
					# variables for axes definition :
					axesDefinition = cms.uint32( 6 ),    # CMS default is 1-pass KT axes
					nPass = cms.int32(999),             # not used by default
					akAxesR0 = cms.double(-999.0) ) )        # not used by default

		elemToKeep += [ 'keep *_'+mod["Nsubjettiness"]+'_*_*' ]
		for tau in rangeTau: getattr( proc, ( mod["NsubPATSubjets"] if not updateCollectionSubjets else patSubJets ) ).userData.userFloats.src += [mod["Nsubjettiness"]+':tau'+str(tau) ] 
		jetSeq += getattr(proc, mod["Nsubjettiness"])
		toolsUsed.append( mod["Nsubjettiness"] )

	###### QGTagger
	if addQGTagger:
		if ( 'ak4' in jetalgo ) and ( PUMethod not in ['Puppi','CS','SK'] ) :
			from RecoJets.JetProducers.QGTagger_cfi import QGTagger
			proc.load('RecoJets.JetProducers.QGTagger_cfi') 	## In 74X you need to run some stuff before.
			mod["QGTagger"] = 'QGTagger'+mod["PATJetsLabelPost"]
			_addProcessAndTask(proc, mod["QGTagger"],
					QGTagger.clone(
						srcJets = cms.InputTag( mod["PFJetsOrUpdate"] ),    # Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)
						jetsLabel = cms.string('QGL_AK4PF'+QGjetsLabel)        # Other options (might need to add an ESSource for it): see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
						)
					)
			elemToKeep += [ 'keep *_'+mod["QGTagger"]+'_*_*' ]
			getattr( proc, mod["PATJets"]).userData.userFloats.src += [mod["QGTagger"]+':qgLikelihood']
			jetSeq += getattr(proc, mod["QGTagger"])

			toolsUsed.append( mod["QGTagger"] )
		else:
			raise ValueError('|---- jetToolBox: QGTagger is optimized for ak4 jets with CHS. NOT running QGTagger')

			
	####### Pileup JetID
	if addPUJetID:
		if ( 'ak4' in jetalgo ) and ( PUMethod not in ['CS','SK'] ):
			if PUMethod=="Puppi" and verbosity>=1: print('|---- jetToolBox: PUJetID is not yet optimized for ak4 PFjets with PUPPI. USE ONLY FOR TESTING.')
			from RecoJets.JetProducers.pileupjetidproducer_cfi import pileupJetIdCalculator,pileupJetIdEvaluator

			mod["PUJetIDCalc"] = mod["PATJetsLabelPost"]+'pileupJetIdCalculator'
			_addProcessAndTask(proc, mod["PUJetIDCalc"],
					pileupJetIdCalculator.clone(
						jets = cms.InputTag( mod["PFJetsOrUpdate"] ),
						rho = cms.InputTag("fixedGridRhoFastjetAll"),
						vertexes = cms.InputTag(pvLabel),
						applyJec = cms.bool(True),
						inputIsCorrected = cms.bool(False)
						))

			mod["PUJetIDEval"] = mod["PATJetsLabelPost"]+'pileupJetIdEvaluator'
			_addProcessAndTask(proc, mod["PUJetIDEval"],
					pileupJetIdEvaluator.clone(
						jetids = cms.InputTag(mod["PUJetIDCalc"]),
						jets = cms.InputTag( mod["PFJetsOrUpdate"] ),
						rho = cms.InputTag("fixedGridRhoFastjetAll"),
						vertexes = cms.InputTag(pvLabel)
						)
					)

			getattr( proc, mod["PATJets"]).userData.userFloats.src += [mod["PUJetIDEval"]+':fullDiscriminant']
			getattr( proc, mod["PATJets"]).userData.userInts.src += [mod["PUJetIDEval"]+':cutbasedId',mod["PUJetIDEval"]+':fullId']
			elemToKeep += ['keep *_'+mod["PUJetIDEval"]+'_*_*']
			toolsUsed.append( mod["PUJetIDEval"] )
		else:
			raise ValueError('|---- jetToolBox: PUJetID is optimized for ak4 PFjets with CHS. NOT running PUJetID.')

	###### Energy Correlation Functions
	if addEnergyCorrFunc:
		if PUMethod!="Puppi" or (addSoftDrop==False and addSoftDropSubjets==False):
			raise ValueError("|---- jetToolBox: addEnergyCorrFunc only supported for Puppi w/ addSoftDrop or addSoftDropSubjets")
		from RecoJets.JetProducers.ECF_cff import ecfNbeta1, ecfNbeta2
		mod["ECFnb1"] = 'nb1'+mod["SubstructureLabel"]+'SoftDrop'
# 		mod["ECFnb2"] = 'nb2'+mod["SubstructureLabel"]+'SoftDrop'
		_addProcessAndTask(proc, mod["ECFnb1"], ecfNbeta1.clone(src=cms.InputTag(mod["PFJetsSoftDrop"]), cuts=cms.vstring('', '', 'pt > 250')))
# 		_addProcessAndTask(proc, mod["ECFnb2"], ecfNbeta2.clone(src=cms.InputTag(mod["PFJetsSoftDrop"]), cuts=cms.vstring('', '', 'pt > 250')))
		elemToKeep += [ 'keep *_' + mod["ECFnb1"] + '_*_*',
					# 'keep *_'+mod["ECFnb2"]+'_*_*'
					]
		jetSeq += getattr(proc, mod["ECFnb1"])
# 		jetSeq += getattr(proc, mod["ECFnb2"])
# 		toolsUsed.extend([mod["ECFnb1"], mod["ECFnb2"]])
		toolsUsed.extend([mod["ECFnb1"]])

		# set up user floats
		getattr(proc, mod["PATJetsSoftDrop"]).userData.userFloats.src += [
			mod["ECFnb1"]+':ecfN2',
			mod["ECFnb1"]+':ecfN3',
# 			mod["ECFnb2"]+':ecfN2',
# 			mod["ECFnb2"]+':ecfN3',
		]
		# rekey the groomed ECF value maps to the ungroomed reco jets, which will then be picked
		# up by PAT in the user floats. 
		mod["PFJetsSoftDropValueMap"] = mod["PFJetsSoftDrop"]+'ValueMap'
		_addProcessAndTask(proc, mod["PFJetsSoftDropValueMap"],
			cms.EDProducer("RecoJetToPatJetDeltaRValueMapProducer",
				src = cms.InputTag(mod["PFJets"]),
				matched = cms.InputTag(mod["PATJetsSoftDrop"]),
				distMax = cms.double(jetSize),
				values = cms.vstring([
					'userFloat("'+mod["ECFnb1"]+':ecfN2'+'")',
					'userFloat("'+mod["ECFnb1"]+':ecfN3'+'")',
# 					'userFloat("'+mod["ECFnb2"]+':ecfN2'+'")',
# 					'userFloat("'+mod["ECFnb2"]+':ecfN3'+'")',
				]),
				valueLabels = cms.vstring( [
					mod["ECFnb1"]+'N2',
					mod["ECFnb1"]+'N3',
# 					mod["ECFnb2"]+'N2',
# 					mod["ECFnb2"]+'N3',
				]),
			)
		)
		getattr(proc, mod["PATJets"]).userData.userFloats.src += [
			mod["PFJetsSoftDropValueMap"]+':'+mod["ECFnb1"]+'N2',
			mod["PFJetsSoftDropValueMap"]+':'+mod["ECFnb1"]+'N3',
# 			mod["PFJetsSoftDropValueMap"]+':'+mod["ECFnb2"]+'N2',
# 			mod["PFJetsSoftDropValueMap"]+':'+mod["ECFnb2"]+'N3',
		]

	if addEnergyCorrFuncSubjets:
		if PUMethod!="Puppi" or addSoftDropSubjets==False:
			raise ValueError("|---- jetToolBox: addEnergyCorrFuncSubjets only supported for Puppi w/ addSoftDropSubjets")
		from RecoJets.JetProducers.ECF_cff import ecfNbeta1, ecfNbeta2
		mod["ECFnb1Subjets"] = 'nb1'+mod["SubstructureLabel"]+'SoftDropSubjets'
		mod["ECFnb2Subjets"] = 'nb2'+mod["SubstructureLabel"]+'SoftDropSubjets'
		_addProcessAndTask(proc, mod["ECFnb1Subjets"], ecfNbeta1.clone(src=cms.InputTag(mod["PFJetsSoftDrop"], 'SubJets')))
		_addProcessAndTask(proc, mod["ECFnb2Subjets"], ecfNbeta2.clone(src=cms.InputTag(mod["PFJetsSoftDrop"], 'SubJets')))
		elemToKeep += [ 'keep *_'+mod["ECFnb1Subjets"]+'_*_*', 'keep *_'+mod["ECFnb2Subjets"]+'_*_*']
		jetSeq += getattr(proc, mod["ECFnb1Subjets"])
		jetSeq += getattr(proc, mod["ECFnb2Subjets"])
		toolsUsed.extend([mod["ECFnb1Subjets"],mod["ECFnb2Subjets"]])

		# set up user floats
		getattr(proc, mod["PATSubjetsSoftDrop"]).userData.userFloats.src += [
			mod["ECFnb1Subjets"]+':ecfN2',
			mod["ECFnb1Subjets"]+':ecfN3',
			mod["ECFnb2Subjets"]+':ecfN2',
			mod["ECFnb2Subjets"]+':ecfN3',
		]

	if hasattr(proc, 'patJetPartons'): proc.patJetPartons.particles = genParticlesLabel

	_addProcessAndTask(proc, mod["selPATJets"], selectedPatJets.clone(src=mod["PATJets"], cut=Cut))
	elemToKeep += [ 'keep *_'+mod["selPATJets"]+'_*_*' ]
	elemToKeep += [ 'drop *_'+mod["selPATJets"]+'_calo*_*' ]
	elemToKeep += [ 'drop *_'+mod["selPATJets"]+'_tagInfos_*' ]

	if updateCollectionSubjets:
		mod["PATSubjets"] = patJets+mod["PATSubjetsLabel"]
		mod["selPATSubjets"] = selPatJets+mod["PATSubjetsLabel"]

		_addProcessAndTask(proc, mod["selPATSubjets"], selectedPatJets.clone(src=mod["PATSubjets"], cut=Cut))

		getattr(proc, mod["PATSubjets"]).addTagInfos = cms.bool(True)
		#getattr(proc, mod["selPATSubjets"]).addTagInfos = cms.bool(True)
		elemToKeep += [ 'keep *_'+mod["selPATSubjets"]+'__*' ]


	if len(toolsUsed) > 0 and verbosity>=2: print('|---- jetToolBox: Running '+', '.join(toolsUsed)+'.')
	if verbosity>=2: print('|---- jetToolBox: Creating '+mod["selPATJets"]+' collection.')
	if updateCollectionSubjets and verbosity>=2: print('|---- jetToolBox: Creating '+mod["selPATSubjets"]+' collection.')

	### "return"
	setattr(proc, jetSequence, jetSeq)
	if outputFile!='':
		if hasattr(proc, outputFile): getattr(proc, outputFile).outputCommands += elemToKeep
		else: setattr( proc, outputFile, 
				cms.OutputModule('PoolOutputModule', 
					fileName = cms.untracked.string('jettoolbox.root'), 
					outputCommands = cms.untracked.vstring( elemToKeep ) ) )

	if associateTask:
		from PhysicsTools.PatAlgos.tools.helpers import getPatAlgosToolsTask
		task = getPatAlgosToolsTask(proc)
		if hasattr(proc, 'endpath'):
			getattr(proc, 'endpath').associate(task)
		else:
			setattr(proc, 'endpath', cms.EndPath(task))
		if outputFile != '': 
			getattr(proc, 'endpath').insert(-1, getattr(proc, outputFile))

	#### removing mc matching for data
	if runOnData:
		from PhysicsTools.PatAlgos.tools.coreTools import removeMCMatching
		removeMCMatching(proc, names=['Jets'], outputModules=[outputFile])

	if verbosity>=3:
		print('|---- jetToolBox: List of modules created (and other internal names):')
		for m in mod:
			print('      '+m+' = '+mod[m])


def _addProcessAndTask(proc, label, module):
	from PhysicsTools.PatAlgos.tools.helpers import getPatAlgosToolsTask, addToProcessAndTask
	task = getPatAlgosToolsTask(proc)
	addToProcessAndTask(label, module, proc, task)
