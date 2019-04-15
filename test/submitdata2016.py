import re
import os
import subprocess
from os import listdir
from os.path import isfile, join
import glob
import copy
import math
import sys


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

for iset in tosubmitDATA:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_DATA2016.TMP > "+tempname)
        commands.append("crab submit "+tempname)



for s in commands :
    print 'executing ' + s
    subprocess.call( [s], shell=True )



