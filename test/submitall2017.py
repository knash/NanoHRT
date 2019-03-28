import re
import os
import subprocess
from os import listdir
from os.path import isfile, join
import glob
import copy
import math
import sys


tosubmit = [
"BstarToTW_private_M-1200_LH_TuneCP5_13TeV-madgraph-pythia8/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
"BstarToTW_private_M-2000_LH_TuneCP5_13TeV-madgraph-pythia8/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
"BstarToTW_private_M-2800_LH_TuneCP5_13TeV-madgraph-pythia8/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
"WkkToRWToTri_Wkk3000R100_ZA_private_TuneCP5_13TeV-madgraph-pythia8_v2/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
"WkkToRWToTri_Wkk3000R200_ZA_private_TuneCP5_13TeV-madgraph-pythia8_v2/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER"
]


tosubmitWW = [
"WkkToWRadionToWWW_M3000-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WkkToWRadionToWWW_M3500-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WkkToWRadionToWWW_M4000-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3/MINIAODSIM",
"WkkToWRadionToWWW_M4500-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
"WkkToWRadionToWWW_M5000-R0-06_TuneCP5_13TeV-madgraph/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM"
]



tosubmitQCD = [
#"QCD_HT1000to1500_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM",
#"QCD_HT1500to2000_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
#"QCD_HT2000toInf_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
#"WJetsToQQ_HT400to600_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
#"WJetsToQQ_HT600to800_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"WJetsToQQ_HT-800toInf_qc19_3j_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM ",
#"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
#"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM",
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


for iset in tosubmit:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring.TMP > "+tempname)
	#commands.append("crab submit "+tempname)

for iset in tosubmitWW:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_WW.TMP > "+tempname)
	#commands.append("crab submit "+tempname)


for iset in tosubmitQCD:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_QCD.TMP > "+tempname)
        commands.append("crab submit "+tempname)


for iset in tosubmitTT:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_TT.TMP > "+tempname)
        #commands.append("crab submit "+tempname)



for iset in tosubmitDATA:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_DATA2017.TMP > "+tempname)
        #commands.append("crab submit "+tempname)



for s in commands :
    print 'executing ' + s
    subprocess.call( [s], shell=True )



