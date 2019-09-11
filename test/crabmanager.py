import copy, os, sys, time, logging, re, subprocess, glob, math, shutil
from os import listdir
from os.path import isfile, join
from optparse import OptionParser
from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException
from multiprocessing import Process, Queue

	
#def psubp(que,strw):
#	try:
#		que.put(subprocess.call( [strw], shell=True ))
#
#	except:
#		#temptosub.write("rm -rf "+fold+"\n")
#		#temptosub.write("crab submit "+fold.replace("crab_projects","tempcrabfiles")+"\n")
#		print "ERR"
    	
#def pcrabp(que,mode,fold):
#	que.put(crabCommand(mode, dir=fold ))
						
parser = OptionParser()

parser.add_option('-s', '--set', metavar='F', type='string', action='store',
	          default	=	'NONE',
	          dest		=	'set',
	          help		=	'csv set list')
parser.add_option('-l', '--location', metavar='F', type='string', action='store',
	          default	=	'NONE',
	          dest		=	'location',
	          help		=	'output eos folder')
parser.add_option('-d', '--site', metavar='F', type='string', action='store',
	          default	=	'NONE',
	          dest		=	'site',
	          help		=	'site (ie T2_CH_CERN etc)')
parser.add_option('-v', '--version', metavar='F', type='string', action='store',
	          default	=	'NONE',
	          dest		=	'version',
	          help		=	'ntuple version')
parser.add_option('-y', '--year', metavar='F', type='string', action='store',
	          default	=	'NONE',
	          dest		=	'year',
	          help		=	'2016,2017,2018')
parser.add_option('-w', '--whitelist', metavar='F', type='string', action='store',
	          default	=	'DEF',
	          dest		=	'whitelist',
	          help		=	'whitelist csv (if runloc=False) or NONE')
parser.add_option('-b', '--blacklist', metavar='F', type='string', action='store',
	          default	=	'DEF',
	          dest		=	'blacklist',
	          help		=	'blacklist csv or NONE')
parser.add_option('-m', '--mode', metavar='F', type='string', action='store',
                  default	=	'submit',
                  dest		=	'mode',
                  help		=	'mode kll, status, resubmit, recover, lumicalc')
parser.add_option('-S', '--search', metavar='F', type='string', action='store',
                  default	=	'NONE',
                  dest		=	'search',
                  help		=	'search string (match crab directory)')
parser.add_option('-o', '--ops', metavar='F', type='string', action='store',
                  default	=	'',
                  dest		=	'ops',
                  help		=	'options')
parser.add_option('--submit', metavar='F', action='store_true',
	          default=False,
	          dest='submit',
	          help='submit to crab')
parser.add_option('--runloc', metavar='F', action='store_true',
	          default=False,
	          dest='runloc',
	          help='runloc')

(options, args) = parser.parse_args()

print('Options summary')
print('==================')
for opt,value in options.__dict__.items():
    print(str(opt) +': '+ str(value))
print('==================')

from WMCore.Configuration import Configuration
queue = Queue()
#processes = [Process(target=rand_num, args=(queue,)) for x in range(4)]
processes = []


