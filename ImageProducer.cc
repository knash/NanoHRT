#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "../interface/ImageTagger.h"

#include <memory>
#include <iostream>
#include <Python.h>
class ImageProducer : public edm::stream::EDProducer<> {

  public:
    explicit ImageProducer(const edm::ParameterSet&);
    ~ImageProducer() override;
    double principal_axis(std::vector<std::vector<double>>);
    static void fillDescriptions(edm::ConfigurationDescriptions&);

  private:
    void beginStream(edm::StreamID) override {}
    void produce(edm::Event&, const edm::EventSetup&) override;
    void endStream() override {}

    const edm::EDGetTokenT<edm::View<pat::Jet>> src_;
    const edm::EDGetTokenT<edm::View<pat::Jet>> sj_;
    edm::FileInPath dnn_path_;

    std::unique_ptr<ImageTagger> tagger_;

};

ImageProducer::ImageProducer(const edm::ParameterSet& iConfig)
: src_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("src")))
, sj_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("sj")))
, dnn_path_(iConfig.getUntrackedParameter<edm::FileInPath>("dnn_path", edm::FileInPath("PhysicsTools/NanoHRT/data/BEST/BEST_mlp.json")))
{
  produces<pat::JetCollection>();
}

ImageProducer::~ImageProducer(){
}

void ImageProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

double ImageProducer::principal_axis(std::vector<std::vector<double>> partlist)
	{
  	double tan_theta=0.0;
	double M11=0.0;
	double M20=0.0;
	double M02=0.0;
	for(uint i=0;i < partlist[0].size();++i)
		{
		M11 += partlist[0][i]*partlist[1][i]*partlist[2][i];
		M20 += partlist[0][i]*partlist[1][i]*partlist[1][i];
		M02 += partlist[0][i]*partlist[2][i]*partlist[2][i];
		}
  	double denom=(M20-M02+std::sqrt(4*M11*M11+(M20-M02)*(M20-M02)));
  	if(denom!=0.0) tan_theta=2.0*M11/denom;
  	return tan_theta;
	}


void ImageProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<edm::View<pat::Jet>> jets;
  edm::Handle<edm::View<pat::Jet>> subjets;

  iEvent.getByToken(src_, jets);
  iEvent.getByToken(sj_, subjets);
  std::cout<<"event"<<std::endl;
  TLorentzVector curtlv;
  TLorentzVector sublv;
  for (const auto &AK8pfjet : *jets)
	{
        TLorentzVector curtlv;
	curtlv.SetPtEtaPhiM(AK8pfjet.pt(),AK8pfjet.eta(),AK8pfjet.phi(),AK8pfjet.mass());
  	//std::cout<<curtlv.Perp()<<std::endl;
	int ndau = AK8pfjet.numberOfDaughters();
        std::vector<pat::PackedCandidate> allpf;

	//sdlabs = []
	std::vector<std::vector<double>> partlist = {{},{},{},{},{},{},{},{}};
	std::vector<double> sjlist = {};
	//int sjn=0;
	//SDtop=None;
	//std::vector<edm::Ptr<pat::Jet>>subjs = AK8pfjet.subjets();
	for(int idau=0;idau<ndau;idau++)
		{

	        const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>(AK8pfjet.daughter(idau) );
		//const pat::Jet* ljet = dynamic_cast<const pat::Jet *>( AK8pfjet.daughter(idau));
        	TLorentzVector pfclv;
		pfclv.SetPtEtaPhiM(lPack->pt(),lPack->eta(),lPack->phi(),lPack->mass());
  		//std::cout<<"pack?"<<std::endl;
	  	if (lPack != nullptr)
			{
			double funcval =(6.62733e-02) + (2.63732e+02)*(1.0/curtlv.Perp());
			double drcorval = 0.6/(funcval);


			int pfcflav = std::abs(lPack->pdgId());


			double neweta = pfclv.Eta()+(pfclv.Eta()-curtlv.Eta())*(drcorval-1.0);
			double newphi = pfclv.Phi()+(pfclv.Phi()-curtlv.Phi())*(drcorval-1.0);

			double newdetafj = (pfclv.Eta()-curtlv.Eta())*drcorval;
			double newdphifj = (pfclv.Phi()-curtlv.Phi())*drcorval;

			//std::cout<<newdetafj<<"   "<<newdphifj<<std::endl;
			if(std::sqrt(newdphifj*newdphifj+newdetafj*newdetafj)>1.6) continue;

			partlist[0].push_back(lPack->pt()*lPack->puppiWeight());
			partlist[1].push_back(neweta-curtlv.Eta());
			partlist[2].push_back(newphi-curtlv.Phi());
			if(pfcflav==13)partlist[3].push_back(lPack->pt()*lPack->puppiWeight());
			else partlist[3].push_back(0.0);
			if(pfcflav==11)partlist[4].push_back(lPack->pt()*lPack->puppiWeight());
			else partlist[4].push_back(0.0);
			if(pfcflav==211)partlist[5].push_back(lPack->pt()*lPack->puppiWeight());
			else partlist[5].push_back(0.0);
			if(pfcflav==22)partlist[6].push_back(lPack->pt()*lPack->puppiWeight());
			else partlist[6].push_back(0.0);
			if(pfcflav==130)partlist[7].push_back(lPack->pt()*lPack->puppiWeight());
			else partlist[7].push_back(0.0);

	
			
			}
		
		}
	
  	for (const auto &subjet : *subjets)
		{
		sublv.SetPtEtaPhiM(subjet.pt(),subjet.eta(),subjet.phi(),subjet.mass());
		if (sublv.DeltaR(curtlv)>0.8) continue;

		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
		}
        
	for(uint isj=0;isj<(12-sjlist.size());isj++) sjlist.push_back(-10.);
	double tan_theta=principal_axis(partlist); 

        int npoints=38;
	std::vector<int> ietalist = {};
	std::vector<int> iphilist = {};
	//rotation
	double fullint = 0.0;
	double DReta=1.6;
	double DRphi=1.6;
	for(uint i=0;i < partlist[0].size();++i)
		{
		fullint+=partlist[0][i];
	
		partlist[1][i] = partlist[1][i]*std::cos(std::atan(tan_theta))+partlist[2][i]*std::sin(std::atan(tan_theta));
		partlist[2][i] = -1.0*partlist[1][i]*std::sin(std::atan(tan_theta))+partlist[2][i]*std::cos(std::atan(tan_theta));
		ietalist.push_back(int((partlist[1][i]+DReta)/(2.0*DReta/float(npoints-1))));
		iphilist.push_back(int((partlist[2][i]+DRphi)/(2.0*DRphi/float(npoints-1))));
		}
  	int ncolors=15;
	std::vector<std::vector<std::vector<double>>> grid(37,std::vector<std::vector<double>>(37,std::vector<double>(ncolors,0.0)));
	//std::fill(grid[0][0], grid[0][0]+37*37*ncolors, 0.0);
	std::vector<std::pair<std::vector<uint>,std::vector<double>>> indexedimage;
	//normalization,//digitization
	for(uint i=0;i < partlist[0].size();++i)
		{
		for(uint j=0;j < partlist.size();++j)
			{
			if((j>2) or (j==0))
				{
				grid[ietalist[i]][iphilist[i]][j]+=partlist[j][i]/fullint;
				//std::cout<<partlist[j][i]/fullint<<" -- "<<grid[ietalist[i]][iphilist[i]][j]<<std::endl;
				}
			}				
		}

	for(uint i=0;i < grid.size();++i)
		{

		for(uint j=0;j < grid[i].size();++j)
			{
				std::cout<<grid[i][j][0]<<std::endl;
				if(grid[i][j][0]>0.00000001)
					{
					std::pair<std::vector<uint>,std::vector<double>> elem;
					elem.first = {i,j};
					std::cout<<elem.first[0]<<","<<elem.first[1]<<std::endl;
					for(uint k=0;k < grid[i][j].size();++k)
					{
						elem.second.push_back(grid[i][j][k]);					
					//	std::cout<<grid[i][j][k]<<std::endl;
					}
					 	

					indexedimage.push_back(elem);
					}
			}
		}


	half_img=(npoints-2)/2
	lower_sum=0.0
	upper_sum=0.0
	for(uint i=0;i < indexedimage.size();++i)
		{
		if(indexedimage[i][0][1]<half_img)lower_sum+=indexedimage[i][1][0];
		else upper_sum+=indexedimage[i][1][0];
		}
	if(lower_sum>upper_sum)
	{
		for(uint i=0;i < indexedimage.size();++i)
			{
  			indexedimage[i][0]={indexedimage[i][0][0],npoints-2-indexedimage[i][0][1]};
			}
	}
  }

  auto outputs = std::make_unique<pat::JetCollection>();

  for (const auto &jet : *jets){
    pat::Jet newJet(jet);
    //newJet.addUserFloat("Image:top", 1.0);
    outputs->push_back(newJet);
  }

  // put into the event
  iEvent.put(std::move(outputs));
}

//define this as a plug-in
DEFINE_FWK_MODULE(ImageProducer);
