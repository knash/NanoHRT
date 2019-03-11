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
"JetHT/Run2017F-31Mar2018-v1/MINIAOD",
"JetHT/Run2017E-31Mar2018-v1/MINIAOD",
"JetHT/Run2017D-31Mar2018-v1/MINIAOD",
"JetHT/Run2017C-31Mar2018-v1/MINIAOD",
"JetHT/Run2017B-31Mar2018-v1/MINIAOD"
]


commands = []

for iset in tosubmitDATA:
	iset = iset.replace("/","\/")
	print iset
	commands.append("echo "+ iset)
	tempname = "crab3_TEMP"+iset.split("/")[0]+".py"
	commands.append("sed 's/RSET/"+iset+"/g' crab_debugstring_DATA.TMP > "+tempname)
        #commands.append("crab submit "+tempname)



for s in commands :
    print 'executing ' + s
    subprocess.call( [s], shell=True )



