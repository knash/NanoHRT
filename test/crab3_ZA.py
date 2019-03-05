from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()


curset = "WkkToRWToTri_Wkk3000R100_ZA_private_TuneCP5_13TeV-madgraph-pythia8_v2/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER" 
substr = curset.split('/')[0]
print curset
print substr
ver ='_v0'

config.General.requestName = substr+'imagednn_fortraining'+ver
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'test_nanoHRT_mc_102X_NANO.py'
config.JobType.outputFiles = ['debugout.dat']
config.JobType.pyCfgParams = ['Settype=pho']
config.JobType.allowUndistributedCMSSW = True

config.Data.inputDataset = "/"+curset
config.Data.inputDBS = 'global'
config.Data.splitting = 'Automatic'
config.Data.unitsPerJob = 300
config.Data.outLFNDirBase = '/store/user/knash/EOS'
config.Data.publication = False
config.Data.outputDatasetTag = 'debugtext_fortraining'+ver

config.Site.storageSite = 'T3_US_Rutgers'
