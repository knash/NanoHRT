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
#include "DataFormats/Math/interface/LorentzVector.h"


#include <iostream>
#include <fstream>

#include <memory>
#include <iostream>
#include <Python.h>


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
  const edm::EDGetTokenT<edm::View<pat::Electron>> srcel_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxl_;
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
, srcel_(consumes<edm::View<pat::Electron>>(iConfig.getParameter<edm::InputTag>("srcel")))
 {   
  vtxl_ = consumes<std::vector<reco::Vertex>>(edm::InputTag("offlineSlimmedPrimaryVertices"));
 }


bool NanoAODFilterSlep::filter( edm::Event& iEvent, const edm::EventSetup& iSetup) {


  bool foundmu = false;





  edm::Handle<edm::View<pat::Muon>> mus;
  iEvent.getByToken(srcmu_, mus);

  math::PtEtaPhiMLorentzVector lepp4;

  int curmuindex = 0;
  for (const auto &mu : *mus)
	{
	//std::cout<<"mu pt "<<mu.pt()<<" idval "<<mu.isMediumMuon()<<std::endl;
	if(mu.pt()>60.0 and mu.isMediumMuon() and fabs(mu.eta())<2.4)
		{
		foundmu=true;
		lepp4 = mu.p4();
		break;
		}
	curmuindex+=1;
	}



  bool foundel = false;



  edm::Handle<edm::View<pat::Electron>> els;
  iEvent.getByToken(srcel_, els);

  for (const auto &el : *els)
	{
	//std::cout<<"el pt "<<el.pt()<<" idval "<<el.electronID("mvaEleID-Fall17-noIso-V1-wp90")<<std::endl;
	if(el.pt()>60.0 and (el.electronID("mvaEleID-Fall17-noIso-V1-wp90")>0.5) and fabs(el.eta())<2.4)
		{
		//std::cout<< "Found EL "<<std::endl;
		foundel=true;
		lepp4 = el.p4();
		break;
		}
	}


  if(not (foundmu or foundel)) return false;
  if(foundmu and foundel) return false;
  edm::Handle<edm::View<pat::MET>> met;
  iEvent.getByToken(srcmet_, met);
  //for (const auto &mm : *met)
	//{
	//std::cout<<mm.corPt()<<std::endl;
	//}
  if (met->at(0).corPt()<50.0) return false;

  bool foundak8 = false;

  edm::Handle<edm::View<pat::Jet>> jetsAK8;
  iEvent.getByToken(srcAK8_, jetsAK8);

  if (jetsAK8->size()==0)return false;
  for (const auto &AK8pfjet : *jetsAK8)
	{
  	//std::cout<<"pt "<<AK8pfjet.pt()<<" DR "<<reco::deltaR(AK8pfjet.p4(),lepp4)<<std::endl;
	if (AK8pfjet.pt()>150.0 and (reco::deltaR(AK8pfjet.p4(),lepp4)>1.5))foundak8=true;
	}

  if (not foundak8)return false;

  //std::cout<<"passed MET "<<met->at(0).corPt()<<" leppt "<<lepp4.pt()<<std::endl;
  
  return true;
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



