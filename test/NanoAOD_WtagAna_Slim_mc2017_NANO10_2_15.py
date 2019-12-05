# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('NANOHRT',eras.Run2_2017,eras.run2_nanoAOD_94XMiniAODv2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200000)
)

# Input source
process.source = cms.Source("PoolSource",
   # fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/270000/48E78AC0-5CBB-E811-AF83-0CC47AF9B306.root"),
    #fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/270000/48E78AC0-5CBB-E811-AF83-0CC47AF9B306.root"),
    #fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/100000/62B46A64-9964-E811-9292-0025905C9726.root"),
    # fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/30994A38-7BBC-E811-BC8F-0242AC130002.root"),
    # fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/ZprimeToWWToWlepWhad_narrow_M-1400_TuneCP5_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/10000/20B24084-9C1D-E911-8162-0CC47AB360C6.root"),
     #fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/WpToTpB_Wp3500Nar_Tp1700Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/70000/48968C14-7359-E911-81FC-0242AC130002.root"),
  #   fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/WJetsToQQ_HT600to800_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/D8BAB5EE-DC42-E811-BA23-0CC47A78A3EC.root"),
   #  fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/70000/72D87D33-6941-E911-823F-0025905B859A.root"),
   #fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/20000/6074D5E8-929A-E811-B7AD-B496910A90B8.root"),
	fileNames = cms.untracked.vstring(
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/5271DA24-8D29-E911-9050-EC0D9A8221DE.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/DC54D60B-942C-E911-8055-B083FED04CAA.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/74C6CAA1-2C2D-E911-A212-B02628342C70.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/4C041404-9229-E911-856C-0242AC130002.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/969DFD44-5026-E911-87FD-0CC47AFCC65E.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/482FB5AE-8D29-E911-9BAE-001E6779250C.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/908292DC-8C29-E911-8C54-8CDCD4A9A484.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/F04A14F5-8D29-E911-9199-0CC47A4C8E1E.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/D29BEDF1-A625-E911-9FF7-AC1F6B239D78.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/92479E5F-9426-E911-8959-AC1F6B23C94C.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/1427EC0B-142D-E911-A3D9-0025905C53A6.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/421BAEBD-792C-E911-B27E-AC1F6B23C814.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/A2C99C71-5F2C-E911-9297-FA163ED68858.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/90000/0CEEF95F-8D29-E911-B68D-0CC47A545060.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/AA721AB1-2C2D-E911-97C7-0CC47A78A426.root",
"/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M2000_W20_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/40000/206C93D7-872C-E911-B7A8-0025905A6080.root",	
	),
	#fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravTohhTohbbhbb_width0p05_M-1200_TuneCP2_13TeV-madgraph_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/260000/E077ADA4-A254-E911-84BC-0CC47A545298.root"),
	#fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravToZZToZhadZhad_narrow_M-1000_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/80000/507F0D65-ED1C-E911-851E-E0071B73B6E0.root"),
	#fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravToWW_narrow_M-1200_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/10000/CEECBDDE-4C20-E911-AE4C-90E2BAD4912C.root"),
    secondaryFileNames = cms.untracked.vstring(),
    #firstEvent = cms.untracked.uint32(20792524)
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Path and EndPath definitions






outputCommandsHRT = process.MINIAODSIMEventContent.outputCommands
#outputCommandsHRT.append("drop *")
outputCommandsHRT.append("keep *_WtagProducerpuppi_*_*")
outputCommandsHRT.append("keep *_offlineSlimmedPrimaryVertices_*_*")
outputCommandsHRT.append("keep *_offlinePrimaryVertices_*_*")
outputCommandsHRT.append("keep *_slimmedSecondaryVertices_*_*")
outputCommandsHRT.append("keep *_slimmedMuons_*_*")
outputCommandsHRT.append("keep *_slimmedElectrons_*_*")

#outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_genParticleTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_genWeightsTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_puTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_photonTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_vertexTable_pv_*")
#outputCommandsHRT.append("keep edmTriggerResults_*_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_btagWeightTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_jetTable_*_*")


# Output definition

process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('MINIAODSIM'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:MINIskim.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents =  cms.vstring('filt_step_full')),
    outputCommands = outputCommandsHRT
)


# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mc2017_realistic_v7', '')


Bdiscs = ['pfDeepFlavourJetTags:probb', 'pfDeepFlavourJetTags:probbb', 'pfDeepFlavourJetTags:probuds', 'pfDeepFlavourJetTags:probg' , 'pfDeepFlavourJetTags:problepb', 'pfDeepFlavourJetTags:probc','pfCombinedInclusiveSecondaryVertexV2BJetTags','pfDeepCSVJetTags:probb', 'pfDeepCSVJetTags:probbb']

from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
from PhysicsTools.NanoHRT.jetToolbox_cff import jetToolbox
JETCorrLevels = ['L2Relative', 'L3Absolute', 'L2L3Residual']

bTagDiscriminators = [
        'pfCombinedInclusiveSecondaryVertexV2BJetTags',
        'pfBoostedDoubleSecondaryVertexAK8BJetTags',
        'pfMassIndependentDeepDoubleBvLJetTags:probHbb',
        'pfMassIndependentDeepDoubleCvLJetTags:probHcc',
    ]
subjetBTagDiscriminators = [
        'pfCombinedInclusiveSecondaryVertexV2BJetTags',
        'pfDeepCSVJetTags:probb',
        'pfDeepCSVJetTags:probbb',
    ]
