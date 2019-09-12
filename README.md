# NanoHRT

### Set up CMSSW

```bash
cmsrel CMSSW_10_2_15
cd CMSSW_10_2_15/src
cmsenv
```

### Customize Standard NanoAOD workflow
Here, in theory we should grab the latest/greatest outside of NanoAOD releases.
For now, 2018 JetID PR is still in progres -- merge with current version so maybe we dont have to reprocess.
```bash
git-cms-addpkg PhysicsTools/NanoAOD
git-cms-addpkg PhysicsTools/SelectorUtils
git remote add tmp https://github.com/knash/cmssw.git
git fetch tmp
git checkout tmp/JetID2018_backport_102X

```
### Get customized NanoAOD producers for HeavyResTagging
This gives us prototype (or otherwise awaiting PR) taggers
```bash
git clone https://github.com/knash/NanoHRT.git PhysicsTools/NanoHRT
cd PhysicsTools/NanoHRT
git fetch
git checkout Forskimming
```

### Compile

```bash
cd ../../
scram b -j 12
```

### Test

```bash
cd PhysicsTools/NanoHRT/test/
```
then
```bash
cmsRun NanoAOD_Slim_data2017_NANO10_2_9.py
```
or 
```bash
cmsRun NanoAOD_Slim_mc2017_NANO10_2_9.py
```

A series of Ntuples can be submitted to crab with crabmanager.py (for example)

```bash
python crabmanager.py -s sig,ttbar -l /store/user/NAME -v 8 -d T2_CH_CERN -y 2016,2017
```
Where the -s option refers to the various profiles sig,ttbar,data,qcd and is csv parsed.
These and the year correspond to the crab_20XX_YYY.TMP and sets_20XX_YYY.txt files which should be modified as desired.
Whe -l and -d option will need to be customized for each user and location.

This will create files for submission in tempcrabfiles.
Check that these look ok and then add--submit to submit all of the datasets automatically

for other crab commands, you can use this script for example

```bash
python crabmanager.py -S crab_projects/\*v8 -o "--maxmemory 9000" -m status
```

will analyze the status output of all files matching the search string in the -S command.  Jobs in failed status are resubmitted using the options in the -o command.
non-status commands will simply run the command and options.

The ntuples are based on NanoAODv5 and 1June2019 NanoAOD versions

