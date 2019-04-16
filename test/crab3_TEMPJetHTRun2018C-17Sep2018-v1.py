from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()


curset = "JetHT/Run2018C-17Sep2018-v1/MINIAOD" 
substr = curset.split('/')[0]
substr1 = curset.split('/')[1]
substr+=substr1
print curset
print substr
ver ='_v7'
config.section_('General')
config.General.requestName = substr+'NanoSlimNtuples'+ver
config.General.workArea = 'crab_projects'
config.General.transferLogs = False
config.General.transferOutputs = True
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'NanoAOD_Slim_data2018_NANO10_2_9.py'
config.JobType.numCores = 4
config.JobType.sendExternalFolder = True
config.JobType.maxMemoryMB = 5000
config.JobType.allowUndistributedCMSSW = True
config.section_('Data')
config.Data.inputDataset = "/"+curset
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob =40
config.Data.outLFNDirBase = '/store/user/knash'
config.Data.publication = False
config.Data.outputDatasetTag = 'NanoSlimNtuples'+ver+substr1
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt'
config.Data.ignoreLocality=True
config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
config.Site.whitelist = ["T2_US*"]
