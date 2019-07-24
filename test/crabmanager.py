import copy, os, sys, time, logging, re, subprocess, glob, math
from os import listdir
from os.path import isfile, join

from optparse import OptionParser
parser = OptionParser()

parser.add_option('-m', '--mode', metavar='F', type='string', action='store',
                  default	=	'NONE',
                  dest		=	'mode',
                  help		=	'mode kll, status, resubmit, recover, lumicalc')

parser.add_option('-s', '--search', metavar='F', type='string', action='store',
                  default	=	'NONE',
                  dest		=	'search',
                  help		=	'search string (match crab directory)')

(options, args) = parser.parse_args()

print('Options summary')
print('==================')
for opt,value in options.__dict__.items():
    print(str(opt) +': '+ str(value))
print('==================')


if options.mode=="NONE":
	logging.error("Please enter a valid mode")
	sys.exit()
if options.search=="NONE":
	logging.error("Please enter a valid search string")
	sys.exit()

commands = []

folders = glob.glob(options.search)
for fold in folders:
	print fold
	if options.mode in ("status","kill","resubmit"):
		commands.append("crab "+options.mode+" "+fold)
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

		with open(curcrab) as fp:
   			for line in fp:

				line=line.replace("NanoSlimNtuples","NanoSlimNtuples_recov")
       				lsplit = line.split("=")
				if len(lsplit)==2:
					lsplit[0]=lsplit[0].replace(" ","")
					if lsplit[0]=="config.Data.unitsPerJob":
						lsplit[1]=str(int(0.5*int(lsplit[1])))+"\n"
					if lsplit[0]=="config.Data.lumiMask":
						lsplit[1]='\"'+NFL+'\"'+"\n"
					if lsplit[0]=="config.Data.lumiMask":
						lsplit[1]='\"'+NFL+'\"'+"\n"
					fil.write(lsplit[0]+"="+lsplit[1])
				else:
					fil.write(line)
		#commands.append("crab submit "+crabpyname)
#for s in commands :
    #print 'executing ' + s
    #subprocess.call( [s], shell=True )

