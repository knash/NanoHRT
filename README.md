# NanoHRT

### Set up CMSSW

```bash
cmsrel CMSSW_10_2_15
cd CMSSW_10_2_15/src
cmsenv
```

### Get customized NanoAOD producers for HeavyResTagging

```bash
git clone https://github.com/knash/NanoHRT.git PhysicsTools/NanoHRT
cd PhysicsTools/NanoHRT
git fetch
git checkout Forskimming
```

### Compile

```bash
scram b -j 12
```

### Test

```bash
cd test/
```
then
```bash
cmsRun NanoAOD_Slim_data2017_NANO10_2_9.py
```
or 
```bash
cmsRun NanoAOD_Slim_mc2017_NANO10_2_9.py
```

A series of Ntuples can be submitted to crab with submitall.py

```bash
python submitall.py -s sig,ttbar -l /store/user/NAME -v 8 -d T2_CH_CERN -y 2016,2017
```
Where the -s option refers to the various profiles sig,ttbar,data,qcd and is csv parsed.
These and the year correspond to the crab_20XX_YYY.TMP and sets_20XX_YYY.txt files which should be modified as desired.
Whe -l and -d option will need to be customized for each user and location.

This will create files for submission in tempcrabfiles.
Check that these look ok and then add--submit to submit all of the datasets automatically

The ntuples are based on NanoAODv5 and 1June2019 NanoAOD versions

