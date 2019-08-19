from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

curset = "/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM" 
outloc = "/store/user/knash" 
version = "14" 
store = "T2_CH_CERN" 
runloc = True
fnalonly = False

substr = curset.split('/')[1]
substr1 = curset.split('/')[2]
print substr,substr1

ver ='_v'+version

config.section_('General')
config.General.requestName = substr+'NanoWtagProducer2017mc'+ver
config.General.workArea = 'crab_projects'
config.General.transferLogs = False
config.General.transferOutputs = True
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'NanoAOD_WtagAna_Slim_mc2017_NANO10_2_15.py'
config.JobType.numCores = 4
config.JobType.sendExternalFolder = True
config.JobType.maxMemoryMB = 6000
config.JobType.allowUndistributedCMSSW = True
config.section_('Data')
config.Data.inputDataset = curset
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 700000
config.Data.outLFNDirBase = outloc
config.Data.publication = False
config.Data.outputDatasetTag =substr1+'_NanoWtagProducer2017mc'+ver
if runloc:
	config.section_('Site')
	config.Site.storageSite = store
else:
	config.Data.ignoreLocality=True
	config.section_('Site')
	config.Site.storageSite = store
	if fnalonly:
		config.Site.whitelist = ["T3_US_FNALLPC"] 
	else:
		config.Site.whitelist = ["T2_US*","T3_US_FNALLPC","T1_US*","T2_CH_CERN"] 
	config.Site.ignoreGlobalBlacklist = True

