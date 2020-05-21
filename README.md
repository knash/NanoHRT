# NanoHRT

### Set up CMSSW

## 2017 version
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
git checkout origin/ForTraining --track
```

### Compile

```bash
scram b -j 16
```


MC:
```bash
cd test/
cmsRun test_nanoHRT_mc_102X_NANO.py Settype=YYY
```

where YYY is the signal type - top, pho, ww, etc
