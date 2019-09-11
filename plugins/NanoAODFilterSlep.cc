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



  edm::Handle<std::vector<reco::Vertex>> vtx;
  iEvent.getByToken(vtxl_, vtx);
  const std::vector<reco::Vertex>* vtxvec  = vtx.product();
  int curmuindex = 0;
  for (const auto &mu : *mus)
	{
        //float isol = (mu.pfIsolationR04().sumChargedHadronPt + std::max(0., mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt))/mu.pt();
	//std::cout<<"isol "<<isol<<std::endl;
	if(mu.pt()>50.0 and mu.isHighPtMuon(vtxvec->at(0)))
		{
		foundmu=true;
		}
	curmuindex+=1;
	}



  bool foundel = false;



  edm::Handle<edm::View<pat::Electron>> els;
  iEvent.getByToken(srcel_, els);

  for (const auto &el : *els)
	{
        //float isol = (mu.pfIsolationR04().sumChargedHadronPt + std::max(0., mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt))/mu.pt();
	//std::cout<<"isol "<<isol<<std::endl;
	std::cout<<"ept "<<el.pt()<<" idval "<<el.electronID("eidTight")<<std::endl;
	if(el.pt()>50.0 and el.electronID("eidTight"))
		{
		foundmu=true;
		}
	curmuindex+=1;
	}


  if(not (foundmu or foundel)) return false;

  edm::Handle<edm::View<pat::MET>> met;
  iEvent.getByToken(srcmet_, met);
  //for (const auto &mm : *met)
	//{
	//std::cout<<mm.corPt()<<std::endl;
	//}
  if (met->at(0).corPt()<50.0) return false;


  edm::Handle<edm::View<pat::Jet>> jetsAK4;
  iEvent.getByToken(srcAK4_, jetsAK4);



  float htval = 0.0;
  for (const auto &AK4pfjet : *jetsAK4)
	{
	float AK4pt = AK4pfjet.pt();
	if (AK4pfjet.pt()>30.0)
		{
 		htval+=AK4pt;
		}
	}

  if(htval<200.0) return false;

  edm::Handle<edm::View<pat::Jet>> jetsAK8;
  iEvent.getByToken(srcAK8_, jetsAK8);

  if (jetsAK8->size()==0)return false;
  if (jetsAK8->at(0).pt()<200.0)return false;
  
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



