# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts
options = opts.VarParsing ('analysis')
options.register('ijob',
     1,
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.int,
     'job')
options.register('totjobs',
     1,
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.int,
     'tot jobs')
options.parseArguments()
exstr=""
if options.totjobs!=1:
	exstr="_job"+str(options.ijob)+"of"+str(options.totjobs)

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
    input = cms.untracked.int32(90000)
)

# Input source
process.source = cms.Source("PoolSource",
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
    secondaryFileNames = cms.untracked.vstring()
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






outputCommandsHRT = process.NANOAODSIMEventContent.outputCommands
outputCommandsHRT.append("drop *")
#outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_customWBAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_customWWAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_customMJAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_WBhotvrTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_WWhotvrTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_MJhotvrTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_genParticleTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_genWeightsTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_puTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_photonTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_muonTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_electronTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_matchvartableWW_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_matchvartableWB_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_matchvartableMJ_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_vertexTable_pv_*")
#outputCommandsHRT.append("keep edmTriggerResults_*_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_btagWeightTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_jetTable_*_*")



# Output definition

process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAODSIM'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:NanoAODSIMv5skim'+exstr+'.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents =  cms.vstring('filt_step_full')),
    outputCommands = outputCommandsHRT
)


# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mc2017_realistic_v7', '')

# Path and EndPath definitions

#process.NanoAOD_Filter = cms.EDFilter('NanoAOD_Filter',
#			srcAK4 = cms.InputTag("slimmedJets"))
#process.filt_step = cms.Path(process.NanoAOD_Filter)

process.NanoAODFilterSlep = cms.EDFilter('NanoAODFilterSlep',
        		merge=cms.bool(True),
			srcAK4 = cms.InputTag("slimmedJets"),
			srcAK8 = cms.InputTag("slimmedJetsAK8"),
			srcmet = cms.InputTag("slimmedMETs","","PAT"),
			srcmu = cms.InputTag("slimmedMuons"))

process.NanoAOD_Filter_Slep_post= cms.EDFilter('NanoAOD_Filter_Slep_post',
			srcAK8 = cms.InputTag("WWProducerpuppi","wjet"),
			srcAK4 = cms.InputTag("WWProducerpuppi","bjet"))

process.filt_step = cms.Path(process.NanoAODFilterSlep)
process.filt_step_full = cms.Path(process.NanoAODFilterSlep)

process.nanoAOD_step = cms.Path(process.NanoAODFilterSlep*process.nanoSequenceMC)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)

Bdiscs = ['pfDeepFlavourJetTags:probb', 'pfDeepFlavourJetTags:probbb', 'pfDeepFlavourJetTags:probuds', 'pfDeepFlavourJetTags:probg' , 'pfDeepFlavourJetTags:problepb', 'pfDeepFlavourJetTags:probc','pfCombinedInclusiveSecondaryVertexV2BJetTags','pfDeepCSVJetTags:probb', 'pfDeepCSVJetTags:probbb']

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


process.WWProducerpuppi = cms.EDProducer('WtagProducer',
        		src=cms.InputTag('slimmedJetsAK8'),
			srcAK4 = cms.InputTag("slimmedJets"),
        		sj=cms.InputTag('selectedUpdatedPatJetsAK8PFPuppiwithbtagSoftDropPacked'),
			sec=cms.InputTag('slimmedSecondaryVertices'),
			mu=cms.InputTag('slimmedMuons'),
			el=cms.InputTag('slimmedElectrons'),
        		merge=cms.bool(True),
        		mergeb=cms.bool(False),
        		mergemj=cms.bool(False),
        		load=cms.bool(True),
        		pnum=cms.uint32(options.ijob),
        		tnum=cms.uint32(options.totjobs))

process.WWProducerpuppi1 = cms.EDProducer('WtagProducer1',
        		links= cms.InputTag("WWProducerpuppi","secvertrekey"),
			sec = cms.InputTag("WWProducerpuppi","newvecs"),
        		pfcs=cms.InputTag("WWProducerpuppi"),
			mus=cms.InputTag("WWProducerpuppi","newmus"))


process.WBProducerpuppi = cms.EDProducer('WtagProducer',
        		src=cms.InputTag('slimmedJetsAK8'),
			srcAK4 = cms.InputTag("slimmedJets"),
        		sj=cms.InputTag('selectedUpdatedPatJetsAK8PFPuppiwithbtagSoftDropPacked'),
			sec=cms.InputTag('slimmedSecondaryVertices'),
			mu=cms.InputTag('slimmedMuons'),
			el=cms.InputTag('slimmedElectrons'),
        		merge=cms.bool(False),
        		mergeb=cms.bool(True),
        		mergemj=cms.bool(False),
        		load=cms.bool(True),
        		pnum=cms.uint32(options.ijob),
        		tnum=cms.uint32(options.totjobs))

