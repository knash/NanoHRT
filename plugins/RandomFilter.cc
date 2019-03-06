#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <random>


#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"
#include "TLorentzVector.h"

#include <iostream>
#include <fstream>

#include <memory>
#include <iostream>
#include <Python.h>




class  Random_Filter : public edm::EDFilter {
public:

  explicit Random_Filter( const edm::ParameterSet & );   
    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_real_distribution<> dis = std::uniform_real_distribution<>(0.0,1.0);

 // ~Random_Filter();
private:
  float randoval;
  int saveevery_;
  edm::EDGetTokenT<std::vector<reco::GenParticle>> gplab ;
  const edm::EDGetTokenT<edm::View<reco::GenParticle>> genparts_;
  bool filter( edm::Event &, const edm::EventSetup & );
  void beginRun(edm::Run const&, edm::EventSetup const&);
  void beginJob() ;
  void endJob() ;
 };


Random_Filter::Random_Filter(const edm::ParameterSet& iConfig):
   saveevery_ (iConfig.getUntrackedParameter<int>("saveevery", 1))
 {   
  randoval=1.0/float(saveevery_);
 }


bool Random_Filter::filter( edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  double randomnum = dis(gen);

  if (randoval>randomnum) return 1;
  return 0;
 }




void 
Random_Filter::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{		
}



// ------------ method called once each job just before starting event loop  ------------
void 
Random_Filter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Random_Filter::endJob() 
{

}

#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(Random_Filter);








