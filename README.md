# NanoHRT

### Set up CMSSW

//## 2016 version
//```bash
//cmsrel CMSSW_9_4_12
//cd CMSSW_9_4_12/src
//cmsenv
//```
## 2017 version
```bash
cmsrel CMSSW_10_2_15
cd CMSSW_10_2_15/src
cmsenv
```

### Get customized NanoAOD producers for HeavyResTagging

```bash
git clone https://github.com/knash/NanoHRT.git PhysicsTools/NanoHRT
```

### Compile

```bash
scram b -j 16
```


MC:
//## 2016 version
//```bash
//cmsRun test_nanoHRT_mc_NANO.py Settype=YYY
//```
//## 2017 version
```bash
cmsRun test_nanoHRT_mc_102X_NANO.py Settype=YYY
```

where YYY is the signal type - top, pho, ww, etc
