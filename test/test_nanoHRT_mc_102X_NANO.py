# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: test_nanoHRT_mc -n 1000 --mc --eventcontent NANOAODSIM --datatier NANOAODSIM --conditions 102X_mc2017_realistic_v6 --step NANO --nThreads 4 --era Run2_2017,run2_nanoAOD_94XMiniAODv2 --customise PhysicsTools/NanoHRT/nanoHRT_cff.nanoHRT_customizeMC --filein /store/mc/RunIISummer16MiniAODv2/ZprimeToTT_M-3000_W-30_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/D6D620EF-73BE-E611-8BFB-B499BAA67780.root --fileout file:nano_mc.root --customise_commands process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True)) --no_exec
import FWCore.ParameterSet.Config as cms

import sys
import FWCore.ParameterSet.VarParsing as opts
options = opts.VarParsing ('analysis')
options.register('Settype',
     'None',
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.string,
     'top, QCD, etc...')
options.register('Recordevery',
     1,
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.int,
     'Record every Xth event (at random)')
options.parseArguments()

Settype = options.Settype
if Settype !="None":
	print "running over",Settype
else:
	sys.exit("Need to specify a set type")
if options.Recordevery>1:
	print "recording every",options.Recordevery,"events"


from Configuration.StandardSequences.Eras import eras

process = cms.Process('NANO',eras.Run2_2017,eras.run2_nanoAOD_94XMiniAODv2)

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
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/ZprimeToTT_M-3000_W-30_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/80000/D6D620EF-73BE-E611-8BFB-B499BAA67780.root'),
    #fileNames = cms.untracked.vstring('/store/user/knash/WkkToRWToTri_Wkk3000R300_WW_private_TuneCP5_13TeV-madgraph-pythia8_v2/MINIAODSIM/190123_161539/0000/B2G-RunIIFall17DRPremix-00528_583.root'),
    #fileNames = cms.untracked.vstring('/store/user/knash/WkkToRWToTri_Wkk3000R200_ZA_private_TuneCP5_13TeV-madgraph-pythia8_v2/MINIAODSIM/190120_224229/0000/B2G-RunIIFall17DRPremix-00528_10.root'),
    #fileNames = cms.untracked.vstring('file:///afs/cern.ch/work/k/knash/NanoHRT/CMSSW_10_2_9/src/PhysicsTools/NanoHRT/test/kkwww4000_06.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/BulkGravToZZToZhadZhad_narrow_M-1200_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/00000/0A3ADA5C-891A-E911-8512-001E67E6F869.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/WkkToWRadionToWWW_M4500-R0-08_TuneCP5_13TeV-madgraph/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/30000/EA3AB5C3-FA63-E811-A35D-0025904CBF10.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/BulkGravTohhTohbbhbb_width0p10_M-2400_TuneCP2_13TeV-madgraph_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/260000/6CE7B40C-F857-E911-80B6-0CC47A13CCFC.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/GluGluToBulkGravitonToHHTo4C_M-2500_narrow_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/40000/C627A459-6CA9-E811-9B66-0242AC130002.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/BulkGravTohhTohVVhbb_narrow_M-4500_TuneCP5_13TeV-madgraph-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/80000/425131B4-2927-E911-B46A-AC1F6BAB6860.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/60000/FEBFEC07-54B6-E611-BF15-001E677928A4.root'),
    fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv3/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/110000/E047F225-4EE6-E811-8EC2-0242AC130002.root'),
    #fileNames = cms.untracked.vstring('file:///afs/cern.ch/user/k/knash/Workspace/NanoHRT/CMSSW_10_2_9/src/PhysicsTools/NanoHRT/test/Failfile.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/ZprimeToTT_M1000_W100_TuneCP2_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/80000/AE3459E4-B525-E911-817F-0CC47AACFCDE.root'),
    #fileNames = cms.untracked.vstring("/store/mc/RunIIFall17MiniAODv2/BulkGravTohhTohbbhbb_width0p05_M-1200_TuneCP2_13TeV-madgraph_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/260000/E077ADA4-A254-E911-84BC-0CC47A545298.root"),
    #fileNames = cms.untracked.vstring('/store/mc/RunIIFall17MiniAODv2/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/80000/F450DBD1-7841-E811-BB7A-B499BAA67BB8.root'),
    secondaryFileNames = cms.untracked.vstring(),

)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('test_nanoHRT_mc nevts:1000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition


outputCommandsHRT = process.NANOAODSIMEventContent.outputCommands
outputCommandsHRT.append("drop *")
outputCommandsHRT.append("keep nanoaodFlatTable_customAK8Table_*_*")
outputCommandsHRT.append("keep nanoaodFlatTable_hotvrTable_*_*")


process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAODSIM'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(SelectEvents =  cms.vstring('f1')),
    fileName = cms.untracked.string('file:nano_mc.root'),
    outputCommands = outputCommandsHRT
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mc2017_realistic_v7', '')




process.Random_Filter = cms.EDFilter("Random_Filter",saveevery=cms.untracked.int32(options.Recordevery))
process.f1 = cms.Path(process.Random_Filter)



# Path and EndPath definitions
process.nanoAOD_step = cms.Path(process.Random_Filter*process.nanoSequenceMC)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(process.NANOAODSIMoutput)

process.dump=cms.EDAnalyzer('EventContentAnalyzer')
process.p = cms.Path(process.dump)
# Schedule definition
#process.schedule = cms.Schedule(process.nanoAOD_step,process.p,process.endjob_step,process.NANOAODSIMoutput_step)
process.schedule = cms.Schedule(process.f1,process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeMC 

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
process = nanoAOD_customizeMC(process)

# Automatic addition of the customisation function from PhysicsTools.NanoHRT.nanoHRT_cff
from PhysicsTools.NanoHRT.nanoHRT_cff import nanoHRT_customizeMC 

#call to customisation function nanoHRT_customizeMC imported from PhysicsTools.NanoHRT.nanoHRT_cff
process = nanoHRT_customizeMC(process,Settype)

# End of customisation functions

# Customisation from command line

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)



# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
