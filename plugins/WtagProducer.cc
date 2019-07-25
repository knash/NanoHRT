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
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

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

    void beginStream(edm::StreamID) override {}
    void produce(edm::Event&, const edm::EventSetup&) override;
    void endStream() override {}
    int nevmix;
    const edm::EDGetTokenT<edm::View<pat::Jet>> src_;
    const edm::EDGetTokenT<edm::View<pat::Jet>> srcAK4_;
    bool merge_;
    bool mergeb_;
    edm::EDGetTokenT<int> windex_;
    edm::EDGetTokenT<int> bindex_;
    uint pnum_;
    uint tnum_;
    TFile* inputTFile_;
    TH1F* topmass_;
    TH1F* wwmass_;
    //fwlite::Event* ev_;
    std::atomic<fwlite::ChainEvent*> ev_;
    edm::EDGetTokenT<std::vector<reco::GenParticle>> gplab ;


};


WtagProducer::WtagProducer(const edm::ParameterSet& iConfig)
: 
src_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("src")))
, srcAK4_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcAK4")))
, merge_(iConfig.getParameter<bool>("merge"))
, mergeb_(iConfig.getParameter<bool>("mergeb"))
, pnum_(iConfig.getParameter<uint>("pnum"))
, tnum_(iConfig.getParameter<uint>("tnum"))
//pfcs_(consumes<edm::View<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("pfcs")))
{
  if(merge_ or mergeb_)
	{
  	produces<std::vector<pat::PackedCandidate>>("");
  	produces<std::vector<pat::Jet>>("jet");
  	produces<std::vector<int>>("wmatch");
	}
  else
	{
  	produces<std::vector<pat::PackedCandidate>>("wcands");
  	produces<std::vector<pat::PackedCandidate>>("bcands");
  	produces<std::vector<pat::Jet>>("wjet");
  	produces<std::vector<pat::Jet>>("bjet");
  	produces<std::vector<int>>("wmatch");
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
	myfile.open("fnames.txt");
	while(getline(myfile, line))fnames.push_back(line);
	ev_ =  new fwlite::ChainEvent(fnames);
	nevmix=ev_.load()->size();
	std::cout<<"mixing "<<nevmix<<" events";
	}
  //windex_ = consumes<edm::View<int>>(edm::InputTag("windex","NanoAODFilterSlep"));
  //bindex_ = consumes<edm::View<int>>(edm::InputTag("bindex","NanoAODFilterSlep"));
  windex_ = consumes<int>(edm::InputTag("NanoAODFilterSlep","windex"));
  bindex_ = consumes<int>(edm::InputTag("NanoAODFilterSlep","bindex"));
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
  std::cout<<"1"<<std::endl;
  edm::Handle<edm::View<pat::Jet>>jets;
  edm::Handle<int>windex;
  edm::Handle<int>bindex;
  edm::Handle<edm::View<pat::PackedCandidate>>PackedCandidates;
  iEvent.getByToken(src_, jets);
  iEvent.getByToken(windex_, windex);
  iEvent.getByToken(bindex_, bindex);  
  int iwindex=*(windex.product());
  int ibindex=*(bindex.product());
  std::cout<<iwindex<<"   --  "<<ibindex<<std::endl;

  //iEvent.getByToken(pfcs_, PackedCandidates);
  edm::Handle<edm::View<pat::Jet>> jetsAK4;
  iEvent.getByToken(srcAK4_, jetsAK4);

  std::vector<pat::PackedCandidate> allpf;
  std::vector<pat::PackedCandidate> allpfb;
  std::vector<pat::Jet> alljets;
  std::vector<pat::Jet> bjettopush;
  std::vector<int> wmatch;
  int evtoload = 0;


  edm::Handle<std::vector<reco::GenParticle>> genparts;
  iEvent.getByToken(gplab, genparts);
  const std::vector<reco::GenParticle>* genpartsvec  = genparts.product();

  bool merge=merge_;
  bool mergeb=mergeb_;
  std::cout<<"2"<<std::endl;
  if(merge or mergeb)
	{
  	std::cout<<"tnum "<<tnum_<<" pnum "<<pnum_<<std::endl;

	bool foundev=false;
	while(foundev==false)
		{
 		evtoload=std::rand()%nevmix;
		if(evtoload%tnum_==(pnum_-1))foundev=true;
		}
  	std::cout<<"evtoload "<<evtoload<<std::endl;
	}



  float fakefac=1.0;
  std::cout<<"nak81 "<<jets->size()<<std::endl;
  auto AK8pfjet = jets->at(iwindex);
  std::cout<<"AK8 mass "<<AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass")<<std::endl;
    std::cout<<"3"<<std::endl;
  //std::vector<fwlite::Event>::iterator curev = ev_.load()->toBegin() + evtoload;
  if(merge or mergeb)
	{
	//std::cout<<"Events to mix "<<nevmix<<std::endl;
	ev_.load()->toBegin();	
	for(int temp=0;temp<(evtoload-1);temp++)++(*ev_);
	edm::Handle<std::vector<pat::Jet>> Wjets;
	edm::Handle<std::vector<pat::PackedCandidate>> Wjetspfc;
	float DRmin=0.0;
	float DRmax=0.0;
    std::cout<<"4"<<std::endl;
	if(merge)
			{
			//std::cout<<"Load W"<<std::endl;
	  		ev_.load()->getByLabel(edm::InputTag("WtagProducerpuppi","wjet"), Wjets);
	  		ev_.load()->getByLabel(edm::InputTag("WtagProducerpuppi","wcands"), Wjetspfc);
			DRmin=0.0;
			DRmax=0.4;
			}
	if(mergeb)
			{
			//std::cout<<"Load B"<<std::endl;
	  		ev_.load()->getByLabel(edm::InputTag("WtagProducerpuppi","bjet"), Wjets);
	  		ev_.load()->getByLabel(edm::InputTag("WtagProducerpuppi","bcands"), Wjetspfc);
			DRmin=0.5;
			DRmax=0.8;
			}
    std::cout<<"5"<<std::endl;
	auto Wjetsprod = Wjets.product();
	auto Wjetspfcprod = Wjetspfc.product();
	if (Wjetsprod->size()>0)
		{
    std::cout<<"6"<<std::endl;
		float DRt = DRmin + (DRmax-DRmin)*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		float ang = 2*3.1415*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			
			
		float detaval = DRt*std::sin(ang);
		float dphival = DRt*std::cos(ang);
		ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > wjp4((Wjetsprod->at(0).p4()));
		wjp4.SetPhi(AK8pfjet.phi()+dphival);
		wjp4.SetEta(AK8pfjet.eta()+detaval);
		if(mergeb)
			{
			std::cout<<"ptW "<<AK8pfjet.p4().pt()<<" ptb "<<wjp4.pt()<<std::endl;
			std::cout<<"wbmass "<<(AK8pfjet.p4()+wjp4).mass()<<std::endl;
			}
		float randomass = 0.0;
		if(mergeb)randomass=topmass_->GetRandom();
		if(merge)randomass=wwmass_->GetRandom();
		std::cout<<"randomass "<<randomass<<std::endl;
		fakefac=1.0;//1.35*172.5*randomass/(AK8pfjet.p4()+wjp4).mass();
		for(uint idau=0;idau<Wjetspfcprod->size();idau++)
			{
				pat::PackedCandidate lPack = Wjetspfcprod->at(idau);
				ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > packp4(lPack.p4());


				//std::cout<<"PHI? "<<(reco::deltaPhi(Wjetsprod->at(0).p4(),packp4))<<std::endl;
				//std::cout<<"OLDPACK "<<lPack.phi()<<","<<lPack.eta()<<"  "<<lPack.phiAtVtx()<<","<<lPack.etaAtVtx()<<std::endl;
				//std::cout<<"DRpre "<<reco::deltaR(lPack.p4(),AK8pfjet.p4())<<std::endl;
				//std::cout<<"Dphifromw "<<reco::deltaPhi(lPack.p4().phi(),Wjetsprod->at(0).p4().phi())<<std::endl;
				//std::cout<<"Detafromw "<<Wjetsprod->at(0).p4().eta()-packp4.eta()<<std::endl;
				//std::cout<<"DRfromw "<<reco::deltaR(Wjetsprod->at(0).p4(),wjp4)<<std::endl;
				packp4.SetPhi(wjp4.phi()-(reco::deltaPhi(Wjetsprod->at(0).p4(),packp4)));
				packp4.SetEta(wjp4.eta()-(Wjetsprod->at(0).p4().eta()-packp4.eta()));
				//packp4.SetPt(Wjetsprod->at(0).p4().pt()*fakefac);
				//packp4.SetM(Wjetsprod->at(0).p4().mass()*fakefac);
				packp4 = fakefac*packp4;
  						

  				pat::PackedCandidate newpack(packp4, lPack.vertex(),packp4.pt(), packp4.eta(), packp4.phi(),lPack.pdgId(),reco::VertexRefProd(lPack.vertexRef().id(),lPack.vertexRef().productGetter()),lPack.vertexRef().key());
				//lPack.setP4(packp4);
				//std::cout<<"NEWPACK "<<newpack.phi()<<","<<newpack.eta()<<"  "<<newpack.phiAtVtx()<<","<<newpack.etaAtVtx()<<std::endl;
				//std::cout<<"DRpost "<<reco::deltaR(newpack.p4(),AK8pfjet.p4())<<std::endl;
				//std::cout<<"Dphifromwpost "<<reco::deltaPhi(newpack.p4().phi(),wjp4.phi())<<std::endl;
				//std::cout<<"Detafromwpost "<<wjp4.eta()-packp4.eta()<<std::endl;
				//std::cout<<"DRfromwpost "<<reco::deltaR(newpack.p4(),wjp4)<<std::endl;
				allpf.push_back(newpack);
						
			  }
		}
	else std::cout<<"emptyfileevent!"<<std::endl;
    std::cout<<"7"<<std::endl;
	}
	int ndau = AK8pfjet.numberOfDaughters();
	//std::cout<<"foundtheW "<<std::endl;
	for(int idau=0;idau<ndau;idau++)
		{

		const pat::PackedCandidate * lPack = dynamic_cast<const pat::PackedCandidate *>((jets->at(iwindex)).daughter(idau) );

		if (lPack != nullptr) 
			{
			if (merge or mergeb) 
				{		

				pat::PackedCandidate newpack = (*lPack);
				auto newpackp4 = lPack->polarP4();
				//packp4.SetM(Wjetsprod->at(0).p4().mass()*fakefac);
				newpackp4 = fakefac*newpackp4;

				newpack.setP4(newpackp4);
				//std::cout<<"DRpost1 "<<reco::deltaR(newpack.p4(),AK8pfjet.p4())<<std::endl;
				allpf.push_back(newpack);
				}
			else allpf.push_back(*lPack);
					
		  	}
		}
		
	alljets.push_back(AK8pfjet);
        TLorentzVector curtlv;
	curtlv.SetPtEtaPhiM(AK8pfjet.pt(),AK8pfjet.eta(),AK8pfjet.phi(),AK8pfjet.mass());
	std::pair<int,float> matched(0,-10.0);
	matched = signalmatch(curtlv, genpartsvec,0.8);			
	wmatch.push_back(matched.first);
	std::cout <<"matched "<<matched.first<<","<<matched.second<<std::endl;
			

	bjettopush.push_back(jetsAK4->at(ibindex));
	std::cout <<"bjetpt "<<jetsAK4->at(ibindex).pt()<<std::endl;
	int ndaub = (bjettopush[0]).numberOfDaughters();
	//std::cout <<"ndaub "<<ndaub<<std::endl;
	for(int idau=0;idau<ndaub;idau++)
		{
		const pat::PackedCandidate * lPack = dynamic_cast<const pat::PackedCandidate *>((jetsAK4->at(ibindex)).daughter(idau) );

		if (lPack != nullptr) allpfb.push_back(*lPack);
		else std::cout<<"NULL"<<std::endl;					
			
		}
	
	
  //for (const auto cpf : allpf)
				//{
				//std::cout<<" phi:"<<cpf.phi()<<" eta:"<<cpf.eta()<<"  TR:"<<cpf.phiAtVtx()<<","<<cpf.etaAtVtx()<<std::endl;
    				//const reco::RecoChargedRefCandidate* trkCand = dynamic_cast <const reco::RecoChargedRefCandidate*> (&cpf);
				//std::cout<<"  TR:"<<trkCand->track().px()<<std::endl;
				//}
  //Add to jet userfloats
  std::cout<<"bjs "<<bjettopush.size()<<std::endl;
  if(merge_ or mergeb_)
	{
  	auto outputs = std::make_unique<std::vector<pat::PackedCandidate>>(allpf);
  	iEvent.put(std::move(outputs),"");
  	auto outputsjet = std::make_unique<std::vector<pat::Jet>>(alljets);
  	iEvent.put(std::move(outputsjet),"jet");
	}
  else
	{
  	auto outputs = std::make_unique<std::vector<pat::PackedCandidate>>(allpf);
  	iEvent.put(std::move(outputs),"wcands");
  	auto outputsbtag = std::make_unique<std::vector<pat::PackedCandidate>>(allpfb);
  	iEvent.put(std::move(outputsbtag),"bcands");
  	auto outputsjet = std::make_unique<std::vector<pat::Jet>>(alljets);
  	iEvent.put(std::move(outputsjet),"wjet");
  	auto outputsbjet = std::make_unique<std::vector<pat::Jet>>(bjettopush);
  	iEvent.put(std::move(outputsbjet),"bjet");
  	auto outputswmatch = std::make_unique<std::vector<int>>(wmatch);
  	iEvent.put(std::move(outputswmatch),"wmatch");
	}




}

//define this as a plug-in
DEFINE_FWK_MODULE(WtagProducer);