process.WBProducerpuppi1 = cms.EDProducer('WtagProducer1',
        		links= cms.InputTag("WBProducerpuppi","secvertrekey"),
			sec = cms.InputTag("WBProducerpuppi","newvecs"),
        		pfcs=cms.InputTag("WBProducerpuppi"),
			mus=cms.InputTag("WBProducerpuppi","newmus"))



process.MJProducerpuppi = cms.EDProducer('WtagProducer',
        		src=cms.InputTag('slimmedJetsAK8'),
			srcAK4 = cms.InputTag("slimmedJets"),
        		sj=cms.InputTag('selectedUpdatedPatJetsAK8PFPuppiwithbtagSoftDropPacked'),
			sec=cms.InputTag('slimmedSecondaryVertices'),
			mu=cms.InputTag('slimmedMuons'),
			el=cms.InputTag('slimmedElectrons'),
        		merge=cms.bool(False),
        		mergeb=cms.bool(False),
        		mergemj=cms.bool(True),
        		load=cms.bool(True),
        		pnum=cms.uint32(options.ijob),
        		tnum=cms.uint32(options.totjobs))

from PhysicsTools.NanoAOD.common_cff import *

process.matchvartableWW = cms.EDProducer("GlobalVariablesTableProducer",
    variables = cms.PSet(
        WWdrorig = ExtVar(cms.InputTag("WWProducerpuppi","matchvals0"), float, doc = "aaa"),
        WWdr = ExtVar(cms.InputTag("WWProducerpuppi","matchvals1"), float, doc = "aaa"),
        WWang = ExtVar(cms.InputTag("WWProducerpuppi","matchvals2"), float, doc = "aaa"),
        WWinvm = ExtVar(cms.InputTag("WWProducerpuppi","matchvals3"), float, doc = "aaa"),
        WWfpt = ExtVar(cms.InputTag("WWProducerpuppi","matchvals4"), float, doc = "aaa"),
        WWpt = ExtVar(cms.InputTag("WWProducerpuppi","matchvals5"), float, doc = "aaa"),
        WWopt = ExtVar(cms.InputTag("WWProducerpuppi","matchvals6"), float, doc = "aaa"),
        WWfeta = ExtVar(cms.InputTag("WWProducerpuppi","matchvals7"), float, doc = "aaa"),
        WWeta = ExtVar(cms.InputTag("WWProducerpuppi","matchvals8"), float, doc = "aaa"),
        WWoeta = ExtVar(cms.InputTag("WWProducerpuppi","matchvals9"), float, doc = "aaa"),
        WWfmatch = ExtVar(cms.InputTag("WWProducerpuppi","matchvals10"), float, doc = "aaa"),
        WWfmatchdr = ExtVar(cms.InputTag("WWProducerpuppi","matchvals11"), float, doc = "aaa"),
        WWsumdiffb = ExtVar(cms.InputTag("WWProducerpuppi","matchvals12"), float, doc = "aaa"),
        WWmatch = ExtVar(cms.InputTag("WWProducerpuppi","matchvals13"), float, doc = "aaa"),
        WWmatchdr = ExtVar(cms.InputTag("WWProducerpuppi","matchvals14"), float, doc = "aaa"),
        WWismergetop = ExtVar(cms.InputTag("WWProducerpuppi","matchvals15"), float, doc = "aaa"),
        WWnelsrot = ExtVar(cms.InputTag("WWProducerpuppi","matchvals16"), float, doc = "aaa"),
        WWnmusrot = ExtVar(cms.InputTag("WWProducerpuppi","matchvals17"), float, doc = "aaa"),
    )
)
		
process.matchvartableWB = cms.EDProducer("GlobalVariablesTableProducer",
    variables = cms.PSet(
        WBdrorig = ExtVar(cms.InputTag("WBProducerpuppi","matchvals0"), float, doc = "aaa"),
        WBdr = ExtVar(cms.InputTag("WBProducerpuppi","matchvals1"), float, doc = "aaa"),
        WBang = ExtVar(cms.InputTag("WBProducerpuppi","matchvals2"), float, doc = "aaa"),
        WBinvm = ExtVar(cms.InputTag("WBProducerpuppi","matchvals3"), float, doc = "aaa"),
        WBfpt = ExtVar(cms.InputTag("WBProducerpuppi","matchvals4"), float, doc = "aaa"),
        WBpt = ExtVar(cms.InputTag("WBProducerpuppi","matchvals5"), float, doc = "aaa"),
        WBopt = ExtVar(cms.InputTag("WBProducerpuppi","matchvals6"), float, doc = "aaa"),
        WBfeta = ExtVar(cms.InputTag("WBProducerpuppi","matchvals7"), float, doc = "aaa"),
        WBeta = ExtVar(cms.InputTag("WBProducerpuppi","matchvals8"), float, doc = "aaa"),
        WBoeta = ExtVar(cms.InputTag("WBProducerpuppi","matchvals9"), float, doc = "aaa"),
        WBfmatch = ExtVar(cms.InputTag("WBProducerpuppi","matchvals10"), float, doc = "aaa"),
        WBfmatchdr = ExtVar(cms.InputTag("WBProducerpuppi","matchvals11"), float, doc = "aaa"),
        WBsumdiffb = ExtVar(cms.InputTag("WBProducerpuppi","matchvals12"), float, doc = "aaa"),
        WBmatch = ExtVar(cms.InputTag("WBProducerpuppi","matchvals13"), float, doc = "aaa"),
        WBmatchdr = ExtVar(cms.InputTag("WBProducerpuppi","matchvals14"), float, doc = "aaa"),
        WBismergetop = ExtVar(cms.InputTag("WBProducerpuppi","matchvals15"), float, doc = "aaa"),
        WBnelsrot = ExtVar(cms.InputTag("WBProducerpuppi","matchvals16"), float, doc = "aaa"),
        WBnmusrot = ExtVar(cms.InputTag("WBProducerpuppi","matchvals17"), float, doc = "aaa"),
    )
)

