#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include  "DataFormats/FWLite/interface/ChainEvent.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "TLorentzVector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

#include "DataFormats/FWLite/interface/Event.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"

#include <fstream>

class WtagProducer1 : public edm::stream::EDProducer<> {

  public:
    explicit WtagProducer1(const edm::ParameterSet&);
    ~WtagProducer1() override;
    static void fillDescriptions(edm::ConfigurationDescriptions&);
    static void globalEndJob();


  private:
    void beginStream(edm::StreamID) override {}
    void produce(edm::Event&, const edm::EventSetup&) override;
    void endStream() override {}

    const edm::EDGetTokenT<std::vector<std::pair<int,int>>> links_;
    const edm::EDGetTokenT<reco::VertexCompositePtrCandidateCollection> svs_;
    const edm::EDGetTokenT<std::vector<pat::PackedCandidate>> pfcs_;
    const edm::EDGetTokenT<std::vector<pat::Muon>> mus_;

};


WtagProducer1::WtagProducer1(const edm::ParameterSet& iConfig)
: 
links_(consumes<std::vector<std::pair<int,int>>>(iConfig.getParameter<edm::InputTag>("links")))
, svs_(consumes<reco::VertexCompositePtrCandidateCollection>(iConfig.getParameter<edm::InputTag>("sec")))
, pfcs_(consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("pfcs")))
, mus_(consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("mus")))
{
  produces<reco::VertexCompositePtrCandidateCollection>("linkedsvs");
}
WtagProducer1::~WtagProducer1(){
}

void WtagProducer1::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void WtagProducer1::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //std::cout<<"1"<<std::endl;
  edm::Handle<std::vector<std::pair<int,int>>> links;
  iEvent.getByToken(links_, links);

  edm::Handle<reco::VertexCompositePtrCandidateCollection> svs;
  iEvent.getByToken(svs_, svs);

  edm::Handle<std::vector<pat::PackedCandidate>> pfcs;
  iEvent.getByToken(pfcs_, pfcs);
  std::vector<std::pair<int,int>> plinks = *(links.product());
  std::vector<pat::PackedCandidate> ppfcs = *(pfcs.product());
  reco::VertexCompositePtrCandidateCollection linkedsvs = *(svs.product());



  for (auto pl : plinks)
	{
 	//std::cout<<"addau"<<std::endl;
	linkedsvs[pl.first].addDaughter(edm::Ptr<pat::PackedCandidate>(pfcs,pl.second));
	}
  //for (auto pf : ppfcs)
	//{
	//if (pf.hasTrackDetails()) std::cout<<"HTD "<<(pf.bestTrack())->referencePoint().x()<<std::endl;
	//}

  auto outputslinkedsvs = std::make_unique<reco::VertexCompositePtrCandidateCollection>(linkedsvs);
  iEvent.put(std::move(outputslinkedsvs),"linkedsvs");

  //edm::EDGetTokenT<std::vector<pat::Muon>> mus_;
  //mus_ = consumes<std::vector<pat::Muon>>( edm::InputTag("WBProducerpuppi","newmus"));
  edm::Handle<std::vector<pat::Muon>> allmusff;
 
  iEvent.getByToken(mus_, allmusff);
  for(auto curmu : *(allmusff.product()))
	{
		//std::cout<<"mu 1 "<<curmu.eta()<<std::endl;

		//std::cout<<"tryglob"<<std::endl;
		//reco::Track newtrack = trotate(track, dx, dy, dz, dthetaj, dphij,newopv[0]);

		//if (curmu.globalTrack().isNonnull())std::cout<<"globs "<<curmu.globalTrack().get()->eta()<<std::endl;
		//else if (curmu.innerTrack().isNonnull())std::cout<<"ins "<<curmu.innerTrack().get()->eta()<<std::endl;
		//else if (curmu.outerTrack().isNonnull())std::cout<<"outs "<<curmu.outerTrack().get()->eta()<<std::endl;
	}





}

//define this as a plug-in
DEFINE_FWK_MODULE(WtagProducer1);
