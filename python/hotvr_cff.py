import FWCore.ParameterSet.Config as cms
from  PhysicsTools.NanoAOD.common_cff import *

# ---------------------------------------------------------


def setupHOTVR(process, runOnMC=False, path=None, Settype=''):

    # HOTVR
    process.hotvrPuppi = cms.EDProducer('HOTVRProducer',
        src=cms.InputTag("puppi"),
        doRekey=cms.bool(True),
	rekeyCandidateSrc= cms.InputTag('packedPFCandidates'),
    )
   
    from  PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import patJetCorrFactors
    process.jetCorrFactorsHOTVRSubjets = patJetCorrFactors.clone(
        src=cms.InputTag('hotvrPuppi', 'RecoSubJets'),
        levels=cms.vstring('L2Relative', 'L3Absolute', 'L2L3Residual'),
        payload=cms.string('AK4PFPuppi'),
        primaryVertices=cms.InputTag("offlineSlimmedPrimaryVertices"),
    )
    from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import _patJets
    process.updatedHOTVRSubjets = _patJets.clone(
        jetSource=cms.InputTag('hotvrPuppi', 'RecoSubJets'),
        jetCorrFactorsSource=cms.VInputTag(cms.InputTag("jetCorrFactorsHOTVRSubjets")),
        addBTagInfo=cms.bool(False),
        addAssociatedTracks=cms.bool(False),
        addJetCharge=cms.bool(False),
        addGenPartonMatch=cms.bool(False),
        embedGenPartonMatch=cms.bool(False),
	embedPFCandidates=cms.bool(True),
        addGenJetMatch=cms.bool(False),
        embedGenJetMatch=cms.bool(False),
        getJetMCFlavour=cms.bool(False),
        addJetFlavourInfo=cms.bool(False),
    )

 
    from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
    Bdiscs = ['pfDeepFlavourJetTags:probb', 'pfDeepFlavourJetTags:probbb', 'pfDeepFlavourJetTags:probuds', 'pfDeepFlavourJetTags:probg' , 'pfDeepFlavourJetTags:problepb', 'pfDeepFlavourJetTags:probc','pfCombinedInclusiveSecondaryVertexV2BJetTags']
    updateJetCollection(
     process,
     labelName = 'UpdatebtagHotVRPuppiSubjets',
     jetSource = cms.InputTag('updatedHOTVRSubjets'),
     jetCorrections = ('AK4PFPuppi', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual']), 'None'),
     pvSource = cms.InputTag("offlineSlimmedPrimaryVertices"),
     svSource = cms.InputTag('slimmedSecondaryVertices'),
     muSource = cms.InputTag('slimmedMuons'),
     elSource = cms.InputTag('slimmedElectrons'),
     explicitJTA = True,         
     svClustering = False,  
     rParam = 1.2,
     #fatJets = cms.InputTag('hotvrPuppi'), 
     algo = 'ak',
     btagDiscriminators = Bdiscs
     )


    process.hotvrPuppiUG = cms.EDProducer('HOTVRProducer',
        src=cms.InputTag("puppi"),
        doRekey=cms.bool(True),
        unGroom=cms.bool(True),
	rekeyCandidateSrc= cms.InputTag('packedPFCandidates'),
    )



    process.imageJetsHotVR = cms.EDProducer('ImageProducer',
        src=cms.InputTag("hotvrPuppiUG"),
        sj=cms.InputTag('selectedUpdatedPatJetsUpdatebtagHotVRPuppiSubjets'),
        sdmcoll=cms.string('NONE'),
        pb_path=cms.untracked.FileInPath('PhysicsTools/NanoHRT/data/Image/NNtraining_preliminary_01232018.pb'),
        pb_pathMD=cms.untracked.FileInPath('PhysicsTools/NanoHRT/data/Image/NNtraining_preliminary_MD_01232018.pb'),
	stype=cms.string(Settype),
        extex=cms.string(''),
        isHotVR=cms.bool(True),
        drfac=cms.double(1.0)
    )
    

    process.finalHOTVR = cms.EDProducer('HOTVRUpdater',
        src=cms.InputTag("hotvrPuppi"),
        subjets=cms.InputTag('updatedHOTVRSubjets'),
    )








    process.hotvrTable = cms.EDProducer("SimpleCandidateFlatTableProducer",
        src=cms.InputTag("imageJetsHotVR"),
        name=cms.string("HOTVRPuppi"),
        cut=cms.string(""),
        doc=cms.string("HOTVR Puppi jets"),
        singleton=cms.bool(False),  # the number of entries is variable
        extension=cms.bool(False),  # this is the main table for the jets
        variables=cms.PSet(P4Vars,
            itop=Var("userFloat('ImageHotVR:top')", float, doc="Image tagger score top", precision=-1),
            iMDtop=Var("userFloat('ImageMDHotVR:top')", float, doc="Image tagger score top", precision=-1),
        )
    )
    process.hotvrTable.variables.pt.precision = 10

    process.hotvrSubJetTable = cms.EDProducer("SimpleCandidateFlatTableProducer",
        src=cms.InputTag('updatedHOTVRSubjets'),
        cut=cms.string(""),
        name=cms.string("HOTVRPuppiSubJet"),
        doc=cms.string("HOTVR Puppi subjets"),
        singleton=cms.bool(False),  # the number of entries is variable
        extension=cms.bool(False),  # this is the main table for the jets
        variables=cms.PSet(P4Vars,
            #rawFactor=Var("1.-jecFactor('Uncorrected')", float, doc="1 - Factor to get back to raw pT", precision=6),
            area=Var("jetArea()", float, doc="jet catchment area, for JECs", precision=10),
        )
    )
    process.hotvrSubJetTable.variables.pt.precision = 10

    process.hotvrTask = cms.Task(
        process.hotvrPuppi,
	#process.updatedPatJetUpdatebtagHotVRPuppiSubjets,
        process.updatedHOTVRSubjets,
        process.jetCorrFactorsHOTVRSubjets,
        process.hotvrSubJetTable,
	process.hotvrPuppiUG,
	process.imageJetsHotVR,
        #process.finalHOTVR,
        process.hotvrTable,

        )

    if path is None:
        process.schedule.associate(process.hotvrTask)
    else:
        getattr(process, path).associate(process.hotvrTask)

# ---------------------------------------------------------
