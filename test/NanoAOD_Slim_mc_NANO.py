# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: test_nanoHRT_mc -n 1000 --mc --eventcontent NANOAODSIM --datatier NANOAODSIM --conditions 94X_mcRun2_asymptotic_v2 --step NANO --nThreads 4 --era Run2_2016,run2_miniAOD_80XLegacy --customise PhysicsTools/NanoHRT/nanoHRT_cff.nanoHRT_customizeMC --filein /store/mc/RunIISummer16MiniAODv2/ZprimeToTT_M-3000_W-30_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/D6D620EF-73BE-E611-8BFB-B499BAA67780.root --fileout file:nano_mc.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('NANO',eras.Run2_2016,eras.run2_miniAOD_80XLegacy)

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
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:///afs/cern.ch/user/k/knash/Workspace/NanoAODskim/CMSSW_9_4_11_cand1/src/PhysicsTools/NanoHRT/test/ZP.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('test_nanoHRT_mc nevts:1000'),
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



outputCommandsHRT = process.NANOAODSIMEventContent.outputCommands
outputCommandsHRT.append("drop *")
outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_genParticleTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_genWeightsTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_puTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_photonTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_vertexTable_pv_*")
outputCommandsHRT.append("keep edmTriggerResults_*_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_btagWeightTable_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_jetTable_*_*")



# Output definition
process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAODSIM'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:nano_mc.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents =  cms.vstring('filt_step')),
    outputCommands = outputCommandsHRT
)

#outputCommands.append(drop)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_mcRun2_asymptotic_v2', '')


process.nanoAOD_step = cms.Path(process.NanoAOD_Filter*process.nanoSequenceMC)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)
#cms.Sequence(process.filt_step,process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)

process.schedule = cms.Schedule(process.filt_step,process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
# Schedule definition

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

##Setup FWK for multithreaded
#process.options.numberOfThreads=cms.untracked.uint32(4)
#process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC 


#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeMC(process)

# Automatic addition of the customisation function from PhysicsTools.NanoHRT.nanoHRT_cff
from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeMC 

#call to customisation function nanoHRT_customizeMC imported from PhysicsTools.NanoHRT.nanoHRT_cff
process = nanoHRT_customizeMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
