#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "TLorentzVector.h"

#include <iostream>
#include <fstream>

#include <memory>
#include <iostream>


class  NanoAODFilterSlep : public edm::stream::EDFilter<> {
public:
  int windex;
  int tindex;
  int bindex;
  int mindex;
  explicit NanoAODFilterSlep( const edm::ParameterSet & );   

private:

  bool ISDATA_ ;
  bool merge_;
  const edm::EDGetTokenT<edm::View<pat::Jet>> srcAK8_;
  const edm::EDGetTokenT<edm::View<pat::Jet>> srcAK4_;
  const edm::EDGetTokenT<edm::View<pat::MET>> srcmet_;
  const edm::EDGetTokenT<edm::View<pat::Muon>> srcmu_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxl_;
  const edm::EDGetTokenT<edm::View<pat::Electron>> srcele_;
  bool filter( edm::Event &, const edm::EventSetup & );
  void beginRun(edm::Run const&, edm::EventSetup const&);
  void beginJob() ;
  void endJob() ;
 };


NanoAODFilterSlep::NanoAODFilterSlep(const edm::ParameterSet& iConfig):
   ISDATA_ (iConfig.getUntrackedParameter<bool>("ISDATA", false))
, merge_(iConfig.getParameter<bool>("merge"))
, srcAK8_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcAK8")))
, srcAK4_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcAK4")))
, srcmet_(consumes<edm::View<pat::MET>>(iConfig.getParameter<edm::InputTag>("srcmet")))
, srcmu_(consumes<edm::View<pat::Muon>>(iConfig.getParameter<edm::InputTag>("srcmu")))
 {   
  vtxl_ = consumes<std::vector<reco::Vertex>>(edm::InputTag("offlineSlimmedPrimaryVertices"));
  produces<int>("windex");
  produces<int>("tindex");
  produces<int>("bindex");
  produces<int>("mindex");
 }


