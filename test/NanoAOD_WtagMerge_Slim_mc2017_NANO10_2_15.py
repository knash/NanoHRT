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
    input = cms.untracked.int32(30000)
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
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/1649677B-49BC-E811-BFF1-0242AC130002.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/189F902B-5ABB-E811-87AC-20CF3019DF19.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/E0AB4E92-A6BB-E811-B7C4-001EC9ADCD0C.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/A8488F82-6ABC-E811-A5E0-0CC47AFC3C72.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/A8685FAB-04BE-E811-BC7B-7CD30AD09B08.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/2880148E-55BB-E811-920F-0025905A6056.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/EAE5867E-0BBC-E811-95B3-0025905A60D2.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/22587777-E2BA-E811-B181-1C6A7A26E149.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/E0CD5188-F0BA-E811-B108-0CC47AD9908C.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/30994A38-7BBC-E811-BC8F-0242AC130002.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/E24F2B9D-41BB-E811-8D62-0242AC130002.root",
	"/store/mc/RunIIFall17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/120000/1649677B-49BC-E811-BFF1-0242AC130002.root",
	),
	#fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravTohhTohbbhbb_width0p05_M-1200_TuneCP2_13TeV-madgraph_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/260000/E077ADA4-A254-E911-84BC-0CC47A545298.root"),
	#fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravToZZToZhadZhad_narrow_M-1000_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/80000/507F0D65-ED1C-E911-851E-E0071B73B6E0.root"),
	#fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravToWW_narrow_M-1200_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/10000/CEECBDDE-4C20-E911-AE4C-90E2BAD4912C.root"),
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
#outputCommandsHRT.append("drop *")
#outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_hotvrTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_genParticleTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_genWeightsTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_puTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_photonTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_muonTable_*_*")
#outputCommandsHRT.append("keep nanoaodFlatTable_electronTable_*_*")
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

process.WWProducerpuppi = cms.EDProducer('WtagProducer',
        		src=cms.InputTag('slimmedJetsAK8'),
			srcAK4 = cms.InputTag("slimmedJets"),
        		merge=cms.bool(True),
        		mergeb=cms.bool(False),
        		pnum=cms.uint32(options.ijob),
        		tnum=cms.uint32(options.totjobs))
process.WBProducerpuppi = cms.EDProducer('WtagProducer',
        		src=cms.InputTag('slimmedJetsAK8'),
			srcAK4 = cms.InputTag("slimmedJets"),
        		merge=cms.bool(False),
        		mergeb=cms.bool(True),
        		pnum=cms.uint32(options.ijob),
        		tnum=cms.uint32(options.totjobs))

process.w_step = cms.Path(process.NanoAODFilterSlep*process.WWProducerpuppi*process.WBProducerpuppi)
process.dump=cms.EDAnalyzer('EventContentAnalyzer')
process.p = cms.Path( process.dump)
process.schedule = cms.Schedule(process.filt_step,process.w_step,process.filt_step_full,process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC 

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeMC(process)

from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeMC 
process = nanoHRT_customizeMC(process)
from PhysicsTools.NanoHRT.ak8_cff import setupCustomizedWWAK8
setupCustomizedWWAK8(process, runOnMC=False, path=None)
# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions

# Customisation from command line
process.particleLevelSequence.remove(process.genParticles2HepMCHiggsVtx);process.particleLevelSequence.remove(process.rivetProducerHTXS);process.particleLevelTables.remove(process.HTXSCategoryTable);process.add_(cms.Service('InitRootHandlers', EnableIMT = cms.untracked.bool(False)))

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