#config = config()
if options.mode=="submit":


	sets = (options.set).split(",")
	years = (options.year).split(",")



	if options.location=="NONE":
		logging.error("Please enter a valid location")
		sys.exit()
	if options.version=="NONE":
		logging.error("Please enter a valid version")
		sys.exit()
	if options.set=="NONE":
		logging.error("Please enter a valid set list")
		sys.exit()
	if options.site=="NONE":
		logging.error("Please enter a valid site")
		sys.exit()
	if options.year=="NONE":
		logging.error("Please enter a valid year list")
		sys.exit()

	if options.whitelist=="DEF":
		whitelist = ["T2_US*","T3_US_FNALLPC","T1_US*","T2_CH_CERN"] 
		blacklist = ["T2_EE_Estonia"]
	elif  options.whitelist=="NONE":
		whitelist = [] 
		blacklist = []
	else:
		whitelist = (options.whitelist).split(",")
		blacklist = (options.blacklist).split(",")	

	LOC = options.location
	VER = options.version
	STO = options.site
	RLO = "False"
	OFNA = "False"
	if options.runloc:
		RLO = "True"
	lmask = 	{
			'2016':'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt'	,
			'2017':'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt',
			'2018':'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt'	
			}
	#not sure how much granulariy we need here
	nevperjob = 	{
			"2016_sig":20000,"2017_sig":20000,"2018_sig":20000,
			"2016_ttbar":300000,"2017_ttbar":300000,"2018_ttbar":300000,
			"2016_qcd":50000,"2017_qcd":50000,"2018_qcd":50000,
			"2016_data":100000,"2017_data":100000,"2018_data":100000,
			}
	nconf=0


	for year in years:
		for cset in sets: 
			idstr=year+"_"+cset
			for RSET in open("txtfiles/sets_"+idstr+".txt", "r"):
				config = Configuration()


				RSET = RSET.replace("\n","")
				ver ='_v'+VER
				print RSET
				majset = RSET.split('/')[1]
				minset = RSET.split('/')[2]
				typestr = "mc"
				exstr=""
			 	if cset=="data":
					typestr = "data"
					exstr=minset
				config.section_('General')
				rname= majset+exstr+'NanoSlimNtuples'+year+typestr+ver
				config.General.requestName = rname
				config.General.workArea = 'crab_projects'
				config.General.transferLogs = False
				config.General.transferOutputs = True
				config.section_('JobType')

				config.JobType.pluginName = 'Analysis'
				config.JobType.psetName = 'NanoAOD_Slim_'+typestr+year+'_NANO10_2_15.py'
				config.JobType.numCores = 4
				config.JobType.sendExternalFolder = True
				config.JobType.maxMemoryMB = 8000
				config.JobType.allowUndistributedCMSSW = True
				config.section_('Data')
				config.Data.inputDataset = RSET
				config.Data.inputDBS = 'global'
				config.Data.splitting = 'EventAwareLumiBased'
				config.Data.unitsPerJob = nevperjob[idstr]
				config.Data.outLFNDirBase = LOC
				config.Data.publication = True
				config.Data.outputDatasetTag =minset+'_NanoSlimNtuples'+year+typestr+ver
				if cset=="data":
					config.Data.lumiMask=lmask[year]
				if RLO:
					config.section_('Site')
					config.Site.storageSite = STO
				else:
					config.Data.ignoreLocality=True
					config.section_('Site')

					config.Site.storageSite = STO
					config.Site.whitelist = whitelist
					config.Site.ignoreGlobalBlacklist = True
				config.Site.blacklist = blacklist

				#print dir(config)
				#print config
				tempname = "tempcrabfiles/crab_"+rname+".py"
				tempfile = open(tempname, "w")
				print "saving "+tempname
				tempfile.write(str(config))
				tempfile.close() 
				if options.submit:
					print "submitting "+RSET
					comm = 'crab submit '+tempname
					#processes.append(Process(target=psubp, args=(queue,comm,)))
					subprocess.call( ['crab submit '+tempname+" "+options.ops], shell=True )
					#Absolutely no idea why this doesn't work
					#print config
					#try:
					#    crabCommand('submit', config=config)
					#except HTTPException as hte:
					#    print "Failed submitting task: %s" % (hte.headers)
					#except ClientException as cle:
					#    print "Failed submitting task: %s" % (cle)
				nconf+=1

	#doesnt work for some reason	
	#for p in processes:
	#	p.start()
	#for p in processes:
	#	p.join()
	#results = [queue.get() for p in processes]
	#print(results)
										