bool NanoAODFilterSlep::filter( edm::Event& iEvent, const edm::EventSetup& iSetup) {


  /*
  uint evtum = iEvent.id().event();
  if(merge_) 
		{
		if(evtum%2==0)return false;			
		}
  else
		{
		if(evtum%2!=0)return false;
		}
  */
  //std::cout<<"glorp"<<std::endl;

  bool foundmu = false;
  int muindex = 999;




  edm::Handle<edm::View<pat::Muon>> mus;
  iEvent.getByToken(srcmu_, mus);



  edm::Handle<std::vector<reco::Vertex>> vtx;
  iEvent.getByToken(vtxl_, vtx);
  const std::vector<reco::Vertex>* vtxvec  = vtx.product();
  int curmuindex = 0;
  for (const auto &mu : *mus)
	{
        float isol = (mu.pfIsolationR04().sumChargedHadronPt + std::max(0., mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt))/mu.pt();
	//std::cout<<"isol "<<isol<<std::endl;
	if(mu.pt()>70.0 and mu.isHighPtMuon(vtxvec->at(0)) and isol<0.15)
		{
		muindex = curmuindex;
		foundmu=true;
		}
	curmuindex+=1;
	}
  if(not foundmu) return false;

  edm::Handle<edm::View<pat::MET>> met;
  iEvent.getByToken(srcmet_, met);
  //for (const auto &mm : *met)
	//{
	//std::cout<<mm.corPt()<<std::endl;
	//}
  if (met->at(0).corPt()<40.0) return false;
  double metphi = met->at(0).corPhi();
  //std::cout<<"PREmu"<<std::endl;



  edm::Handle<edm::View<pat::Jet>> jetsAK4;
  iEvent.getByToken(srcAK4_, jetsAK4);



  std::vector<int> bindices;
  int ak4index = 0;
  
  for (const auto &AK4pfjet : *jetsAK4)
	{

	if (AK4pfjet.pt()>70.0)
		{
 
		//float flavdisc = AK4pfjet.bDiscriminator("pfDeepCSVJetTags:probb")+AK4pfjet.bDiscriminator("pfDeepCSVJetTags:probbb");
		float flavdisc = AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probb") + AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:probbb") + AK4pfjet.bDiscriminator("pfDeepFlavourJetTags:problepb");
		//if(flavdisc>0.1241)
		// std::cout<<"flavdisc "<<flavdisc<<std::endl;
		if(flavdisc>0.2770)
			{
			//std::cout<<"bpts "<<AK4pfjet.pt()<<std::endl;
			bindices.push_back(ak4index);
			}
		}
	ak4index+=1;
	}
  //std::cout<<"PREbs"<<std::endl;
  if(bindices.size()<1) return false;


  edm::Handle<edm::View<pat::Jet>> jetsAK8;
  iEvent.getByToken(srcAK8_, jetsAK8);

  //std::cout<<"nak8 "<<jetsAK8->size()<<std::endl;
  int iak8=0;
  //bool foundW = false;
  for (const auto &AK8pfjet : *jetsAK8)
	{
	//and  (AK8pfjet.userFloat("NjettinessAK8Puppi:tau2")/AK8pfjet.userFloat("NjettinessAK8Puppi:tau1")<0.45) 
	//float tau21=1.0;
	//if(AK8pfjet.userFloat("NjettinessAK8Puppi:tau1")>0.0) tau21=AK8pfjet.userFloat("NjettinessAK8Puppi:tau2")/AK8pfjet.userFloat("NjettinessAK8Puppi:tau1");
	if(AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass")>65.0 and AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass")<105.0 and AK8pfjet.pt()>250.0) 
		{
		int nbs=0;
		for(uint bind=0;bind<bindices.size();bind++)
			{
			if ((reco::deltaR( AK8pfjet.p4(),jetsAK4->at(bind).p4())>1.0) and (reco::deltaR( mus->at(muindex).p4(),jetsAK4->at(bind).p4())>0.4)) 
				{
				nbs+=1;
				//std::cout<<"abs "<<fabs(reco::deltaPhi(metphi,AK8pfjet.p4().phi()))<<std::endl;
				if ((reco::deltaR( AK8pfjet.p4(),mus->at(muindex).p4())>1.7) and nbs>=1 and fabs(reco::deltaPhi(metphi,AK8pfjet.p4().phi()))>1.7) 
					{
					windex=iak8;
					tindex=-1;
					bindex=bind;
					mindex=curmuindex;
					auto outputsw = std::make_unique<int>(windex);
					iEvent.put(std::move(outputsw),"windex");
					auto outputst = std::make_unique<int>(tindex);
					iEvent.put(std::move(outputst),"tindex");
					auto outputsb = std::make_unique<int>(bindex);
					iEvent.put(std::move(outputsb),"bindex");
					auto outputsm = std::make_unique<int>(mindex);
					iEvent.put(std::move(outputsm),"mindex");
					//std::cout<<"windex "<<windex<<std::endl;
					//std::cout<<"bindex "<<bindex<<std::endl;
					//std::cout<<"PASS"<<std::endl;
					return false;
					}
				}
			}
		}

	else if(AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass")>150.0 and AK8pfjet.pt()>400.0) 	
		{
		int nbs=0;
		for(uint bind=0;bind<bindices.size();bind++)
			{
			if ((reco::deltaR( AK8pfjet.p4(),jetsAK4->at(bind).p4())>1.2) and (reco::deltaR( mus->at(muindex).p4(),jetsAK4->at(bind).p4())>0.4)) 
				{
				nbs+=1;
				//std::cout<<"abs "<<fabs(reco::deltaPhi(metphi,AK8pfjet.p4().phi()))<<std::endl;
				if ((reco::deltaR( AK8pfjet.p4(),mus->at(muindex).p4())>1.7) and nbs>=1 and fabs(reco::deltaPhi(metphi,AK8pfjet.p4().phi()))>1.7) 
					{
					windex=-1;
					tindex=iak8;
					bindex=bind;
					mindex=curmuindex;
					auto outputsw = std::make_unique<int>(windex);
					iEvent.put(std::move(outputsw),"windex");
					auto outputst = std::make_unique<int>(tindex);
					iEvent.put(std::move(outputst),"tindex");
					auto outputsb = std::make_unique<int>(bindex);
					iEvent.put(std::move(outputsb),"bindex");
					auto outputsm = std::make_unique<int>(mindex);
					iEvent.put(std::move(outputsm),"mindex");
					//std::cout<<"windex "<<windex<<std::endl;
					//std::cout<<"bindex "<<bindex<<std::endl;
					//std::cout<<"PASS mt"<<std::endl;
					return true;
					}
				}
			}
		}
	iak8+=1;
	}
   return false;
 }




void 
NanoAODFilterSlep::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
 

		
}



// ------------ method called once each job just before starting event loop  ------------
void 
NanoAODFilterSlep::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NanoAODFilterSlep::endJob() 
{

}

#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(NanoAODFilterSlep);



