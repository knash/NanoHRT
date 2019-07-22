#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "TLorentzVector.h"

#include <iostream>
#include <fstream>

#include <memory>
#include <iostream>
#include <Python.h>



class  NanoAOD_Filter_Slep_post : public edm::stream::EDFilter<> {
public:
  explicit NanoAOD_Filter_Slep_post( const edm::ParameterSet & );   
private:
  bool ISDATA_ ;
  const edm::EDGetTokenT<edm::View<pat::Jet>> srcAK8_;
  const edm::EDGetTokenT<edm::View<pat::Jet>> srcAK4_;
  bool filter( edm::Event &, const edm::EventSetup & );
  void beginRun(edm::Run const&, edm::EventSetup const&);
  void beginJob() ;
  void endJob() ;
 };


NanoAOD_Filter_Slep_post::NanoAOD_Filter_Slep_post(const edm::ParameterSet& iConfig):
srcAK8_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcAK8")))
,srcAK4_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("srcAK4")))
 {   

 }


bool NanoAOD_Filter_Slep_post::filter( edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<edm::View<pat::Jet>> jetsAK8;
  iEvent.getByToken(srcAK8_, jetsAK8);

  int njets = jetsAK8->size();
  //std::cout<<"FROMSLEPPOST"<<std::endl;
  //std::cout<<"Njets "<< njets <<std::endl;
  //for (const auto &AK8pfjet : *jetsAK8)
	//{
	//std::cout<<"SD: "<<AK8pfjet.userFloat("ak8PFJetsPuppiWWSoftDropMass")<<std::endl;
	//std::cout<<"PT: "<<AK8pfjet.pt()<<std::endl;
	//std::cout<<"ETA: "<<AK8pfjet.eta()<<std::endl;
	//std::cout<<"PHI: "<<AK8pfjet.phi()<<std::endl;
	//}
  if(njets==0) return false;


  edm::Handle<edm::View<pat::Jet>> jetsAK4;
  iEvent.getByToken(srcAK4_, jetsAK4);

  int njetsak4 = jetsAK4->size();
  if(njetsak4==0) return false;
  //std::cout<<"PASS sleppost"<<std::endl;
  return true;

 }




void 
NanoAOD_Filter_Slep_post::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
 

		
}



// ------------ method called once each job just before starting event loop  ------------
void 
NanoAOD_Filter_Slep_post::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NanoAOD_Filter_Slep_post::endJob() 
{

}

#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(NanoAOD_Filter_Slep_post);



