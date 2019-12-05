#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include  "DataFormats/FWLite/interface/ChainEvent.h"

#include "DataFormats/Math/interface/normalizedPhi.h"
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
#include "../interface/WtagProducer.hh"
#include <fstream>

class WtagProducer : public edm::stream::EDProducer<> {

  public:
    explicit WtagProducer(const edm::ParameterSet&);
    ~WtagProducer() override;
    static void fillDescriptions(edm::ConfigurationDescriptions&);
    static void globalEndJob();


  private:
    //math::XYZPoint protate(math::XYZPoint,reco::Vertex,float,float);
    //reco::Track trotate(reco::Track ,float ,float ,float ,float ,float,reco::Vertex );
    void beginStream(edm::StreamID) override {}
    void produce(edm::Event&, const edm::EventSetup&) override;
    void endStream() override {}
    int nevmix;
    const edm::EDGetTokenT<edm::View<pat::Jet>> src_;
    const edm::EDGetTokenT<edm::View<pat::Jet>> srcAK4_;
    const edm::EDGetTokenT<edm::View<pat::Jet>> sj_;
    const edm::EDGetTokenT<reco::VertexCompositePtrCandidateCollection> svs_;
    const edm::EDGetTokenT<std::vector<pat::Muon>> mu_;
    const edm::EDGetTokenT<std::vector<pat::Electron>> el_;

    bool merge_;
    bool mergeb_;
    bool mergemj_;
    bool load_;

    edm::EDGetTokenT<int> windex_;
    edm::EDGetTokenT<int> tindex_;
    edm::EDGetTokenT<int> bindex_;


    edm::EDGetTokenT<std::vector<reco::Vertex>> opv_;
    uint pnum_;
    uint tnum_;
    const edm::EDGetTokenT<edm::View<pat::PackedCandidate>> pfcs_;
     
    //TFile* inputTFile_;
    //fwlite::Event* ev_;
    fwlite::ChainEvent* ev_;
    std::atomic<TH1F*> topmass_;
    std::atomic<TH1F*> wwmass_;
    edm::EDGetTokenT<std::vector<reco::GenParticle>> gplab ;
    int nvals;

};


WtagProducer::WtagProducer(const edm::ParameterSet& iConfig)
: 
src_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("src")))
, srcAK4_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcAK4")))
, sj_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("sj")))
, svs_(consumes<reco::VertexCompositePtrCandidateCollection>(iConfig.getParameter<edm::InputTag>("sec")))
, mu_(consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("mu")))
, el_(consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("el")))
, merge_(iConfig.getParameter<bool>("merge"))
, mergeb_(iConfig.getParameter<bool>("mergeb"))
, mergemj_(iConfig.getParameter<bool>("mergemj"))
, load_(iConfig.getParameter<bool>("load"))
, pnum_(iConfig.getParameter<uint>("pnum"))
, tnum_(iConfig.getParameter<uint>("tnum"))
, pfcs_(consumes<edm::View<pat::PackedCandidate>>(edm::InputTag("packedPFCandidates")))
{
  nvals=18;

  if(merge_ or mergeb_ or mergemj_)
	{
  	produces<std::vector<pat::PackedCandidate>>("");
  	produces<std::vector<pat::Jet>>("jet");
  	produces<std::vector<int>>("wmatch");
  	produces<std::vector<float>>("wmatchdr");
  	produces<std::vector<float>>("sjmerger");
  	produces<std::vector<reco::Track>>("newtracks");
	produces<std::vector<reco::Vertex>>("newopvs");
  	produces<std::vector<pat::Electron>>("newels");
  	produces<std::vector<reco::GsfTrack>>("gsftracks");
  	produces<std::vector<reco::GsfElectronCore>>("gsfcores");
  	produces<std::vector<pat::Muon>>("newmus");
  	produces<reco::VertexCompositePtrCandidateCollection>("newvecs");
  	produces<std::vector<std::pair<int,int>>>("secvertrekey");
	for(int ival=0;ival<nvals;ival++)produces<float>("matchvals"+std::to_string(ival));
	}
  else
	{
  	produces<std::vector<pat::PackedCandidate>>("wcands");
  	produces<std::vector<pat::PackedCandidate>>("bcands");
  	produces<std::vector<pat::Jet>>("wjet");
  	produces<std::vector<pat::Jet>>("bjet");
  	produces<std::vector<pat::Muon>>("allmusb");
  	produces<std::vector<pat::Electron>>("allelsb");
  	produces<std::vector<pat::Muon>>("allmus");
  	produces<std::vector<pat::Electron>>("allels");
  	produces<std::vector<int>>("wmatch");
  	produces<std::vector<float>>("wmatchdr");
  	produces<std::vector<bool>>("ismerge");
  	produces<std::vector<float>>("sjw");
  	produces<std::vector<float>>("sjb");
  	produces<std::vector<reco::Vertex>>("newopv");
  	produces<reco::VertexCompositePtrCandidateCollection>("wvecs");
  	produces<reco::VertexCompositePtrCandidateCollection>("bvecs");
	}

  if(merge_ or mergeb_)
	{

	//inputTFile_  = new TFile("TEMPMIX.root","read");
	auto topf  = new TFile("topmass.root ","read"); 
	auto wwf  = new TFile("wwmass.root ","read"); 
    	topmass_ = (TH1F*)topf->Get("Masshisttt");
    	wwmass_ = (TH1F*)wwf->Get("Masshisttt");
	//ev_ = new fwlite::Event(inputTFile_);
	std::vector<std::string> fnames;
	std::string line;
	std::ifstream myfile;
	//std::cout<<"file"<<std::endl;
	myfile.open("fnames.txt");
	while(getline(myfile, line))fnames.push_back(line);
	ev_ =  new fwlite::ChainEvent(fnames);
	nevmix=ev_->size();
	ev_->toBegin();
	//nevmix=100;
	//std::cout<<"mixing "<<nevmix<<" events";
	}
  //windex_ = consumes<edm::View<int>>(edm::InputTag("windex","NanoAODFilterSlep"));
  //bindex_ = consumes<edm::View<int>>(edm::InputTag("bindex","NanoAODFilterSlep"));
  windex_ = consumes<int>(edm::InputTag("NanoAODFilterSlep","windex"));
  tindex_ = consumes<int>(edm::InputTag("NanoAODFilterSlep","tindex"));
  bindex_ = consumes<int>(edm::InputTag("NanoAODFilterSlep","bindex"));
  opv_ = consumes<std::vector<reco::Vertex>>(edm::InputTag("offlineSlimmedPrimaryVertices"));
  gplab = consumes<std::vector<reco::GenParticle>>(edm::InputTag("prunedGenParticles"));

  //TFile mixfile = TFile("MINIskim.root","open");
  //auto events=Events("MINIskim.root");

}
WtagProducer::~WtagProducer(){
}

void WtagProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void WtagProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::cout<<"startproducer "<<iEvent.id().event()<<std::endl;
  std::srand(iEvent.id().event()+9 );
  //std::cout<<"1"<<std::endl;
  edm::Handle<edm::View<pat::Jet>>jets;
  edm::Handle<std::vector<pat::Muon>>muons;


  //edm::ESHandle<TrackerTopology> tTopoHandle;
  //iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);
  //const TrackerTopology* const tTopo = tTopoHandle.product();

  edm::Handle<std::vector<pat::Electron>>electrons;
  edm::Handle<int>windex;  
  edm::Handle<int>tindex;
  edm::Handle<int>bindex;
  edm::Handle<edm::View<pat::PackedCandidate>>PackedCandidates;
  iEvent.getByToken(src_, jets);


  iEvent.getByToken(windex_, windex);
  iEvent.getByToken(tindex_, tindex);
  iEvent.getByToken(bindex_, bindex);  

  iEvent.getByToken(mu_, muons);
  iEvent.getByToken(el_, electrons); 



  std::vector<pat::Muon> newmu = *(muons.product()); 
  std::vector<pat::Electron> newel = *(electrons.product()) ; 


  int iwindex=*(windex.product());
  int itindex=*(tindex.product());
  int ibindex=*(bindex.product());
  //std::cout<<"tindex "<<itindex<<std::endl;

  bool isfmerge=false;
  if(iwindex<0)isfmerge=true;
  iEvent.getByToken(pfcs_, PackedCandidates);
  edm::Handle<edm::View<pat::Jet>> jetsAK4;
  iEvent.getByToken(srcAK4_, jetsAK4);

  edm::Handle<edm::View<pat::Jet>> sj;
  iEvent.getByToken(sj_, sj);

  edm::Handle<reco::VertexCompositePtrCandidateCollection> svs;
  iEvent.getByToken(svs_, svs);

  reco::VertexCompositePtrCandidateCollection newSV = *(svs.product());

  //for (size_t i = 0; i < svs->size(); i++) 
	//{
    	//const reco::VertexCompositePtrCandidate& sv = (*svs)[i];
       	//std::cout<<"fromev "<<sv.p4().pt()<<std::endl;
	//}

  std::vector<pat::PackedCandidate> allpf = {};
  std::vector<pat::PackedCandidate> allpfb = {};
  std::vector<pat::PackedCandidate> allpfg = {};
  std::vector<pat::PackedCandidate> allpfsj = {};
  std::vector<pat::PackedCandidate> allpfsjb = {};
  reco::VertexCompositePtrCandidateCollection allvecs = {};
  reco::VertexCompositePtrCandidateCollection allvecsb = {};


  std::vector<pat::Jet> alljets;
  std::vector<bool> isfmergev={};
  isfmergev.push_back(isfmerge);
  std::vector<pat::Jet> bjettopush;
  std::vector<int> wmatch;
  std::vector<float> wmatchdr;
  std::vector<float> matchvals = {};
  for(int ival=0;ival<nvals;ival++)matchvals.push_back(-1000);
  int evtoload = 0;


  edm::Handle<std::vector<reco::GenParticle>> genparts;
  iEvent.getByToken(gplab, genparts);
  const std::vector<reco::GenParticle>* genpartsvec  = genparts.product();

  bool merge=merge_;
  bool mergeb=mergeb_;
  bool mergemj=mergemj_;
  bool load=load_;
  float fakefac=1.0;
  std::vector<reco::GsfTrack>gsftracks={};
  std::vector<reco::GsfElectronCore>gsfcores={};
  pat::Jet AK8pfjet;
  pat::Jet AK4pfjet;
  //float fdr=0.0;
  float fang=0.0;
  //float fwpt=0.0;
  float fdetaval=0.0;
  float fdphival=0.0;
  std::vector<float> sjlistw = {};
  std::vector<float> sjlistb = {};
  std::vector<float> sjmerger = {};
  reco::VertexCompositePtrCandidateCollection newvecs = newSV;
  std::vector<pat::Muon> newmus = newmu;
  std::vector<pat::Electron> newels = newel;
  float sumdiffb = 0.0;
  int nmusrot=0;
  int nelsrot=0;
  //edm::EDProductGetter pget;
  //std::cout<<"pget"<<std::endl;
  //edm::EDProductGetter pget=(jets->at(0)).daughter(idau)->productGetter();
  std::vector<std::pair<uint,std::pair<float,float>>> rekeyvec={};
  std::vector<std::pair<int,int>> rekeyvec2={};
  if(isfmerge)
	{
	std::cout<<"running merged version"<<std::endl;
	if (mergemj)
		{
		AK8pfjet=jets->at(itindex);
		uint ndau = AK8pfjet.numberOfDaughters();
		for(uint idau=0;idau<ndau;idau++)
			{

			const pat::PackedCandidate * daunw = dynamic_cast<const pat::PackedCandidate *>((jets->at(itindex)).daughter(idau) );

			//if(idau==0)pvref =daunw->vertexRef();
			allpfsj.push_back(*daunw);
			}
		AK4pfjet = jetsAK4->at(ibindex);
		//std::cout<<"MJ PT ratio "<<AK4pfjet.pt()/AK8pfjet.pt();
		uint ndaub = AK4pfjet.numberOfDaughters();
		for(uint idau=0;idau<ndaub;idau++)
			{
			const pat::PackedCandidate * daunb = dynamic_cast<const pat::PackedCandidate *>((jetsAK4->at(ibindex)).daughter(idau) );
			allpfsjb.push_back(*daunb);

			}
	

		auto const & sdSubjetsPuppi = (jets->at(itindex)).subjets("SoftDropPuppi");
	  	//if(isfmerge)
		//	{
		//	std::cout<<"mergeMvals"<<std::endl;
		//	std::cout<<"invm "<<(sdSubjetsPuppi[0]->p4()+sdSubjetsPuppi[1]->p4()).mass()<<std::endl;
		//	std::cout<<"ptasy "<<(abs(sdSubjetsPuppi[0]->pt()-sdSubjetsPuppi[1]->pt()))/(sdSubjetsPuppi[0]->pt()+sdSubjetsPuppi[1]->pt())<<std::endl;
		//	std::cout<<"ptvmasy "<<(abs(sdSubjetsPuppi[0]->pt()-sdSubjetsPuppi[1]->pt()))/((sdSubjetsPuppi[0]->p4()+sdSubjetsPuppi[1]->p4()).mass())<<std::endl;
		//	std::cout<<"ptsumvm "<<(sdSubjetsPuppi[0]->pt()+sdSubjetsPuppi[1]->pt())/((sdSubjetsPuppi[0]->p4()+sdSubjetsPuppi[1]->p4()).mass())<<std::endl;
		//	}
		matchvals[0] = -1.0;
		matchvals[1] = (reco::deltaR(sdSubjetsPuppi[0]->p4(),sdSubjetsPuppi[1]->p4()));
		matchvals[2] = -1.0;
		matchvals[3] = (sdSubjetsPuppi[0]->p4()+sdSubjetsPuppi[1]->p4()).mass();
		matchvals[4] = (sdSubjetsPuppi[1]->pt());
		matchvals[5] = (sdSubjetsPuppi[0]->pt());
		matchvals[6] = -1.0;
		matchvals[7] = (sdSubjetsPuppi[1]->eta());
		matchvals[8] = (sdSubjetsPuppi[0]->eta());
		matchvals[9] = -1.0;
		matchvals[10] = -1.0;
		matchvals[11] = -1.0;
	
		}
	else
		{
		//std::cout<<"rmv1"<<std::endl;
		auto const & sdSubjetsPuppi = (jets->at(itindex)).subjets("SoftDropPuppi");

		AK8pfjet = sdSubjetsPuppi[0];
		//std::cout<<"subjetpt "<<AK8pfjet.pt()<<std::endl;
		//std::cout<<"sjlistwTRY "<<AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:probb")<<std::endl;
		//uint ipc=0;
		//for (const auto &pc : *PackedCandidates)
		//	{
		//	std::cout<<"ALLDAU "<<ipc<<" pt "<<pc.pt()<<std::endl;
		//	ipc+=1;
		//	}

  		for (const auto &csj : *sj)
			{

			//std::cout<<"wjet dR "<<reco::deltaR(csj.p4(),AK8pfjet.p4())<<std::endl;
			if(reco::deltaR(csj.p4(),AK8pfjet.p4())<0.01)
				{
				//const reco::SecondaryVertexTagInfo &svTagInfo = *jet->tagInfoSecondaryVertex();
				//std::cout<<"tinfl"<<std::endl;
				//std::vector<std::string> const& tagInfoLabels1 = csj.tagInfoLabels() ;
  				//for (const auto ti : tagInfoLabels1)std::cout<<ti<<std::endl;
				auto curvert = (csj.tagInfoCandSecondaryVertex("pfInclusiveSecondaryVertexFinder"));
				//auto curvert = (csj.tagInfo("pfSecondaryVertex"));
				//std::cout<<"getit "<<std::endl;	
				uint nvtx = curvert->nVertices();
				//std::cout<<"nv "<<nvtx<<std::endl;	
				for (uint ii = 0; ii < nvtx; ii++)
					{
					allvecs.push_back(curvert->secondaryVertex(ii));
					//for(auto dp:curvert->secondaryVertex(ii).daughterPtrVector())
					//	{		
					//	std::cout<<"id "<<dp.id()<<" key "<<dp.key()<<std::endl;
					//	std::cout<<"pt "<<dp.get()->pt()<<std::endl;
					//	}
					}
									//std::cout<<"endv "<<std::endl;	
				//std::cout<<"bdisc "<<csj.bDiscriminator("pfDeepFlavourJetTags:probb")<<std::endl;
				//std::cout<<"Found!"<<std::endl;
				sjlistw.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probb"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probbb"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probuds"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probg"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probc"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:problepb"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepCSVJetTags:probb"));
				sjlistw.push_back(csj.bDiscriminator("pfDeepCSVJetTags:probbb"));
				break;
				}
			}
		//std::cout<<"NsjlistW "<<sjlistw.size()<<std::endl;
		AK4pfjet = sdSubjetsPuppi[1];
		//std::cout<<"starttag1 "<<std::endl;	
		//std::cout<<"rmv2"<<std::endl;
		//std::cout<<"sjlistbTRY "<<AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probb")<<std::endl;

  		for (const auto &csj : *sj)
			{
			//std::cout<<"bjet dR "<<reco::deltaR(csj.p4(),AK4pfjet.p4())<<std::endl;
			if(reco::deltaR(csj.p4(),AK4pfjet.p4())<0.01)
				{
				//const reco::SecondaryVertexTagInfo &svTagInfo = *jet->tagInfoSecondaryVertex();
				auto curvert = (csj.tagInfoCandSecondaryVertex());
				//auto curvert = (csj.tagInfo("pfSecondaryVertex"));
				//std::cout<<"getit "<<std::endl;	
				uint nvtx = curvert->nVertices();
				//std::cout<<"nv "<<nvtx<<std::endl;	
				for (uint ii = 0; ii < nvtx; ii++)allvecsb.push_back(curvert->secondaryVertex(ii));
				//std::cout<<"endv "<<std::endl;	
				//std::cout<<"bdisc "<<csj.bDiscriminator("pfDeepFlavourJetTags:probb")<<std::endl;
				sjlistb.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probb"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probbb"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probuds"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probg"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:probc"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepFlavourJetTags:problepb"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepCSVJetTags:probb"));
				sjlistb.push_back(csj.bDiscriminator("pfDeepCSVJetTags:probbb"));

				break;
				}
			}

		
		//std::cout<<"Nsjlistb "<<sjlistb.size()<<std::endl;
		//std::cout<<"sjlistw "<<sjlistw[0]<<std::endl;//
		//std::cout<<"sjlistb "<<sjlistb[0]<<std::endl;
		//std::cout<<"sjlistw dcsv "<<sjlistw[6]<<std::endl;
		//std::cout<<"sjlistb dcsv "<<sjlistb[6]<<std::endl;
		uint ndau1 = sdSubjetsPuppi[0]->numberOfDaughters();
		uint ndau2 = sdSubjetsPuppi[1]->numberOfDaughters();
		uint gdau = ndau1+ndau2;
		uint ndau = (jets->at(itindex)).numberOfDaughters();
		//std::cout<<"rmv3.1"<<std::endl;
		for(uint idau=0;idau<ndau1;idau++)
			{

			const pat::PackedCandidate * daunw = dynamic_cast<const pat::PackedCandidate *>((jets->at(itindex)).daughter(idau) );
			//std::cout<<"pget"<<std::endl;
			//pget=(jets->at(itindex)).daughter(idau)->productGetter();
			//if(idau==0)pvref =daunw->vertexRef();
			//std::cout<<daunw->vertexRef().id()<<std::endl;
			allpfsj.push_back(*daunw);
			//std::cout<<"idau "<<idau<<" daupt "<<daun->pt()<<" ndau "<<daun->numberOfDaughters()<<std::endl;
			}
		for(uint idau=ndau1;idau<gdau;idau++)
			{
			const pat::PackedCandidate * daunb = dynamic_cast<const pat::PackedCandidate *>((jets->at(itindex)).daughter(idau) );
			allpfsjb.push_back(*daunb);
			//std::cout<<"pt "<<daunb->pt()<<"  ->  "<<(jets->at(itindex)).daughterPtr(idau).key()<<std::endl;
			//const pat::PackedCandidate * daunb1 = dynamic_cast<const pat::PackedCandidate *>((jets->at(itindex)).daughter(idau) );
			//std::cout<<"running"<<std::endl;
			//std::cout<<"id "<<daunb1->vertexRef().id()<<std::endl;
			//reco::VertexRefProd vrp = reco::VertexRefProd(daunb1->vertexRef().id(),(daunb1->vertexRef()).productGetter());
			//std::cout<<"running1"<<std::endl; 
  			//pat::PackedCandidate newpack1(daunb1->p4(), daunb1->vertex(),daunb1->p4().pt(), daunb1->p4().eta(), daunb1->p4().phi(),daunb1->pdgId(),vrp,(daunb1)->vertexRef().key());	
			//std::cout<<"NEWPACK "<<newpack1.phi()<<","<<newpack1.eta()<<"  "<<newpack1.phiAtVtx()<<","<<newpack1.etaAtVtx()<<std::endl;
			//std::cout<<"idau "<<idau<<" daupt "<<daun->pt()<<" ndau "<<daun->numberOfDaughters()<<std::endl;
			}

		for(uint idau=gdau;idau<ndau;idau++)
			{
			const pat::PackedCandidate * daung = dynamic_cast<const pat::PackedCandidate *>((jets->at(itindex)).daughter(idau) );
			allpfg.push_back(*daung);
			//std::cout<<"idau "<<idau<<" daupt "<<daun->pt()<<" ndau "<<daun->numberOfDaughters()<<std::endl;
			}
		fdetaval = sdSubjetsPuppi[0]->eta()-sdSubjetsPuppi[1]->eta();
		fdphival = deltaPhi(sdSubjetsPuppi[0]->p4().phi(),sdSubjetsPuppi[1]->p4().phi());

		//std::cout<<"fdetaval "<<fdetaval<<" fdphival "<<fdphival<<std::endl;
		//fdr = deltaR(sdSubjetsPuppi[0]->p4(),sdSubjetsPuppi[1]->p4());
		fang = std::atan(fdetaval/fdphival);
		//fwpt=sdSubjetsPuppi[0]->p4().pt();
		}


	}
  else 
	{
	AK8pfjet = jets->at(iwindex);
	AK4pfjet = jetsAK4->at(ibindex);


	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepCSVJetTags:probb"));
	sjlistw.push_back(AK8pfjet.bDiscriminator("pfDeepCSVJetTags:probbb"));


	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepCSVJetTags:probb"));
	sjlistb.push_back(AK4pfjet.bDiscriminator("pfDeepCSVJetTags:probbb"));

	uint ndauw = AK8pfjet.numberOfDaughters();
	uint ndaub = AK4pfjet.numberOfDaughters();


	//std::cout<<"sjlistw "<<sjlistw[0]<<std::endl;
	//std::cout<<"sjlistb "<<sjlistb[0]<<std::endl;
	//std::cout<<"sjlistw dcsv "<<sjlistw[6]<<std::endl;
	//std::cout<<"sjlistb dcsv "<<sjlistb[6]<<std::endl;

	for(uint idau=0;idau<ndauw;idau++)
		{

		const pat::PackedCandidate * daunw = dynamic_cast<const pat::PackedCandidate *>((jets->at(iwindex)).daughter(idau) );
		//std::cout<<"pget"<<std::endl;
		//pget=(jets->at(itindex)).daughter(idau)->productGetter();
		//if(idau==0)pvref =daunw->vertexRef();
		allpfsj.push_back(*daunw);
		//std::cout<<"idau "<<idau<<" daupt "<<daun->pt()<<" ndau "<<daun->numberOfDaughters()<<std::endl;
		}
	for(uint idau=0;idau<ndaub;idau++)
		{
		const pat::PackedCandidate * daunb = dynamic_cast<const pat::PackedCandidate *>((jetsAK4->at(ibindex)).daughter(idau) );
		allpfsjb.push_back(*daunb);
		//std::cout<<"idau "<<idau<<" daupt "<<daun->pt()<<" ndau "<<daun->numberOfDaughters()<<std::endl;
		}

  	//std::cout<<"AK8 mass "<<AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass")<<std::endl;
	}
  //std::vector<fwlite::Event>::iterator curev = ev_.load()->toBegin() + evtoload;
  edm::Handle<std::vector<reco::Vertex>>opv;
  iEvent.getByToken(opv_, opv);
  reco::VertexRef PVRef(opv.id());
  reco::VertexRefProd PVRefProd(opv);
  std::vector<reco::Vertex> newopv = *(opv.product());
  std::vector<reco::Track>newtracks={};
  uint trackindex=0; 
  if(merge or mergeb)
	{
	//std::cout<<"Events to mix "<<nevmix<<std::endl;
	edm::Handle<std::vector<pat::Jet>> Wjets;
	edm::Handle<std::vector<bool>> ismergetemp;
	edm::Handle<std::vector<pat::PackedCandidate>> Wjetspfc;
	edm::Handle<std::vector<pat::PackedCandidate>> Allpfc;
	edm::Handle<std::vector<int>> wmatchf;
	edm::Handle<std::vector<float>> wmatchdr;
	edm::Handle<std::vector<float>> sjwh;
	edm::Handle<std::vector<float>> sjbh;
	//edm::Handle<std::vector<pat::Muon>> allmuswh;
	edm::Handle<std::vector<pat::Muon>> allmusbh;
	edm::Handle<std::vector<pat::Electron>> allelswh;
	edm::Handle<std::vector<pat::Electron>> allelsbh;
	edm::Handle<reco::VertexCompositePtrCandidateCollection> wvecsh;
	edm::Handle<reco::VertexCompositePtrCandidateCollection> bvecsh;
	edm::Handle<std::vector<reco::Vertex>>opvfh;


	std::vector<reco::Vertex>opvf;
  	//edm::Handle<reco::VertexCompositePtrCandidateCollection> svsfh;
	//float DRmin=0.0;
	//float DRmax=0.0;
	float dptcut=0.0;
	float detacut=0.0;
	std::vector<pat::Jet> Wjetsprod;
	std::vector<pat::PackedCandidate> Wjetspfcprod;
	std::vector<int> Wjetsmatchf;
	std::vector<float> Wjetsmatchfdr;
	std::vector<float> sjw;
	std::vector<float> sjb;

	//std::vector<pat::Muon> allmuswf;
	std::vector<pat::Muon> allmusbf;

	//std::vector<pat::Electron> allelswf;
	std::vector<pat::Electron> allelsbf;

	reco::VertexCompositePtrCandidateCollection wvecsf;
	//reco::VertexCompositePtrCandidateCollection bvecsf;

  	//reco::VertexCompositePtrCandidateCollection svsf;
	//std::cout<<"vrec1"<<std::endl;
  	//std::cout<<"tnum "<<tnum_<<" pnum "<<pnum_<<std::endl;
	bool foundev=false;
	int tries = 0;
	int maxtries=100;
	//inputTFile_->cd();

	if(load){		
	std::lock_guard<std::mutex> lock(write_mutex);
	std::cout<<"load"<<std::endl;
	while(foundev==false and tries<maxtries)
		{
		tries+=1;
		bool foundmerge=false;
		while(!foundmerge)
			{
	 		evtoload=std::rand()%nevmix;
			if(evtoload%tnum_==(pnum_-1))
				{
	  			//std::cout<<"evtoload "<<evtoload<<" - "<<nevmix<<std::endl;


				int iload=0;
   				for( ev_->to(evtoload); !(ev_->atEnd()); ++(*ev_)) 
					{
	  				//std::cout<<"loaded "<<iload+evtoload<<std::endl;
					ev_->getByLabel(edm::InputTag("WtagProducerpuppi","ismerge"), ismergetemp);
	  				//std::cout<<"1"<<std::endl;
					bool ismergetempval = false;
					if ((ismergetemp.product()->size())>0)ismergetempval=(ismergetemp.product())->at(0);
	  				//std::cout<<"2"<<std::endl;
					iload+=1;
					//std::cout<<"im fromfile "<<ismergetempval<<" im "<<isfmerge<<std::endl;
					if(isfmerge and ismergetempval)foundmerge=true;
					if((not isfmerge) and (not ismergetempval))foundmerge=true;
					if (foundmerge) break;
	  				//std::cout<<"3"<<std::endl;
					}
				}
			}

	  	//std::cout<<"4"<<std::endl;
		//float deltabtag = 0.0;
		//float deltawtag = 0.0;
	  	ev_->getByLabel(edm::InputTag("offlineSlimmedPrimaryVertices"), opvfh);
	  	opvf = *(opvfh.product());
		//std::cout<<opvfh.id()<<std::endl;
	  	//ev_->getByLabel(edm::InputTag("slimmedSecondaryVertices"), opvfh);
		//std::cout<<"id1 "<<wvecsh.id()<<std::endl;

		if(merge or mergeb)
			{
			//std::cout<<"Load W"<<std::endl;
	  		//ev_->getByLabel(edm::InputTag("offlineSlimmedPrimaryVertices"), opvfh);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wvecs"), wvecsh);
			//std::cout<<"id1 "<<wvecsh.id()<<std::endl;
			//std::cout<<"jet"<<std::endl;
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wjet"), Wjets);
			//std::cout<<"cands"<<std::endl;
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wcands"), Wjetspfc);
			//std::cout<<"id2 "<<Wjetspfc.id()<<std::endl;
			//std::cout<<"match"<<std::endl;
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wmatch"), wmatchf);
	  		//ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wmatchhot"), wmatchfhot);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wmatchdr"), wmatchdr);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","sjb"), sjbh);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","sjw"), sjwh);
	  		//ev_->getByLabel(edm::InputTag("slimmedSecondaryVertices"), svsfh);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","allmus"), allmusbh);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","allels"), allelsbh);


			allmusbf = *(allmusbh.product());
			allelsbf = *(allelsbh.product());
			sjb = *(sjbh.product());
			sjw = *(sjwh.product());
			


			if (sjw.size()<6 or sjb.size()<6 or sjlistw.size()<6 or sjlistb.size()<6)
				{
				std::cout<<"NOBTAGINFO"<<std::endl;
				continue;
				}
		
			sumdiffb=0;
			for (int ibt=0;ibt<6;ibt++) sumdiffb+=fabs(sjw[ibt]-sjlistw[ibt]);
			//std::cout<<"1"<<std::endl;
			sjmerger=sjw;
			
			if ((sumdiffb>1.2) )
				{
				//std::cout<<"flavour mismatch "<<sumdiffb<<std::endl;
				continue;
				}


			wvecsf = *(wvecsh.product());
			//bvecsf = *(bvecsh.product());



			Wjetsmatchf = *(wmatchf.product());
			Wjetsmatchfdr = *(wmatchdr.product());
			//std::cout<<"3"<<std::endl;
			dptcut=0.4;
			detacut=1.4;

			if(isfmerge)
				{
				dptcut=0.5;
				detacut=1.3;
				}
			}
		if (false)//(mergeb)
			{

			//std::cout<<"Load B"<<std::endl;
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","bvecs"), bvecsh);
			//std::cout<<"id1 "<<bvecsh.id()<<std::endl;
			//std::cout<<"jet"<<std::endl;
	  		//ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wjet"), Wjets);
			//std::cout<<"cands"<<std::endl;
	  		//ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wcands"), Wjetspfc);
			//std::cout<<"id2 "<<Wjetspfc.id()<<std::endl;


	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","bjet"), Wjets);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","bcands"), Wjetspfc);

			Wjetsmatchf.push_back(-1);
			Wjetsmatchfdr.push_back(-1.0);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","sjb"), sjbh);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","sjw"), sjwh);
	  		//ev_->getByLabel(edm::InputTag("slimmedSecondaryVertices"), svsfh);

	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","allmusb"), allmusbh);
	  		ev_->getByLabel(edm::InputTag("WtagProducerpuppi","allelsb"), allelsbh);


			//std::cout<<"start insert"<<std::endl;
			//newvecs.insert(newvecs.end(),bvecsf.begin(),bvecsf.end());
			//std::cout<<"end insert"<<std::endl;
			
			allmusbf = *(allmusbh.product());
			allelsbf = *(allelsbh.product());

			sjb = *(sjbh.product());
			sjw = *(sjwh.product());
			//std::cout<<"1"<<std::endl;
			//svsf = *(svsfh.product());
			//std::cout<<sjw.size()<<","<<sjb.size()<<std::endl;
			if (sjw.size()<6 or sjb.size()<6 or sjlistw.size()<6 or sjlistb.size()<6)
				{
				std::cout<<"NOOBTAGINFO"<<std::endl;
				continue;
				}
			float wsj = sjw[0]+sjw[1]+sjw[5];
			float bsj = sjb[0]+sjb[1]+sjb[5];
			float curwsj = sjlistw[0]+sjlistw[1]+sjlistw[5];
			float curbsj = sjlistb[0]+sjlistb[1]+sjlistb[5];
			sumdiffb=0;
			for (int ibt=0;ibt<6;ibt++) sumdiffb+=fabs(sjb[ibt]-sjlistb[ibt]);
			
			sjmerger=sjb;


			//deltabtag = curbsj-bsj;
			//deltawtag = curwsj-wsj;
			//std::cout<<deltabtag<<","<<deltawtag<<std::endl;
			//std::cout<<" passed btag B "<<sjb[0]<<" "<<sjb[1]<<" "<<sjb[5]<<std::endl;
			//std::cout<<" passed btag B "<<sjlistb[0]<<" "<<sjlistb[1]<<" "<<sjlistb[5]<<std::endl;
			if ((wsj>bsj and curwsj<curbsj) or (wsj<bsj and curwsj>curbsj) )
				{
				//std::cout<<"SJMM B "<<wsj<<","<<bsj<<" "<<curwsj<<","<<curbsj<<std::endl;
				continue;
				}
			//if ((sumdiffb>1.2) )
			//	{
				//std::cout<<"flavour mismatch "<<sumdiffb<<std::endl;
			//	continue;
			//	}

	  		//ev_->getByLabel(edm::InputTag("WtagProducerpuppi","wvecs"), wvecsh);

			//wvecsf = *(wvecsh.product());
			wvecsf = *(bvecsh.product());


			dptcut=0.5;
			detacut=1.3;
			}

		Wjetsprod = *(Wjets.product());
		Wjetspfcprod = *(Wjetspfc.product());
		//std::cout<<"Wjetspfcprod "<<Wjetspfc.id()<<std::endl;
		//std::cout<<deltabtag<<","<<deltawtag<<std::endl;
		if (Wjetsprod.size()>0)
			{
			float deta = std::fabs(Wjetsprod[0].eta()-AK8pfjet.eta());
			float dpt = std::fabs(Wjetsprod[0].pt()-AK8pfjet.pt());
			float dptfrac = dpt/(0.5*(AK8pfjet.pt()+Wjetsprod[0].pt()));
			if(false)
				{
				deta = std::fabs(Wjetsprod[0].eta()-AK4pfjet.eta());
				dpt = std::fabs(Wjetsprod[0].pt()-AK4pfjet.pt());
				dptfrac = dpt/(0.5*(AK4pfjet.pt()+Wjetsprod[0].pt()));
				if (abs(int(Wjetspfcprod.size()-AK4pfjet.numberOfDaughters()))>15)
					{
					std::cout<<"SKIP cands"<<std::endl;
					continue;
					}
				}



			//std::cout<<"pt comp "<<Wjetsprod[0].pt()<<" , "<<AK8pfjet.pt()<<std::endl;
			//std::cout<<"pt frac "<<dptfrac<<std::endl;
			//std::cout<<tries<<","<<deta<<","<<dptfrac<<std::endl;
			if(deta<detacut and dptfrac<dptcut)foundev=true;
				
			}

		}

	if(tries==maxtries) Wjetsprod.clear();
	}
	else
		{
		std::cout<<"inelse"<<std::endl;
		if(merge or mergeb)
			{
			Wjetsprod={AK8pfjet};
			Wjetspfcprod=allpfsj;
			Wjetsmatchf.push_back(-1);
			Wjetsmatchfdr.push_back(-1.0);
			}
		if(false)//(mergeb)
			{
			Wjetsprod={AK4pfjet};
			//std::cout<<"allpfsjb "<<allpfsjb.size()<<std::endl;
			Wjetspfcprod=allpfsjb;
			Wjetsmatchf.push_back(-1);
			Wjetsmatchfdr.push_back(-1.0);
			}
		}
	//std::cout<<"Wjetspfcprod "<<Wjetspfcprod.size()<<std::endl;



	if (Wjetsprod.size()>0)
		{
		//std::cout<<"PT ratio "<<Wjetsprod[0].pt()/AK8pfjet.pt();
		std::string textm = "W";
		if(mergeb)textm = "b";
		//std::cout<<"Found "<<textm<<" match. Tries "<<tries<<" of "<<maxtries<<std::endl;
		//std::cout<<"Tries "<<maxtries<<std::endl;
    		//std::cout<<"prerotate "<<Wjetsprod[0].pt()<<std::endl;

		//if(merge and (not mergeb))
		//{
		//	std::cout<<"tauload!"<<std::endl;
		//	float tau21=1.0;
		//	if(Wjetsprod[0].userFloat("NjettinessAK8Puppi:tau1")>0.0) tau21=Wjetsprod[0].userFloat("NjettinessAK8Puppi:tau2")/Wjetsprod[0].userFloat("NjettinessAK8Puppi:tau1");
		//	std::cout<<"FFtau21 "<<tau21<<std::endl;
		//}

		float ang = 2*3.1415*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));		
		if(isfmerge)
			{
			if(merge)ang=1.0*fang;
			if(mergeb)ang=-1.0*fang;
			}
		float DRt = 0.0;
		float detaval = 0.0;
		float dphival = 0.0;
		ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > wjp4;
		float tmass = 0.0;
		if(mergeb)tmass = 170.0;
		if(merge) tmass = 180.0 + (120.0*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
		//std::cout<<"tmass "<<tmass<<std::endl;
		float ptrat=1.0;//fwpt/Wjetsprod[0].p4().pt();
		for(uint idr=0;idr<80;idr++)
				{
				DRt = float(idr)*0.01;
				detaval = DRt*std::sin(ang);
				dphival = DRt*std::cos(ang);
				if(mergeb)
					{
					wjp4 = Wjetsprod[0].p4();
					//wjp4.SetPhi(AK4pfjet.phi()+dphival);
					//wjp4.SetEta(AK4pfjet.eta()+detaval);
					wjp4.SetPhi(AK4pfjet.phi()+dphival);
					wjp4.SetEta(AK4pfjet.eta()+detaval);
					if ((AK4pfjet.p4()+wjp4*ptrat).mass()>(tmass*1.12))break;
					}
				if(merge)
					{
					wjp4 = Wjetsprod[0].p4();
					wjp4.SetPhi(AK8pfjet.phi()+dphival);
					wjp4.SetEta(AK8pfjet.eta()+detaval);
					if ((AK8pfjet.p4()+wjp4).mass()>(tmass))break;
					}
				}
		if(mergeb)	{
			wjp4.SetPhi(AK8pfjet.p4().phi());
			wjp4.SetEta(AK8pfjet.p4().eta());
				}
  		if(mergeb_)	{
					std::cout<<"dR W,newW "<<reco::deltaR(AK8pfjet.p4(),wjp4)<<std::endl;
					std::cout<<"dR B,newW "<<reco::deltaR(AK4pfjet.p4(),wjp4)<<std::endl;
					}
			
			
		//else
		//	{
		//	DRt = DRmin + (DRmax-DRmin)*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

		//	detaval = DRt*std::sin(ang);
		//	dphival = DRt*std::cos(ang);

		//	wjp4 = Wjetsprod[0].p4();
		//	wjp4.SetPhi(AK8pfjet.phi()+dphival);
		//	wjp4.SetEta(AK8pfjet.eta()+detaval);
		//	}
		//std::cout<<wjp4.pt()<<std::endl;
		//std::cout<<Wjetsprod[0].pt()<<std::endl;
		//std::cout<<ang<<std::endl;
		//std::cout<<(AK8pfjet.p4().eta())<<std::endl;

		if(merge)
			{
			matchvals[0] = (reco::deltaR(AK8pfjet.p4(),Wjetsprod[0].p4()));
			matchvals[1] = (reco::deltaR(AK8pfjet.p4(),wjp4));
			matchvals[2] = (ang);
			matchvals[3] = ((AK8pfjet.p4()+wjp4).mass());
			matchvals[4] = (wjp4.pt());
			matchvals[5] = (AK8pfjet.p4().pt());
			matchvals[6] = (Wjetsprod[0].pt());
			matchvals[7] = (wjp4.eta());
			matchvals[8] = (AK8pfjet.p4().eta());
			matchvals[9] = (Wjetsprod[0].eta());
			matchvals[10] = (Wjetsmatchf[0]);
			matchvals[11] = (Wjetsmatchfdr[0]);
			matchvals[12] = (sumdiffb);
			}
		if(mergeb)
			{
			matchvals[0] = (reco::deltaR(AK8pfjet.p4(),Wjetsprod[0].p4()));
			matchvals[1] = (reco::deltaR(AK8pfjet.p4(),wjp4));
			matchvals[2] = (ang);
			matchvals[3] = ((AK4pfjet.p4()+wjp4).mass());
			matchvals[4] = (wjp4.pt());
			matchvals[5] = (AK8pfjet.p4().pt());
			matchvals[6] = (Wjetsprod[0].pt());
			matchvals[7] = (wjp4.eta());
			matchvals[8] = (AK8pfjet.p4().eta());
			matchvals[9] = (Wjetsprod[0].eta());
			matchvals[10] = (Wjetsmatchf[0]);
			matchvals[11] = (Wjetsmatchfdr[0]);
			matchvals[12] = (sumdiffb);

			}
		if (wjp4.pt()>AK8pfjet.p4().pt())sjmerger.push_back(0.0);
		else sjmerger.push_back(1.0);
		//std::cout<<"donew"<<std::endl;
	  	//if(mergeb and isfmerge)
		//	{
		//	std::cout<<"mergeBvals"<<std::endl;
		//	std::cout<<"invm "<<(wjp4+AK8pfjet.p4()).mass()<<std::endl;
		//	std::cout<<"ptasy "<<(abs(wjp4.pt()-AK8pfjet.pt()))/(wjp4.pt()+AK8pfjet.pt())<<std::endl;
		//	std::cout<<"ptvmasy "<<(abs(wjp4.pt()-AK8pfjet.pt()))/((wjp4+AK8pfjet.p4()).mass())<<std::endl;
		//	std::cout<<"ptsumvm "<<(wjp4.pt()+AK8pfjet.pt())/((wjp4+AK8pfjet.p4()).mass())<<std::endl;
		//	}
		//std::cout<<"Wjetsmatchf[0] "<<Wjetsmatchf[0]<<" Wjetsmatchfdr[0] "<<Wjetsmatchfdr[0]<<std::endl;
		//float randomass = 0.0;
		//if(mergeb)randomass=topmass_.load()->GetRandom();
		//if(merge)randomass=wwmass_.load()->GetRandom();
		//std::cout<<"randomass "<<randomass<<std::endl;
		fakefac=1.0;//1.35*172.5*randomass/(AK8pfjet.p4()+wjp4).mass();
		//if(mergeb and isfmerge)fakefac=(173*1.12)/(AK8pfjet.p4()+wjp4).mass();
		//std::cout<<"DRt "<<DRt<<std::endl;

		//std::cout<<"nvsize "<<newvecs.size()<<std::endl;
		//for (auto pv : newopv)std::cout<<"newpv "<<pv.p4().P()<<std::endl;
		//for (auto pv : opvf)std::cout<<"opvf "<<pv.p4().P()<<std::endl;

		float dx = newopv[0].position().x()-opvf[0].position().x();
		//std::cout<<"x dx "<<newopv[0].position().x()<<" "<<dx<<std::endl;
		float dy = newopv[0].position().y()-opvf[0].position().y();
		float dz = newopv[0].position().z()-opvf[0].position().z();
		float detaj = wjp4.Eta()-Wjetsprod[0].p4().Eta();
		float dthetaj = wjp4.Theta()-Wjetsprod[0].p4().Theta();
		float dphij = reco::deltaPhi(wjp4,Wjetsprod[0].p4());
		//std::cout<<"newmussize "<<newmus.size()<<std::endl;
 		reco::VertexCollection rotatedpvs = {};
		//uint vi1=0;
		//for(auto nopv: newopv) 
		//	{
		//	std::cout<<"FROMMINI "<<vi1<<" nopv.position().x() "<<nopv.position().x()<<std::endl;
		//	vi1+=1;
		//	}
		uint vi=0;
		for(auto copv: opvf)
			{//check charge,chi2
			ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > curvp4(copv.p4());
			curvp4.SetPhi(normalizedPhi(copv.p4().Phi()+dphij));
			curvp4.SetEta(copv.p4().Eta()+detaj);
			curvp4 = fakefac*curvp4;
			//float dx1 = curvp4.x()-(curv.p4()).x();
			//float dy1 = curvp4.y()-(curv.p4()).y();
			//float dz1 = curvp4.z()-(curv.p4()).z();
			//std::cout<<"dx "<<dx<<std::endl;
			//std::cout<<"dy "<<dy<<std::endl;
			//std::cout<<"dz "<<dz<<std::endl;

			//std::cout<<"vi "<<vi<<" copv.position().x() "<<copv.position().x()<<std::endl;
                        math::XYZPoint vert(copv.position().x()+dx, copv.position().y()+dy, copv.position().z()+dz);
			math::XYZPoint vtx1 = protate(vert,newopv[0],dthetaj,dphij);

			ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > pxyzep4;
			pxyzep4.SetPxPyPzE(curvp4.Px(),curvp4.Py(),curvp4.Pz(),curvp4.E());
     			rotatedpvs.push_back(reco::Vertex(vtx1, copv.error(), copv.chi2(), copv.ndof(), copv.tracksSize()));
			vi+=1;
			}

  		auto outputspvs = std::make_unique<reco::VertexCollection>(rotatedpvs);
  		edm::OrphanHandle<reco::VertexCollection> ohandpv = iEvent.put(std::move(outputspvs),"newopvs");	

  		std::vector<std::pair<uint,edm::Ref<reco::VertexCollection>>>vertmap = {} ;
		ev_->getByLabel(edm::InputTag("packedPFCandidates"), Allpfc);
		std::vector<pat::PackedCandidate> Allpfcv = *(Allpfc.product());
		//std::cout<<"Number of vecs "<<wvecsf.size()<<std::endl;

		uint nvv=0;
		for(const auto curv : wvecsf)
			{//check charge,chi2
			ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > curvp4(curv.p4());
			curvp4.SetPhi(normalizedPhi(curv.p4().Phi()+dphij));
			curvp4.SetEta(curv.p4().Eta()+detaj);
			curvp4 = fakefac*curvp4;
			//float dx1 = curvp4.x()-(curv.p4()).x();
			//float dy1 = curvp4.y()-(curv.p4()).y();
			//float dz1 = curvp4.z()-(curv.p4()).z();
			//std::cout<<"dx "<<dx<<std::endl;
			//std::cout<<"dy "<<dy<<std::endl;
			//std::cout<<"dz "<<dz<<std::endl;

                        math::XYZPoint vert(curv.vertex().x()+dx, curv.vertex().y()+dy, curv.vertex().z()+dz);
			math::XYZPoint vtx1 = protate(vert,newopv[0],dthetaj,dphij);

			ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > pxyzep4;
			pxyzep4.SetPxPyPzE(curvp4.Px(),curvp4.Py(),curvp4.Pz(),curvp4.E());

			reco::VertexCompositePtrCandidate curvptr(curv.charge(), pxyzep4, vtx1,curv.vertexCovariance(), curv.vertexChi2(), curv.vertexNdof(),curv.pdgId(), curv.status(),true);
			for(auto dp:curv.daughterPtrVector())
				{
				//edm::Wrapper tempr = ev_->getByProductID(dp.id());
				//tempr->product();	
				//std::cout<<"pt "<<Allpfcv[dp.key()].phi()<<" eta "<<Allpfcv[dp.key()].eta()<<std::endl;
				rekeyvec.push_back(std::pair<uint,std::pair<float,float>>(newvecs.size(),std::pair<float,float>(Allpfcv[dp.key()].phi(),Allpfcv[dp.key()].eta())));

				//edm::Ptr<pat::PackedCandidate>candptr(Allpfc,dp.key());
				//curvptr.addDaughter(candptr);	
				
				}
     			newvecs.push_back(curvptr);
			//std::cout<<"DR "<<reco::deltaR(pxyzep4,wjp4)<<std::endl;
			nvv+=1;
			}
		//std::cout<<"newvecssize "<<newvecs.size()<<std::endl;

		//std::cout<<"nvsize "<<newvecs.size()<<std::endl;
  		//edm::Ref<reco::VertexCollection> VertexRef = edm::Ref(opv);

  		//typedef edm::RefProd<VertexCollection> VertexRefProd;
  		//typedef edm::RefVector<VertexCollection> VertexRefVector;
		//trackmap
		//std::cout<<"file size "<<Wjetspfcprod.size()<<" b size "<<AK4pfjet.numberOfDaughters()<<" W size "<<AK8pfjet.numberOfDaughters()<<std::endl;


		std::vector<uint>trindices={};
		//for(auto nnv :  newvecs)
		//	{
		//	for(auto dp:nnv.daughterPtrVector())
		//		{
		//		//edm::Wrapper tempr = ev_->getByProductID(dp.id());
		//		//tempr->product();			
		//		std::cout<<"id "<<dp.id()<<" key "<<dp.key()<<std::endl;
		//		}
		//	}
		//for(uint idau=0;idau<Allpfcv.size();idau++)std::cout<<"ALL --- idau "<<idau<<","<<Allpfcv[idau].sourceCandidatePtr(0).key()<<" "<<Allpfcv[idau].fromPV()<<" "<<Allpfcv[idau].pvAssociationQuality()<<std::endl;


		for(uint idau=0;idau<Wjetspfcprod.size();idau++)
			{
				//std::cout<<"1"<<std::endl;
				pat::PackedCandidate lPack = Wjetspfcprod[idau];
				//std::cout<<"pt "<<daunb->pt()<<"  ->  "<<(jets->at(itindex)).daughterPtr(idau).key()<<std::endl;
				//std::cout<<"id "<<dp.id()<<" key "<<dp.key()<<std::endl;
				//lPack.setVertex(newopv[0].position());
				ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > packp4(lPack.p4());
				/*std::cout<<"matchpack "<<packp4.eta()<< "<<packp4.phi()"<<std::endl;
				for (size_t i = 0; i < svsf.size(); i++) 
					{
				    	const reco::VertexCompositePtrCandidate& svf = (svsf)[i];

					for (size_t j = 0; j < svf.numberOfDaughters(); j++) 
						{
						ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > svpackp4(svf.daughterPtr(j)->p4());
						if ((reco::deltaR(packp4,svpackp4))<0.2)std::cout<<"fromf "<<svpackp4.eta()<<" fromf "<<svpackp4.phi()<<std::endl;
						}
					}*/
				//std::cout<<"PHI? "<<(reco::deltaPhi(Wjetsprod[0].p4(),packp4))<<std::endl;
				//std::cout<<"OLDPACK "<<lPack.phi()<<","<<lPack.eta()<<"  "<<lPack.phiAtVtx()<<","<<lPack.etaAtVtx()<<std::endl;
				//std::cout<<"DRpre "<<reco::deltaR(lPack.p4(),AK8pfjet.p4())<<std::endl;
				//std::cout<<"Dphifromw "<<reco::deltaPhi(lPack.p4().phi(),Wjetsprod->at(0).p4().phi())<<std::endl;
				//std::cout<<"Detafromw "<<Wjetsprod->at(0).p4().eta()-packp4.eta()<<std::endl;
				//std::cout<<"DRfromw "<<reco::deltaR(Wjetsprod->at(0).p4(),wjp4)<<std::endl;
				//float packdeta=packp4.eta()-Wjetsprod[0].p4().eta();
				//float packdphi=reco::deltaPhi(packp4,Wjetsprod[0].p4());

				packp4.SetPhi(lPack.p4().Phi()+dphij);
				packp4.SetEta(lPack.p4().Eta()+detaj);
				//packp4.SetPt(Wjetsprod->at(0).p4().pt()*fakefac);
				//packp4.SetM(Wjetsprod->at(0).p4().mass()*fakefac);

				packp4 = fakefac*packp4*ptrat;
  				//float dx1 = packp4.x()-(lPack.p4()).x();
				//float dy1 = packp4.y()-(lPack.p4()).y();
				//float dz1 = packp4.z()-(lPack.p4()).z();
				//std::cout<<"pack "<<idau<<" "<<lPack.vertexRef().id()<<" , "<<lPack.fromPV(0)<<std::endl;
				float newphiv=normalizedPhi((lPack.phiAtVtx()+dphij));
 				float newetav=lPack.etaAtVtx()+detaj;

  				//pat::PackedCandidate newpack(packp4, newopv[0].position(),lPack.ptTrk(), packp4.eta(), packp4.phi(),lPack.pdgId(),reco::VertexRefProd(VertexRef0.id(),VertexRef0.productGetter()),VertexRef0.key());
				//std::cout<<pvref.id()<<std::endl;
				//autovref = ev_->getByProductID()

  				//pat::PackedCandidate newpack(packp4, lPack.vertex(),packp4.pt(), packp4.eta(), packp4.phi(),lPack.pdgId(),reco::VertexRefProd(lPack.vertexRef().id(),lPack.vertexRef().productGetter()),lPack.vertexRef().key());
				//std::cout<<"vertpoint "<<lPack.vertex().x()<< " "<<lPack.vertex().y()<< " "<<lPack.vertex().z()<<std::endl;
				//std::cout<<"frompv "<<lPack.fromPV()<<std::endl;
				//std::cout<<"PVposfromf "<<opvf[0].position().x()<< " "<<opvf[0].position().y()<< " "<<opvf[0].position().z()<<std::endl;
				//std::cout<<"PVpos "<<newopv[0].position().x()<< " "<<newopv[0].position().y()<< " "<<newopv[0].position().z()<<std::endl;
				//std::cout<<"LPACK VERT lPack.vertex().x() "<<lPack.vertex().x()<<std::endl;
				math::XYZPoint vpoint1(lPack.vertex().x()+dx, lPack.vertex().y()+dy, lPack.vertex().z()+dz);
				math::XYZPoint vpoint = protate(vpoint1,newopv[0],dthetaj,dphij);
				//reco::Vertex newopv = reco::Vertex(vpoint,lPack.vertex().Error4D(), lPack.vertex().t0(), lPack.vertex().chi2(), lPack.vertex().ndof(), lPack.vertex().tracksSize());
				//edm::Ref<VertexCollection> tempVertexRef;

				//std::cout<<lPack.vertexRef().id()<<","<<lPack.vertexRef().key()<<std::endl;
				//bool foundvm=false;
				//for(auto vm : vertmap)
				//	{
				//	if (vm.first==lPack.vertexRef().key())foundvm=true;
				//	}

				//if(!foundvm)
				//	{
				//	std::cout<<"lPack.vertexRef().get() "<<lPack.vertexRef().get()->position().x()<<std::endl;
				//	edm::Ref<reco::VertexCollection> curref(ohandpv,lPack.vertexRef().key());
				//	std::cout<<"lPack.vertexRef().key() "<<lPack.vertexRef().key()<<std::endl;
				//	vertmap.push_back(std::pair<uint,edm::Ref<reco::VertexCollection>>(lPack.vertexRef().key(),curref));
				//	}
				//edm::Ref<reco::VertexCollection> curref(ohandpv,lPack.vertexRef().key(),&iEvent.productGetter());
				//reco::VertexRefProd currefprod(curref.id(),&iEvent.productGetter());
				//reco::VertexRefProd currefprod(lPack.vertexRef().id(),lPack.vertexRef().productGetter();
  				pat::PackedCandidate newpack(packp4,vpoint,lPack.ptTrk()*ptrat, newetav, newphiv,lPack.pdgId(),PVRefProd,0);

  				//pat::PackedCandidate newpack(packp4, lPack.vertex(),lPack.ptTrk(), newetav, newphiv,lPack.pdgId(),PVRefProd,PVRef.key());
				//reco::VertexRef PVRef(opv.id());
				//reco::VertexRefProd PVRefProd(opv);  	
				//newpack.track_ = lPack.track_;
				//newpack.setP4(packp4);
				//std::cout<<"idau "<<idau<<" "<<lPack.fromPV()<<" "<<lPack.pvAssociationQuality() <<std::endl;

				if (lPack.hasTrackDetails())
					{
					auto track = *(lPack.bestTrack());


					reco::Track newtrack = trotate(track, dx, dy, dz, dthetaj, dphij,newopv[0]);

					auto csch=lPack.covarianceSchema();
					auto cver=lPack.covarianceVersion();
					//track->sourceCandidatePtr(0).key()

					if(abs(lPack.pdgId())==13) 
						{
						trindices.push_back(trackindex);
						std::cout<<"MUTRACKETA "<<track.eta()<<std::endl;
						std::cout<<"ROTTRACKETA "<<newtrack.eta()<<std::endl;

						}
					trackindex+=1;
     					newpack.setHits(newtrack) ;
     					newpack.setTrackProperties(newtrack,newtrack.covariance(), csch,cver);
					//std::cout<<"Pack.bestTrack().sourceCandidatePtr(i).key() "<< <<std::endl;
					//std::vector<reco::CandidatePtr> daughters = v.daughterPtrVector();
					//v.clearDaughters();
					//for(std::vector<reco::CandidatePtr>::const_iterator it = daughters.begin(); it != daughters.end(); ++it) 
					//	{
					//	    if((*pf2pc)[*it].isNonnull() && (*pf2pc)[*it]->numberOfHits() > 0)v.addDaughter(reco::CandidatePtr(edm::refToPtr((*pf2pc)[*it]) ));
					//	}

		                       // v.addDaughter(reco::CandidatePtr(edm::refToPtr((*pf2pc)[*it]) ));

					}
				//std::cout<<"3"<<std::endl;
				//if (newpack.hasTrackDetails())
				//	{
				//	auto track = newpack.bestTrack();
					//std::cout<<"hastrack POST "<<track->pt()<<","<<track->eta()<<","<<track->phi()<<std::endl;
				//	}	
		 		//lPack.setP4(packp4);
				//std::cout<<"NEWPACK "<<newpack.phi()<<","<<newpack.eta()<<"  "<<newpack.phiAtVtx()<<","<<newpack.etaAtVtx()<<std::endl;
				//std::cout<<"DRpost "<<reco::deltaR(newpack.p4(),AK8pfjet.p4())<<std::endl;
				//std::cout<<"Dphifromwpost "<<reco::deltaPhi(newpack.p4().phi(),wjp4.phi())<<std::endl;
				//std::cout<<"Detafromwpost "<<wjp4.eta()-packp4.eta()<<std::endl;
				//std::cout<<"DRfromwpost "<<reco::deltaR(newpack.p4(),wjp4)<<std::endl;
				//std::cout<<"DR "<<reco::deltaR(newpack.p4(),wjp4)<<std::endl;
				for(auto keyvec : rekeyvec)
					{
					float diffeta = fabs(lPack.p4().eta()-keyvec.second.second);
					float diffphi = fabs(lPack.p4().phi()-keyvec.second.first);
					//std::cout<<"diffeta "<<diffeta<<std::endl;
					//std::cout<<"diffphi "<<diffphi<<std::endl;

					if (diffeta<0.000001 and diffphi<0.000001)
						{
						//std::cout<<"Foundit "<<keyvec.first<<std::endl;
						edm::Ptr<pat::PackedCandidate>candptr(&allpf,idau);	
						rekeyvec2.push_back(std::pair<int,int>(keyvec.first,idau));
						//newvecs[keyvec.first].addDaughter(candptr);

						}
					}
				//std::cout<<"np"<<std::endl;
				//std::cout<<lPack.dxy()<<" "<<lPack.dz()<<std::endl;
				//std::cout<<newpack.dxy()<<" "<<newpack.dz()<<std::endl;
				allpf.push_back(newpack);
						
			  }


 		nmusrot=0;
		//uint nlmu=0;
		//std::cout<<"----"<<std::endl;
  		std::vector<std::pair<int,int>> rekeyvecglobmu={};
  		//std::vector<std::pair<int,int>> rekeyvecinmu={};
  		//std::vector<std::pair<int,int>> rekeyvecoutmu={};
		for(const auto curmu : allmusbf)
			{

			//std::cout<<"Loosemu "<<muon::isLooseMuon(curmu)<<std::endl;

			//auto itrack=curmu.innerTrack();
			//std::cout<<"mutrackinfo "<<itrack.get()->innerMomentum().x()<<std::endl;
			pat::Muon tempmu = curmu;
			ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > tempp4(curmu.p4());
			//std::cout<<"pfpt "<<curmu.pfP4().pt()<<std::endl;
			//std::cout<<"pt "<<tempp4.pt()<<std::endl;
			tempp4.SetPhi(normalizedPhi(curmu.p4().Phi()+dphij));
			tempp4.SetEta(curmu.p4().Eta()+detaj);

			ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > pxyzep4;
			pxyzep4.SetPxPyPzE(tempp4.Px(),tempp4.Py(),tempp4.Pz(),tempp4.E());
			tempmu.setP4(pxyzep4);
			tempmu.setPFP4(pxyzep4);

			//else if (curmu.innerTrack().isNonnull())std::cout<<"ins "<<curmu.innerTrack().get()->eta()<<","<<curmu.innerTrack().key()<<std::endl;
			//else if (curmu.outerTrack().isNonnull())std::cout<<"outs "<<curmu.outerTrack().get()->eta()<<","<<curmu.outerTrack().key()<<std::endl;



			/*if(muon::isLooseMuon(curmu))
				{
				//std::cout<<"DR "<<reco::deltaR(tempmu.p4(),wjp4)<<std::endl;
				std::cout<<" TRUE MUON phi "<<curmu.p4().phi()<<" eta "<<curmu.p4().eta()<<std::endl;
				std::cout<<" ROT MUON phi "<<tempmu.p4().phi()<<" eta "<<tempmu.p4().eta()<<std::endl;
				//std::cout<<"mutrackrefs.size() "<<mutrackrefs.size()<<" nlmu "<<nlmu<<std::endl;

				
				if(mutrackrefs.size()>nlmu)
					{
					
					std::cout<<"Found Track"<<std::endl;
					tempmu.setInnerTrack(mutrackrefs[nlmu]);
	 				tempmu.setTrack(mutrackrefs[nlmu]);
					tempmu.setGlobalTrack(mutrackrefs[nlmu]);
					}
				else
					{
					std::cout<<"WARNING NO TRACK"<<std::endl;
					std::cout<<"WARNING NO TRACK"<<std::endl;
					std::cout<<"WARNING NO TRACK"<<std::endl;
					}
				nlmu+=1;
				}
			*/
			//std::cout<<"mu "<<tempmu.eta()<<std::endl;
			newmus.push_back(tempmu);
			nmusrot+=1;
			}

  		auto outputsnewtracks = std::make_unique<std::vector<reco::Track>>(newtracks);
  		iEvent.put(std::move(outputsnewtracks),"newtracks");
		//for(auto globmukey : rekeyvecglobmu) 
		//	{
		//	std::cout<<"writing new tracks"<<std::endl;
		//	newmus[globmukey.first].setInnerTrack(reco::TrackRef(trorph,globmukey.second));
		//	newmus[globmukey.first].setTrack(reco::TrackRef(trorph,globmukey.second));
		//	newmus[globmukey.first].setGlobalTrack(reco::TrackRef(trorph,globmukey.second));
		//	}
		//std::vector<reco::TrackRef>mutrackrefs={};
		//for(const auto trindex : trindices) mutrackrefs.push_back(reco::TrackRef(trorph,trindex));

		//std::cout<<"----"<<std::endl;
		
		//std::cout<<"nmusrot "<<nmusrot<<std::endl;
		//std::cout<<"newmussize "<<newmus.size()<<std::endl;
		//std::cout<<"newelssize "<<newels.size()<<std::endl;
  		nelsrot=0;
		for(const auto curel : allelsbf)
			{
			nelsrot+=1;

			pat::Electron tempel = curel;

			ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > tempp4(tempel.p4());
			tempp4.SetPhi(normalizedPhi(tempel.p4().Phi()+dphij));
			tempp4.SetEta(tempel.p4().Eta()+detaj);
			ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > pxyzep4;
			pxyzep4.SetPxPyPzE(tempp4.Px(),tempp4.Py(),tempp4.Pz(),tempp4.E());
			tempel.setP4(pxyzep4);
			
 			auto trvec =  tempel.gsfTrack().get();
			math::XYZPoint tpoint1(trvec->referencePoint().x()+dx, trvec->referencePoint().y()+dy, trvec->referencePoint().z()+dz);
			math::XYZPoint tpoint = protate(tpoint1,newopv[0],dthetaj,dphij);


			math::XYZVector momVector(pxyzep4.px(),pxyzep4.py(),pxyzep4.pz());
 			reco::GsfElectronCore gsfcore = *(tempel.core().get());
			gsftracks.push_back(reco::GsfTrack(trvec->chi2(), trvec->ndof(), tpoint, momVector, trvec->charge(), trvec->covariance()));
			gsfcores.push_back(gsfcore);

			gsfcore.setGsfTrack(edm::Ref(&gsftracks,gsftracks.size()-1));
			reco::GsfElectron curgsfel(edm::Ref(&gsfcores,gsfcores.size()-1));
			pat::Electron curelcored(curgsfel);
			newels.push_back(curelcored);
			//std::cout<<"---------------------"<<trvec->etaMode()<<std::endl;
			//std::cout<<"gsfeta "<<trvec->etaMode()<<std::endl;
			//std::cout<<"newgsfeta "<<gsftracks.back().etaMode()<<std::endl;
			//std::cout<<"curelcored "<<curelcored.gsfTrack().get()->etaMode()<<std::endl;
			//std::cout<<"p4eta "<<tempel.p4().eta()<<std::endl;
			//std::cout<<"---------------------"<<trvec->etaMode()<<std::endl;

			//std::cout<<"DR "<<reco::deltaR(tempel.p4(),wjp4)<<std::endl;
			}
  	
	  	auto outputsgsftracks = std::make_unique<std::vector<reco::GsfTrack>>(gsftracks);
	  	auto gsforhand = iEvent.put(std::move(outputsgsftracks),"gsftracks");
		for(uint igsf=0;igsf<gsfcores.size();igsf++)
			{
			gsfcores[igsf].setGsfTrack(edm::Ref(gsforhand,igsf));
			}
	  	auto outputsgsfcores = std::make_unique<std::vector<reco::GsfElectronCore>>(gsfcores);
	  	auto gsfcoreorhand = iEvent.put(std::move(outputsgsfcores),"gsfcores");
		for(uint igsf=0;igsf<gsfcores.size();igsf++)
			{
			reco::GsfElectron curgsfel(edm::Ref(gsfcoreorhand,igsf));
			pat::Electron curelcored(curgsfel);
			newels.push_back(curelcored);
			}
		//std::cout<<"nelsrot "<<nelsrot<<std::endl;
		//std::cout<<"newelssize "<<newels.size()<<std::endl;
		//std::cout<<"newvecssize "<<newvecs.size()<<std::endl;
		

  		if(isfmerge)
			{
			//std::cout<<"Adding in ungroomed stuff"<<std::endl;
			//std::cout<<"presize "<<allpf.size()<<std::endl;
			//std::cout<<"groomsize "<<allpfg.size()<<std::endl;
			allpf.insert(std::end(allpf), std::begin(allpfg), std::end(allpfg));
			//std::cout<<"postsize "<<allpf.size()<<std::endl;
			}

		}
	else std::cout<<"emptyfileevent! Tries "<<tries<<" of "<<maxtries<<std::endl;

	}

	uint ndau = AK8pfjet.numberOfDaughters();
	if (mergeb)  ndau = AK4pfjet.numberOfDaughters();
	//std::cout<<"curW len "<<ndau<<std::endl;
	for(uint idau=0;idau<ndau;idau++)
		{
		//std::cout<<idau<<ndau<<std::endl;
		if (merge) 
			{	
			auto newpackp4 = allpfsj[idau].polarP4();
			pat::PackedCandidate newpack = allpfsj[idau];
			newpackp4 = fakefac*newpackp4;
			newpack.setP4(newpackp4);
			allpf.push_back(newpack);
				
			}
		else if (mergeb) 
			{
			auto newpackp4 = allpfsjb[idau].polarP4();
			pat::PackedCandidate newpack = allpfsjb[idau];
			newpackp4 = fakefac*newpackp4;
			newpack.setP4(newpackp4);
			allpf.push_back(newpack);
			}
		else 
			{
			pat::PackedCandidate newpack = allpfsj[idau];
			allpf.push_back(newpack);
			}
		}
	//std::cout<<"a"<<std::endl;
	alljets.push_back(AK8pfjet);
	//std::cout<<"b"<<std::endl;
	std::vector<pat::Muon>allmus = {};
	std::vector<pat::Electron>allels = {};
	for(const auto &curmu:*muons)
		{
		if (reco::deltaR(curmu.p4(),AK8pfjet)<0.4)allmus.push_back(curmu);
		}
	for(const auto &curel:*electrons)
		{
		if (reco::deltaR(curel.p4(),AK8pfjet)<0.4)allels.push_back(curel);
		}
	//std::cout<<"c"<<std::endl;
        TLorentzVector curtlv;
	curtlv.SetPtEtaPhiM(AK8pfjet.pt(),AK8pfjet.eta(),AK8pfjet.phi(),AK8pfjet.mass());
        TLorentzVector curtlvb; 
	curtlvb.SetPtEtaPhiM(AK4pfjet.pt(),AK4pfjet.eta(),AK4pfjet.phi(),AK4pfjet.mass());
	std::pair<int,float> matched(0,-10.0);
  	if(isfmerge) matched = signalmatch(curtlv, curtlvb,genpartsvec,"wb",1.4);	
	else matched = signalmatch(curtlv, curtlvb,genpartsvec,"w",1.2);
	//std::cout <<"matched "<<matched.first<<","<<matched.second<<std::endl;//
		
	wmatch.push_back(matched.first);
	wmatchdr.push_back(matched.second);


	//std::cout<<"matched.first "<<matched.first<<" matched.second "<<matched.second<<std::endl;


	bjettopush.push_back(AK4pfjet);
	//uint ndaub = (bjettopush[0]).numberOfDaughters();

	std::vector<pat::Muon>allmusb = {};
	std::vector<pat::Electron>allelsb = {};
	for(const auto &curmu:*muons)
		{
		if (reco::deltaR(curmu.p4(),AK4pfjet)<0.4)allmusb.push_back(curmu);
		}
	for(const auto &curel:*electrons)
		{
		if (reco::deltaR(curel.p4(),AK4pfjet)<0.4)allelsb.push_back(curel);
		}
	if(merge or mergeb or mergemj) 
		{
		matchvals[13] = matched.first;
		matchvals[14] = matched.second;
		matchvals[15] = float(isfmerge);
		matchvals[16] = float(nelsrot);
		matchvals[17] = float(nmusrot);
		}

	allpfb = allpfsjb;
	//for(uint idau=0;idau<ndaub;idau++)
	//	{
	//	std::cout<<"pfb " << allpfb[idau].pt()<<std::endl;						
	//	}
  //for (const auto cpf : allpf)
				//{
				//std::cout<<" phi:"<<cpf.phi()<<" eta:"<<cpf.eta()<<"  TR:"<<cpf.phiAtVtx()<<","<<cpf.etaAtVtx()<<std::endl;
    				//const reco::RecoChargedRefCandidate* trkCand = dynamic_cast <const reco::RecoChargedRefCandidate*> (&cpf);
				//std::cout<<"  TR:"<<trkCand->track().px()<<std::endl;
				//}
  //Add to jet userfloats
  //std::cout<<"bjs "<<bjettopush.size()<<std::endl;
  if(merge_ or mergeb_ or mergemj_)
	{
	//std::cout<<"writeallpf"<<std::endl;
  	auto outputs = std::make_unique<std::vector<pat::PackedCandidate>>(allpf);
  	auto orhand = iEvent.put(std::move(outputs),"");
	//std::cout<<"writealljet"<<std::endl;
  	auto outputsjet = std::make_unique<std::vector<pat::Jet>>(alljets);
  	iEvent.put(std::move(outputsjet),"jet");
	//std::cout<<"writewmatch"<<std::endl;
  	auto outputssjmerger = std::make_unique<std::vector<float>>(sjmerger);
  	iEvent.put(std::move(outputssjmerger),"sjmerger");
	
  	auto outputsrekeyvec2 = std::make_unique<std::vector<std::pair<int,int>>>(rekeyvec2);
  	iEvent.put(std::move(outputsrekeyvec2),"secvertrekey");

  	//auto outputselectronrekey = std::make_unique<std::vector<std::pair<int,int>>>(electronrekey);
  	//iEvent.put(std::move(outputselectronrekey),"electronrekey");

  	//auto outputsmuonrekey = std::make_unique<std::vector<std::pair<int,int>>>(muonrekey);
  	//iEvent.put(std::move(outputsmuonrekey),"muonrekey");

  	auto outputsnewvecs = std::make_unique<reco::VertexCompositePtrCandidateCollection>(newvecs);
  	iEvent.put(std::move(outputsnewvecs),"newvecs");

  	auto outputsnewels = std::make_unique<std::vector<pat::Electron>>(newels);
  	iEvent.put(std::move(outputsnewels),"newels");

  	auto outputsnewmus = std::make_unique<std::vector<pat::Muon>>(newmus);
  	iEvent.put(std::move(outputsnewmus),"newmus");



	for(int ival=0;ival<nvals;ival++)
		{
  		auto outputsmatchvals = std::make_unique<float>(matchvals[ival]);
  		iEvent.put(std::move(outputsmatchvals),"matchvals"+std::to_string(ival));
		}
	}
  else
	{
	//std::cout<<"writing ismerge "<<isfmerge<<std::endl;
	//std::cout<<"lenWwrite "<<allpf.size()<<std::endl;
  	auto outputs = std::make_unique<std::vector<pat::PackedCandidate>>(allpf);
  	iEvent.put(std::move(outputs),"wcands");
	//std::cout<<"lenBwrite "<<allpfb.size()<<std::endl;
  	auto outputsbtag = std::make_unique<std::vector<pat::PackedCandidate>>(allpfb);
  	iEvent.put(std::move(outputsbtag),"bcands");
  	auto outputsjet = std::make_unique<std::vector<pat::Jet>>(alljets);
  	iEvent.put(std::move(outputsjet),"wjet");
  	auto outputsbjet = std::make_unique<std::vector<pat::Jet>>(bjettopush);
  	iEvent.put(std::move(outputsbjet),"bjet");
	//std::cout<<"startv "<<std::endl;	
  	auto outputsallmus = std::make_unique<std::vector<pat::Muon>>(allmus);
  	iEvent.put(std::move(outputsallmus),"allmus");
  	auto outputsallels = std::make_unique<std::vector<pat::Electron>>(allels);
  	iEvent.put(std::move(outputsallels),"allels");
  	auto outputsallmusb = std::make_unique<std::vector<pat::Muon>>(allmusb);
  	iEvent.put(std::move(outputsallmusb),"allmusb");
  	auto outputsallelsb = std::make_unique<std::vector<pat::Electron>>(allelsb);
  	iEvent.put(std::move(outputsallelsb),"allelsb");
  	auto outputswmatchdr = std::make_unique<std::vector<float>>(wmatchdr);
  	iEvent.put(std::move(outputswmatchdr),"wmatchdr");		
  	auto outputswmatch = std::make_unique<std::vector<int>>(wmatch);
  	iEvent.put(std::move(outputswmatch),"wmatch");		
  	auto outputssjw = std::make_unique<std::vector<float>>(sjlistw);
  	iEvent.put(std::move(outputssjw),"sjw");	
  	auto outputssjb = std::make_unique<std::vector<float>>(sjlistb);
  	iEvent.put(std::move(outputssjb),"sjb");	
  	auto outputswvecs = std::make_unique<reco::VertexCompositePtrCandidateCollection>(allvecs);
  	iEvent.put(std::move(outputswvecs),"wvecs");	
  	auto outputsbvecs = std::make_unique<reco::VertexCompositePtrCandidateCollection>(allvecsb);
  	iEvent.put(std::move(outputsbvecs),"bvecs");
  	auto outputsnewopv = std::make_unique<std::vector<reco::Vertex>>(newopv);
  	iEvent.put(std::move(outputsnewopv),"newopv");
  	auto outputsismerge = std::make_unique<std::vector<bool>>(isfmergev);
  	iEvent.put(std::move(outputsismerge),"ismerge");	
	}
}

//define this as a plug-in
DEFINE_FWK_MODULE(WtagProducer);
