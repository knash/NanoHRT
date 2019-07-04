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
parser.add_option('-y', '--year', metavar='F', type='string', action='store',
                  default	=	'NONE',
                  dest		=	'year',
                  help		=	'2016,2017,2018')
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
LOC = (options.location).replace("/","\/")
VER = (options.version).replace("/","\/")
STO = (options.site).replace("/","\/")

tosubmit = {}
for year in years:
	for cset in sets: 
		idstr=year+"_"+cset
		tosubmit[idstr]=[]
		f = open("txtfiles/sets_"+idstr+".txt", "r")
		for x in f:
		 	tosubmit[idstr].append(x)
commands = []
for idstr in tosubmit:
	for RSET in tosubmit[idstr]:
		RSET = RSET.replace("/","\/").replace("\n","")
		commands.append("echo "+ RSET)
		tempname = "crab3_TEMP"+RSET.split("\/")[1]+RSET.split("\/")[2]+"_v"+VER+".py"
		commands.append("sed 's/RSET/"+RSET+"/g' tmpfiles/crab_"+idstr+".TMP > "+tempname)
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