else:
	log=open("log","w+")
	commands = []

	folders = glob.glob(options.search)

	output = []
	for fold in folders:
		print "Running",options.mode,"on",fold
		if options.mode in ("status","kill","resubmit"):
			#processes.append(Process(target=pcrabp, args=(queue,options.mode,fold,)))
			
			try:

				#sys.stdout = open("out", 'w')
				sys.stdout = log
				if options.mode=="status":
					crabout = crabCommand(options.mode, dir=fold)
				else:
					crabout = crabCommand(options.mode, dir=fold,*options.ops.split())
				output.append([fold,crabout])	
				#crabout["status"]
				sys.stdout = sys.__stdout__

			
			except:
				sys.stdout = sys.__stdout__
				print "Error!"
				output.append([fold,None])
			if output[-1][1]!=None and options.mode=="status":
				#toresubmit=False
				if output[-1][1]["status"]=="FAILED":
					#for jj in output[-1][1]["jobsPerStatus"]:
					#	print jj
					#	if jj=="failed":
					#		toresubmit=True
					#		break

					
					print "Failed - resubmitting"
					try:
						sys.stdout = log
						reout = crabCommand("resubmit", dir=fold,*options.ops.split())
						sys.stdout = sys.__stdout__
					except:
						sys.stdout = sys.__stdout__
						print "Resubmit Error!"
				elif output[-1][1]["status"]=="COMPLETED":
					print "Completed"
				else:
					print "In Progress"
					

		if options.mode == "recover" or options.mode == "lumicalc":
			commands.append("crab report "+fold)
			if options.mode == "recover":
				commands.append("tar xvf "+fold+"/inputs/debugFiles.tgz")
			if options.mode == "lumicalc":
				commands.append("brilcalc lumi --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json -i "+fold+"/results/processedLumis.json -u /fb")

	for s in commands :
	    print 'executing ' + s
	    subprocess.call( [s], shell=True )
	commands = []
	if options.mode == "recover":
		for fold in folders:
			NFL = fold+"/results/notFinishedLumis.json"
			curcrab = "debug/crabConfig.py"
			crabpyname =fold.split("/")[-1]+"_recov.py"
			fil= open(crabpyname,"w+")
			written=False
			with open(curcrab) as fp:
	   			for line in fp:

	       				lsplit = line.split("=")
					if len(lsplit)==2:
						lsplit[0]=lsplit[0].replace(" ","")
						if lsplit[0]=="config.Data.unitsPerJob":
							lsplit[1]=str(int(0.5*int(lsplit[1])))+"\n"
						if lsplit[0]=="config.Data.lumiMask":
							written=True
							lsplit[1]='\"'+NFL+'\"'+"\n"
						#print lsplit[0]
						if lsplit[0]!="config.Data.outputDatasetTag":
							#print "Found"
							lsplit[1]=lsplit[1].replace("NanoSlimNtuples","NanoSlimNtuples_recovery")
						fil.write(lsplit[0]+"="+lsplit[1])
					else:
						fil.write(line)
			if not written:
				fil.write('config.Data.lumiMask=\"'+NFL+'\"'+"\n")
			#commands.append("crab submit "+crabpyname)
	#for s in commands :
	    #print 'executing ' + s
	    #subprocess.call( [s], shell=True )
	if options.mode=="status":
		temptosub=open("unsubbed.txt","w+")
		jobdict = {'finished': 0, 'transferring': 0, 'running': 0, 'idle': 0, 'failed': 0, 'cooloff': 0, 'unsubmitted':0}
		Ntot = len(output)
		Ncomp = 0		
		Nsub = 0
		Nfailed = 0
		Nerr = 0
		Njobs = 0
		for out in output:
			if out[1]!=None:
				Njobs+=len(out[1]["jobList"])
				for jj in out[1]["jobsPerStatus"]:
					jobdict[jj]+=out[1]["jobsPerStatus"][jj]			
				if out[1]["status"]=="COMPLETED":
					Ncomp+=1
				if out[1]["status"]=="SUBMITTED":
					Nsub+=1
				if out[1]["status"]=="FAILED":
					Nfailed+=1
			else:
				
				Nerr+=1
				temptosub.write("rm -rf "+out[0]+"\n")
				temptosub.write("crab submit "+out[0].replace("crab_projects","tempcrabfiles")+".py\n")
		print "------------"
		print "Task Summary"
		print "\tTotal:",Ntot
		print "\tCompleted:",Ncomp
		print "\tSubmitted:",Nsub
		print "\tFailed:",Nfailed
		print "\tError:",Nerr
		print
		print "Job Summary"
		print "\tTotal:",Njobs
		print "\tUnsubmitted:",jobdict['unsubmitted']
		print "\tIdle:",jobdict['idle']
		print "\tRunning:",jobdict['running']
		print "\tCooloff:",jobdict['cooloff']
		print "\tTransferring:",jobdict['transferring']
		print "\tFinished:",jobdict['finished']
		print "\tFailed:",jobdict['failed']
		print "------------"