from PhysicsTools.NanoHRT.jetToolbox_cff import jetToolbox
jetToolbox(process, 'ak8', 'dummySeq', 'out', associateTask=False,
               PUMethod='Puppi', JETCorrPayload='AK8PFPuppi', JETCorrLevels=JETCorrLevels,
               Cut='pt > 150.0 && abs(rapidity()) < 2.4',
               miniAOD=True, runOnMC=True,
               addNsub=True, maxTau=4, addEnergyCorrFunc=True,
	       GetSubjetMCFlavour=False,GetJetMCFlavour=False,
               addSoftDrop=True, addSoftDropSubjets=True, subJETCorrPayload='AK4PFPuppi', subJETCorrLevels=JETCorrLevels,
               bTagDiscriminators=bTagDiscriminators,subjetBTagDiscriminators=subjetBTagDiscriminators,subjetBTagInfos = ['pfImpactParameterTagInfos', 'pfSecondaryVertexTagInfos', 'pfInclusiveSecondaryVertexFinderTagInfos'],postFix='newak8')


jetToolbox( process, 'ak8', 'ak8JetSubs', 'out', associateTask=False, 
		updateCollection='selectedPatJetsAK8PFPuppinewak8SoftDrop', JETCorrPayload='AK8PFPuppi', JETCorrLevels=JETCorrLevels,
                Cut='pt > 150.0 && abs(rapidity()) < 2.4', PUMethod='Puppi',
                miniAOD=True, runOnMC=True,bTagDiscriminators=Bdiscs,
		updateCollectionSubjets='selectedPatJetsAK8PFPuppinewak8SoftDropSubjets', subjetBTagDiscriminators=Bdiscs,  subjetBTagInfos = ['pfImpactParameterTagInfos', 'pfSecondaryVertexTagInfos', 'pfInclusiveSecondaryVertexFinderTagInfos'],
		subJETCorrPayload='AK4PFPuppi', subJETCorrLevels=JETCorrLevels,postFix='withbtag')

# Path and EndPath definitions
#++patJets "patJetsAK8PFPuppinewak8" "" "NANOHRT" (productId = 6:68)
#++patJets "patJetsAK8PFPuppinewak8SoftDrop" "" "NANOHRT" (productId = 6:69)
#++patJets "patJetsAK8PFPuppinewak8SoftDropSubjets" "" "NANOHRT" (productId = 6:70)
#++patJets "selectedPatJetsAK8PFPuppinewak8" "" "NANOHRT" (productId = 6:71)
#++patJets "selectedPatJetsAK8PFPuppinewak8SoftDrop" "" "NANOHRT" (productId = 6:72)
#++patJets "selectedPatJetsAK8PFPuppinewak8SoftDropPacked" "" "NANOHRT" (productId = 6:73)
#++patJets "selectedPatJetsAK8PFPuppinewak8SoftDropPacked" "SubJets" "NANOHRT" (productId = 6:74)
#++patJets "selectedPatJetsAK8PFPuppinewak8SoftDropSubjets" "" "NANOHRT" (productId = 6:75)


process.WtagProducerpuppi = cms.EDProducer('WtagProducer',
        		src=cms.InputTag('slimmedJetsAK8'),
			srcAK4 = cms.InputTag("slimmedJets"),
        		sj=cms.InputTag('selectedUpdatedPatJetsAK8PFPuppiwithbtagSoftDropPacked'),
			mu=cms.InputTag('slimmedMuons'),
			el=cms.InputTag('slimmedElectrons'),
			sec=cms.InputTag('slimmedSecondaryVertices'),
        		merge=cms.bool(False),
        		mergeb=cms.bool(False),
        		mergemj=cms.bool(False),
        		load=cms.bool(False),
        		pnum=cms.uint32(1),
        		tnum=cms.uint32(0))

process.NanoAODFilterSlep = cms.EDFilter('NanoAODFilterSlep',
        		merge=cms.bool(False),
			srcAK4 = cms.InputTag("slimmedJets"),
			srcAK8 = cms.InputTag("slimmedJetsAK8"),
			srcmet = cms.InputTag("slimmedMETs"),
			srcmu = cms.InputTag("slimmedMuons"))


process.NanoAOD_Filter_Slep_post= cms.EDFilter('NanoAOD_Filter_Slep_post',
			srcAK8 = cms.InputTag("WtagProducerpuppi","wjet"),
			srcAK4 = cms.InputTag("WtagProducerpuppi","bjet"))


process.filt_step = cms.Path(process.NanoAODFilterSlep)
process.filt_step_post = cms.Path(process.NanoAOD_Filter_Slep_post)
process.filt_step_full = cms.Path(process.NanoAODFilterSlep*process.NanoAOD_Filter_Slep_post)
process.w_step = cms.Path(process.NanoAODFilterSlep*process.WtagProducerpuppi)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.MINIAODSIMoutput_step = cms.EndPath(process.MINIAODSIMoutput)
process.dump=cms.EDAnalyzer('EventContentAnalyzer')
process.p = cms.Path(process.NanoAODFilterSlep*process.dump)
process.schedule = cms.Schedule(process.filt_step,process.w_step,process.filt_step_full,process.endjob_step,process.MINIAODSIMoutput_step)


from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
#from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC 

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
#process = nanoAOD_customizeMC(process)

#from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeMC 
#process = nanoHRT_customizeMC(process)

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)
#process.SimpleMemoryCheck.jobReportOutputOnly = cms.untracked.bool(False)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
# End of customisation functions

# Customisation from command line
process.particleLevelSequence.remove(process.genParticles2HepMCHiggsVtx);process.particleLevelSequence.remove(process.rivetProducerHTXS);process.particleLevelTables.remove(process.HTXSCategoryTable);process.add_(cms.Service('InitRootHandlers', EnableIMT = cms.untracked.bool(False)))

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
