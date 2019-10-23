# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('NANOHRT',eras.Run2_2016,eras.run2_nanoAOD_94X2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('/store/data/Run2017D/SingleMuon/MINIAOD/31Mar2018-v1/00000/FEBB1B94-5538-E811-9808-20CF3027A610.root'),
    fileNames = cms.untracked.vstring('/store/data/Run2016E/SingleMuon/MINIAOD/17Jul2018-v1/20000/18DAB1D2-DA8C-E811-BCBF-008CFA197DF0.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('test_nanoHRT_data nevts:1000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)


# Path and EndPath definitions

outputCommandsHRT = process.NANOAODEventContent.outputCommands

outputCommandsHRT.append("drop *")
outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_customAK8SubJetTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_jetTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_hotvrTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_hotvrSubJetTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_muonTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_electronTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_metTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_triggerObjectTable_*_*")
outputCommandsHRT.append("keep edmTriggerResults_*_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_extraFlagsTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_rhoTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_simpleCleanerTable_electrons_*")
outputCommandsHRT.append("keep nanoaodFlatTable_simpleCleanerTable_muons_*")
outputCommandsHRT.append("keep nanoaodFlatTable_simpleCleanerTable_jets_*")
outputCommandsHRT.append("keep nanoaodFlatTable_vertexTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_customAK4Table_*_*")


# Output definition

process.NANOAODoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAOD'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:NanoAODv5skim.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents =  cms.vstring('filt_step')),
    outputCommands = outputCommandsHRT
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v11', '')

# Path and EndPath definitions

process.NanoAODFilterSlep = cms.EDFilter('NanoAODFilterSlep',
        		merge=cms.bool(True),
			srcAK4 = cms.InputTag("slimmedJets"),
			srcAK8 = cms.InputTag("slimmedJetsAK8"),
			#srcmet = cms.InputTag("slimmedMETs"),
			#Why god why are the minor labels different for every year
			srcmet = cms.InputTag("slimmedMETs","","DQM"),
			srcmu = cms.InputTag("slimmedMuons"),
			srcel = cms.InputTag("slimmedElectrons"))


process.filt_step = cms.Path(process.NanoAODFilterSlep)
process.dump=cms.EDAnalyzer('EventContentAnalyzer')
process.p = cms.Path( process.dump)

process.nanoAOD_step = cms.Path(process.NanoAODFilterSlep*process.nanoSequence)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODoutput_step = cms.EndPath(process.NANOAODoutput)

# Schedule definition
process.schedule = cms.Schedule(process.filt_step,process.nanoAOD_step,process.endjob_step,process.NANOAODoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)



# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeData 

#call to customisation function nanoAOD_customizeData imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeData(process)

# Automatic addition of the customisation function from PhysicsTools.NanoHRT.nanoHRT_cff
from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeData

#call to customisation function nanoHRT_customizeData_METMuEGClean imported from PhysicsTools.NanoHRT.nanoHRT_cff
process =  nanoHRT_customizeData(process)

from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeAK4
process =  nanoHRT_customizeAK4(process,False)

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# End of customisation functions

# Customisation from command line

process.options = cms.untracked.PSet ( wantSummary = cms.untracked.bool ( True ) )

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
