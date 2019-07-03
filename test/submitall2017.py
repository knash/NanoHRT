import copy, os, sys, time, logging, re, subprocess, glob, math
from os import listdir
from os.path import isfile, join

from optparse import OptionParser
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
parser.add_option('--submit', metavar='F', action='store_true',
                  default=False,
                  dest='submit',
                  help='submit to crab')

(options, args) = parser.parse_args()

print('Options summary')
print('==================')
for opt,value in options.__dict__.items():
    print(str(opt) +': '+ str(value))
print('==================')
sets = (options.set).split(",")
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

LOC = (options.location).replace("/","\/")
VER = (options.version).replace("/","\/")
STO = (options.site).replace("/","\/")

tosubmitSIG = [
"WpToTpB_Wp4000Nar_Tp3300Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToTpB_Wp4000Nar_Tp3000Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToTpB_Wp4000Nar_Tp3000Nar_Ht_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToTpB_Wp4000Nar_Tp2700Nar_Ht_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToTpB_Wp4000Nar_Tp2300Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToTpB_Wp4000Nar_Tp2300Nar_Ht_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp4000Nar_Bp3300Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp4000Nar_Bp3300Nar_Ht_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp4000Nar_Bp3000Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp4000Nar_Bp2700Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp4000Nar_Bp2000Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToTpB_Wp2000Nar_Tp1700Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp2000Nar_Bp1700Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp2000Nar_Bp1700Nar_Ht_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp2000Nar_Bp1300Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WpToBpT_Wp2000Nar_Bp1000Nar_Zt_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WkkToWRadionToWWW_M3000-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WkkToWRadionToWWW_M3500-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WkkToWRadionToWWW_M4000-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM",
"WkkToWRadionToWWW_M4500-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WkkToWRadionToWWW_M5000-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"
]


tosubmitQCD = [
"QCD_HT700to1000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
]

tosubmitTT = [
"TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",
]

tosubmitDATA = [
"JetHT/Run2017F-31Mar2018-v1/MINIAOD",
"JetHT/Run2017E-31Mar2018-v1/MINIAOD",
"JetHT/Run2017D-31Mar2018-v1/MINIAOD",
"JetHT/Run2017C-31Mar2018-v1/MINIAOD",
"JetHT/Run2017B-31Mar2018-v1/MINIAOD"
]


commands = []

if "sig" in sets:
	for RSET in tosubmitSIG:
		RSET = RSET.replace("/","\/")
		commands.append("echo "+ RSET)
		tempname = "crab3_TEMP"+RSET.split("/")[0]+RSET.split("/")[1]+"_v"+VER+".py"
		commands.append("sed 's/RSET/"+RSET+"/g' crab_debugstring_SIG2017.TMP > "+tempname)
		commands.append("sed -i 's/LOC/"+LOC+"/g' "+tempname)
		commands.append("sed -i 's/VER/"+VER+"/g' "+tempname)
		commands.append("sed -i 's/STO/"+STO+"/g' "+tempname)
		if options.submit:
			commands.append("crab submit "+tempname)

if "qcd" in sets:
	for RSET in tosubmitQCD:
		RSET = RSET.replace("/","\/")
		commands.append("echo "+ RSET)
		tempname = "crab3_TEMP"+RSET.split("/")[0]+RSET.split("/")[1]+"_v"+VER+".py"
		commands.append("sed 's/RSET/"+RSET+"/g' crab_debugstring_QCD2017.TMP > "+tempname)
		commands.append("sed -i 's/LOC/"+LOC+"/g' "+tempname)
		commands.append("sed -i 's/VER/"+VER+"/g' "+tempname)
		commands.append("sed -i 's/STO/"+STO+"/g' "+tempname)
		if options.submit:
			commands.append("crab submit "+tempname)

if "ttbar" in sets:
	for RSET in tosubmitTT:
		RSET = RSET.replace("/","\/")
		commands.append("echo "+ RSET)
		tempname = "crab3_TEMP"+RSET.split("/")[0]+RSET.split("/")[1]+"_v"+VER+".py"
		commands.append("sed 's/RSET/"+RSET+"/g' crab_debugstring_TT2017.TMP > "+tempname)
		commands.append("sed -i 's/LOC/"+LOC+"/g' "+tempname)
		commands.append("sed -i 's/VER/"+VER+"/g' "+tempname)
		commands.append("sed -i 's/STO/"+STO+"/g' "+tempname)
		if options.submit:
			commands.append("crab submit "+tempname)


if "data" in sets:
	for RSET in tosubmitDATA:
		RSET = RSET.replace("/","\/")
		commands.append("echo "+ RSET)
		tempname = "crab3_TEMP"+RSET.split("/")[0]+RSET.split("/")[1]+"_v"+VER+".py"
		commands.append("sed 's/RSET/"+RSET+"/g' crab_debugstring_DATA2017.TMP > "+tempname)
		commands.append("sed -i 's/LOC/"+LOC+"/g' "+tempname)
		commands.append("sed -i 's/VER/"+VER+"/g' "+tempname)
		commands.append("sed -i 's/STO/"+STO+"/g' "+tempname)
		if options.submit:
			commands.append("crab submit "+tempname)

commands.append("rm tempcrabfiles/*")
commands.append("mv crab3_TEMP*.py tempcrabfiles/")

for s in commands :
    print 'executing ' + s
    subprocess.call( [s], shell=True )