'''
process.matchvartableMJ = cms.EDProducer("GlobalVariablesTableProducer",
    variables = cms.PSet(
        MJdrorig = ExtVar(cms.InputTag("MJProducerpuppi","matchvals0"), float, doc = "aaa"),
        MJdr = ExtVar(cms.InputTag("MJProducerpuppi","matchvals1"), float, doc = "aaa"),
        MJang = ExtVar(cms.InputTag("MJProducerpuppi","matchvals2"), float, doc = "aaa"),
        MJinvm = ExtVar(cms.InputTag("MJProducerpuppi","matchvals3"), float, doc = "aaa"),
        MJfpt = ExtVar(cms.InputTag("MJProducerpuppi","matchvals4"), float, doc = "aaa"),
        MJpt = ExtVar(cms.InputTag("MJProducerpuppi","matchvals5"), float, doc = "aaa"),
        MJopt = ExtVar(cms.InputTag("MJProducerpuppi","matchvals6"), float, doc = "aaa"),
        MJfeta = ExtVar(cms.InputTag("MJProducerpuppi","matchvals7"), float, doc = "aaa"),
        MJeta = ExtVar(cms.InputTag("MJProducerpuppi","matchvals8"), float, doc = "aaa"),
        MJoeta = ExtVar(cms.InputTag("MJProducerpuppi","matchvals9"), float, doc = "aaa"),
        MJfmatch = ExtVar(cms.InputTag("MJProducerpuppi","matchvals10"), float, doc = "aaa"),
        MJfmatchdr = ExtVar(cms.InputTag("MJProducerpuppi","matchvals11"), float, doc = "aaa"),
        MJmatch = ExtVar(cms.InputTag("MJProducerpuppi","matchvals13"), float, doc = "aaa"),
        MJmatchdr = ExtVar(cms.InputTag("MJProducerpuppi","matchvals14"), float, doc = "aaa"),
        MJismergetop = ExtVar(cms.InputTag("MJProducerpuppi","matchvals15"), float, doc = "aaa"),
    )
)
'''
#process.slimmedSecondaryVerticesWBRot = cms.EDProducer("PATSecondaryVertexSlimmer",
#     src = cms.InputTag("WBProducerpuppi","newvecs"),
#     packedPFCandidates = cms.InputTag("WBProducerpuppi")
#)

process.dump=cms.EDAnalyzer('EventContentAnalyzer')
process.p = cms.Path(process.NanoAODFilterSlep*process.dump)

process.w_step = cms.Path(process.NanoAODFilterSlep*process.WBProducerpuppi*process.WWProducerpuppi*process.MJProducerpuppi)
process.w_step_m = cms.Path(process.NanoAODFilterSlep*process.matchvartableWB*process.matchvartableWW)
process.w_step_1 = cms.Path(process.NanoAODFilterSlep*process.WBProducerpuppi1*process.WWProducerpuppi1)

process.schedule = cms.Schedule(process.filt_step,process.w_step,process.w_step_1,process.w_step_m,process.filt_step_full,process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(1)
process.options.numberOfStreams=cms.untracked.uint32(0)
process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC 

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeMC(process)

from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeMC 
process = nanoHRT_customizeMC(process)
from PhysicsTools.NanoHRT.ak8_cff import setupCustomizedWWAK8
setupCustomizedWWAK8(process, runOnMC=False, path=None)
#from PhysicsTools.NanoHRT.hotvr_cff import setupWWHOTVR
#setupWWHOTVR(process, runOnMC=False, path=None)
# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)
#process.SimpleMemoryCheck.jobReportOutputOnly = cms.untracked.bool(False)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
# End of customisation functions

# Customisation from command line
process.particleLevelSequence.remove(process.genParticles2HepMCHiggsVtx);process.particleLevelSequence.remove(process.rivetProducerHTXS);process.particleLevelTables.remove(process.HTXSCategoryTable);process.add_(cms.Service('InitRootHandlers', EnableIMT = cms.untracked.bool(False)))
process.NANOAODSIMoutput.fakeNameForCrab = cms.untracked.bool(True)
# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
