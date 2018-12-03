import FWCore.ParameterSet.Config as cms
from  PhysicsTools.NanoAOD.common_cff import *
from Configuration.Eras.Modifier_run2_miniAOD_80XLegacy_cff import run2_miniAOD_80XLegacy

# ---------------------------------------------------------


def setupCustomizedAK8(process, runOnMC=False, path=None):
    # recluster Puppi jets, add N-Subjettiness and ECF
    bTagDiscriminators = [
        'pfCombinedInclusiveSecondaryVertexV2BJetTags',
        'pfBoostedDoubleSecondaryVertexAK8BJetTags',
    ]
    subjetBTagDiscriminators = [
        'pfCombinedInclusiveSecondaryVertexV2BJetTags',
        'pfDeepCSVJetTags:probb',
        'pfDeepCSVJetTags:probbb',
    ]
    JETCorrLevels = ['L2Relative', 'L3Absolute', 'L2L3Residual']

    from PhysicsTools.NanoHRT.jetToolbox_cff import jetToolbox
    jetToolbox(process, 'ak8', 'dummySeq', 'out', associateTask=False,
               PUMethod='Puppi', JETCorrPayload='AK8PFPuppi', JETCorrLevels=JETCorrLevels,
               Cut='pt > 170.0 && abs(rapidity()) < 2.4',
               miniAOD=True, runOnMC=runOnMC,
               addNsub=True, maxTau=4, addEnergyCorrFunc=True,
               addSoftDrop=True, addSoftDropSubjets=True, subJETCorrPayload='AK4PFPuppi', subJETCorrLevels=JETCorrLevels,
               bTagDiscriminators=bTagDiscriminators, subjetBTagDiscriminators=subjetBTagDiscriminators)


    from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

    Bdiscs = ['pfDeepFlavourJetTags:probb', 'pfDeepFlavourJetTags:probbb', 'pfDeepFlavourJetTags:probuds', 'pfDeepFlavourJetTags:probg' , 'pfDeepFlavourJetTags:problepb', 'pfDeepFlavourJetTags:probc','pfCombinedInclusiveSecondaryVertexV2BJetTags']
    updateJetCollection(
     process,
     labelName = 'UpdatebtagAK8PFPuppiSoftDropSubjets',
     jetSource = cms.InputTag('selectedPatJetsAK8PFPuppiSoftDropSubjets'),
     jetCorrections = ('AK4PFchs', cms.vstring([]), 'None'),
     pvSource = cms.InputTag("offlineSlimmedPrimaryVertices"),
     svSource = cms.InputTag('slimmedSecondaryVertices'),
     muSource = cms.InputTag('slimmedMuons'),
     elSource = cms.InputTag('slimmedElectrons'),
     btagDiscriminators = Bdiscs 
     )


    process.imageJetsAK8Puppi = cms.EDProducer('ImageProducer',
        src=cms.InputTag('slimmedJetsAK8'),
        sj=cms.InputTag('selectedUpdatedPatJetsUpdatebtagAK8PFPuppiSoftDropSubjets'),
        pb_path=cms.untracked.FileInPath('PhysicsTools/NanoHRT/data/Image/NNtraining_preliminary_10232018.pb'),
    )



    process.imageJetsAK8Puppi1 = cms.EDProducer('ImageProducer',
        src=cms.InputTag('packedPatJetsAK8PFPuppiSoftDrop'),
        sj=cms.InputTag('selectedUpdatedPatJetsUpdatebtagAK8PFPuppiSoftDropSubjets'),
        pb_path=cms.untracked.FileInPath('PhysicsTools/NanoHRT/data/Image/NNtraining_preliminary_10232018.pb'),
    )


    #process.dump=cms.EDAnalyzer('EventContentAnalyzer')
    #process.p = cms.Path( process.dump)


    # src
    srcJets = cms.InputTag('imageJetsAK8Puppi')

    # jetID
    process.looseJetIdCustomAK8 = cms.EDProducer("PatJetIDValueMapProducer",
              filterParams=cms.PSet(
                version = cms.string('WINTER16'),
                quality = cms.string('LOOSE'),
              ),
              src=srcJets
    )

    process.tightJetIdCustomAK8 = cms.EDProducer("PatJetIDValueMapProducer",
              filterParams=cms.PSet(
                version=cms.string('WINTER17'),
                quality = cms.string('TIGHT'),
              ),
              src=srcJets
    )
    run2_miniAOD_80XLegacy.toModify(process.tightJetIdCustomAK8.filterParams, version="WINTER16")

    process.tightJetIdLepVetoCustomAK8 = cms.EDProducer("PatJetIDValueMapProducer",
              filterParams=cms.PSet(
                version = cms.string('WINTER17'),
                quality = cms.string('TIGHTLEPVETO'),
              ),
              src=srcJets
    )

    process.customAK8WithUserData = cms.EDProducer("PATJetUserDataEmbedder",
        src=srcJets,
        userFloats=cms.PSet(),
        userInts=cms.PSet(
           tightId=cms.InputTag("tightJetIdCustomAK8"),
           tightIdLepVeto=cms.InputTag("tightJetIdLepVetoCustomAK8"),
        ),
    )
    run2_miniAOD_80XLegacy.toModify(process.customAK8WithUserData.userInts,
        looseId=cms.InputTag("looseJetIdCustomAK8"),
        tightIdLepVeto=None,
    )

    process.customAK8Table = cms.EDProducer("SimpleCandidateFlatTableProducer",
        src=cms.InputTag("customAK8WithUserData"),
        name=cms.string("CustomAK8Puppi"),
        cut=cms.string(""),
        doc=cms.string("customized ak8 puppi jets for HRT"),
        singleton=cms.bool(False),  # the number of entries is variable
        extension=cms.bool(False),  # this is the main table for the jets
        variables=cms.PSet(P4Vars,
            itop=Var("userFloat('Image:top')", float, doc="Image tagger score top", precision=10),
        )
    )



    process.customAK8Table1 = cms.EDProducer("SimpleCandidateFlatTableProducer",
        src=cms.InputTag('imageJetsAK8Puppi1'),
        name=cms.string("CustomAK8Puppi1"),
        cut=cms.string(""),
        doc=cms.string("customized ak8 puppi jets for HRT"),
        singleton=cms.bool(False),  # the number of entries is variable
        extension=cms.bool(False),  # this is the main table for the jets
        variables=cms.PSet(P4Vars,
            itop=Var("userFloat('Image:top')", float, doc="Image tagger score top", precision=10),
        )
    )



    run2_miniAOD_80XLegacy.toModify(process.customAK8Table.variables, jetId=Var("userInt('tightId')*2+userInt('looseId')", int, doc="Jet ID flags bit1 is loose, bit2 is tight"))
    process.customAK8Table.variables.pt.precision = 10

    process.customAK8SubJetTable = cms.EDProducer("SimpleCandidateFlatTableProducer",
        src=cms.InputTag("selectedPatJetsAK8PFPuppiSoftDropPacked", "SubJets"),
        cut=cms.string(""),
        name=cms.string("CustomAK8PuppiSubJet"),
        doc=cms.string("customized ak8 puppi subjets for HRT"),
        singleton=cms.bool(False),  # the number of entries is variable
        extension=cms.bool(False),  # this is the main table for the jets
        variables=cms.PSet(P4Vars,
            btagDeepB=Var("bDiscriminator('pfDeepCSVJetTags:probb')+bDiscriminator('pfDeepCSVJetTags:probbb')", float, doc="DeepCSV b+bb tag discriminator", precision=10),
            btagCSVV2=Var("bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')", float, doc=" pfCombinedInclusiveSecondaryVertexV2 b-tag discriminator (aka CSVV2)", precision=10),
            rawFactor=Var("1.-jecFactor('Uncorrected')", float, doc="1 - Factor to get back to raw pT", precision=6),
            area=Var("jetArea()", float, doc="jet catchment area, for JECs", precision=10),
        )
    )
    process.customAK8SubJetTable.variables.pt.precision = 10

    process.customizedAK8Task = cms.Task(
        process.imageJetsAK8Puppi,
        process.imageJetsAK8Puppi1,
        process.tightJetIdCustomAK8,
        process.tightJetIdLepVetoCustomAK8,
        process.customAK8WithUserData,
        process.customAK8Table,
        process.customAK8Table1,
        process.customAK8SubJetTable
        )

    _customizedAK8Task_80X = process.customizedAK8Task.copy()
    _customizedAK8Task_80X.replace(process.tightJetIdLepVetoCustomAK8, process.looseJetIdCustomAK8)
    run2_miniAOD_80XLegacy.toReplaceWith(process.customizedAK8Task, _customizedAK8Task_80X)

    if path is None:
        process.schedule.associate(process.customizedAK8Task)
    else:
        getattr(process, path).associate(process.customizedAK8Task)

