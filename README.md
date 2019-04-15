# NanoHRT

### Set up CMSSW

```bash
cmsrel CMSSW_10_2_9
cd CMSSW_10_2_9/src
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
cd test
```

