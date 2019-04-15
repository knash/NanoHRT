import re
import os
import subprocess
from os import listdir
from os.path import isfile, join
import glob
import copy
import math
import sys


#tosubmit = [
#"BstarToTW_private_M-1200_LH_TuneCP5_13TeV-madgraph-pythia8/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
#"BstarToTW_private_M-2000_LH_TuneCP5_13TeV-madgraph-pythia8/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
#"BstarToTW_private_M-2800_LH_TuneCP5_13TeV-madgraph-pythia8/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
#"WkkToRWToTri_Wkk3000R100_ZA_private_TuneCP5_13TeV-madgraph-pythia8_v2/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER",
#"WkkToRWToTri_Wkk3000R200_ZA_private_TuneCP5_13TeV-madgraph-pythia8_v2/knash-MINIAODSIM-57e6cb033643cfa6c372ff41c8f6b812/USER"
#]


tosubmitWW = [
"WkkToWRadionToWWW_M5000-R0-06-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M4500-R0-06-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M4000-R0-06-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M3500-R0-06-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M3000-R0-06-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
]



tosubmitQCD = [
"QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
"QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
"QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2/MINIAODSIM",
"WJetsToQQ_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
"GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
"GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"
]

tosubmitTT = [
"TT_Mtt-1000toInf_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM",
"TT_Mtt-700to1000_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM"

]

tosubmitDATA = [
"JetHT/Run2016B-17Jul2018_ver2-v2/MINIAOD",
"JetHT/Run2016C-17Jul2018-v1/MINIAOD",
"JetHT/Run2016D-17Jul2018-v1/MINIAOD",
"JetHT/Run2016E-17Jul2018-v1/MINIAOD",
"JetHT/Run2016F-17Jul2018-v1/MINIAOD",
"JetHT/Run2016G-17Jul2018-v1/MINIAOD",
"JetHT/Run2016H-17Jul2018-v1/MINIAOD",
]


commands = []


#for iset in tosubmit:
#	iset = iset.replace("/","\/")
#	print iset
#	commands.append("echo "+ iset)
#	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
#	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring.TMP > "+tempname)
#	commands.append("crab submit "+tempname)

for iset in tosubmitWW:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_WW2016.TMP > "+tempname)
#	commands.append("crab submit "+tempname)


for iset in tosubmitQCD:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_QCD2016.TMP > "+tempname)
#        commands.append("crab submit "+tempname)


for iset in tosubmitTT:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_TT2016.TMP > "+tempname)
        #commands.append("crab submit "+tempname)



for iset in tosubmitDATA:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_DATA2016.TMP > "+tempname)
#        commands.append("crab submit "+tempname)



for s in commands :
    print 'executing ' + s
    subprocess.call( [s], shell=True )



