# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: test_nanoHRT_data -n 1000 --data --eventcontent NANOAOD --datatier NANOAOD --conditions 102X_dataRun2_v8 --step NANO --nThreads 4 --era Run2_2017,run2_nanoAOD_94XMiniAODv2 --customise PhysicsTools/NanoHRT/nanoHRT_cff.nanoHRT_customizeData_METMuEGClean --filein /store/data/Run2016G/JetHT/MINIAOD/03Feb2017-v1/100000/006E7AF2-AEEC-E611-A88D-7845C4FC3B00.root --fileout file:nano_data.root --customise_commands process.options = cms.untracked.PSet ( wantSummary = cms.untracked.bool ( True ) ) --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('NANO',eras.Run2_2017,eras.run2_nanoAOD_94XMiniAODv2)

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
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2017D/JetHT/MINIAOD/31Mar2018-v1/60000/E68B6D32-CF39-E811-97C3-0CC47A7C34C4.root'),
    #firstEvent = cms.untracked.uint32(867013995),
    #firstRun  = cms.untracked.uint32(302393),
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
process.NanoAOD_Filter = cms.EDFilter('NanoAOD_Filter',
			srcAK4 = cms.InputTag("slimmedJetsPuppi"),
			srcAK8 = cms.InputTag("slimmedJetsAK8"),
			srcmu = cms.InputTag("slimmedMuons"),
			srcele = cms.InputTag("slimmedElectrons"))

process.filt_step = cms.Path(process.NanoAOD_Filter)


outputCommandsHRT = process.NANOAODEventContent.outputCommands
outputCommandsHRT.append("drop *")
outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_puTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_photonTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_vertexTable_pv_*")
outputCommandsHRT.append("keep edmTriggerResults_*_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_btagWeightTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_jetTable_*_*")

# Output definition

process.NANOAODoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAOD'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:nano_data.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents =  cms.vstring('filt_step')),
    outputCommands = outputCommandsHRT
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v8', '')

# Path and EndPath definitions
process.nanoAOD_step = cms.Path(process.nanoSequence)
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

# End of customisation functions

# Customisation from command line

process.options = cms.untracked.PSet ( wantSummary = cms.untracked.bool ( True ) )

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(1)
process.options.numberOfStreams=cms.untracked.uint32(0)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
