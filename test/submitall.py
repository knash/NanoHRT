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
"WkkToWRadionToWWW_M5000-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M4500-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M4000-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M3500-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M3000-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M2500-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M2000-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"WkkToWRadionToWWW_M1500-R0-5-TuneCUEP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"
]




tosubmitQCD = [
"QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM",
"QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"
]






commands = []


for iset in tosubmit:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring.TMP > crab3_TEMP.py")
	commands.append("crab submit crab3_TEMP.py")


for iset in tosubmitQCD:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_QCD.TMP > crab3_TEMP.py")
	commands.append("crab submit crab3_TEMP.py")


for s in commands :
    print 'executing ' + s
    subprocess.call( [s], shell=True )



