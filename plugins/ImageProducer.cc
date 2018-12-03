#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <random>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"
#include "TLorentzVector.h"

#include <iostream>
#include <fstream>

#include <memory>
#include <iostream>
#include <Python.h>

struct ImageTFCache {
  ImageTFCache() : graphDef(nullptr) {
  }
  //From deepflavour implementation, for consistency 
  std::atomic<tensorflow::GraphDef*> graphDef;
};

class ImageProducer : public edm::stream::EDProducer<edm::GlobalCache<ImageTFCache>> {

  public:
    explicit ImageProducer(const edm::ParameterSet&, const ImageTFCache*);
    ~ImageProducer() override;
    double principal_axis(std::vector<std::vector<float>>);
    static void fillDescriptions(edm::ConfigurationDescriptions&);
    static void globalEndJob(const ImageTFCache*);
    static std::unique_ptr<ImageTFCache> initializeGlobalCache(const edm::ParameterSet&);

    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_real_distribution<> dis = std::uniform_real_distribution<>(0.0,1.0);

    //just for debug text3
    std::ofstream textout;

  private:
    void beginStream(edm::StreamID) override {}
    void produce(edm::Event&, const edm::EventSetup&) override;
    void endStream() override {}
    tensorflow::Session* tfsession_;

    edm::EDGetTokenT<std::vector<reco::GenParticle>> gplab ;
    edm::EDGetTokenT<reco::VertexCollection> vtx ;

    std::string sdmcoll="ak8PFJetsPuppiSoftDropMass";
    TFile *ratiofile = new TFile("ratfile.root");
    TH1F *ratiohist;
   

    edm::ESHandle<TransientTrackBuilder> builder;
    const edm::EDGetTokenT<edm::View<pat::Jet>> src_;
    const edm::EDGetTokenT<edm::View<pat::Jet>> sj_;
    const edm::EDGetTokenT<edm::View<reco::GenParticle>> genparts_;
    edm::FileInPath pb_path_;

};

ImageProducer::ImageProducer(const edm::ParameterSet& iConfig,  const ImageTFCache* cache)
: 
 tfsession_(nullptr)
, src_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("src")))
, sj_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("sj")))
{
  

  ratiohist = (TH1F*)ratiofile->Get("ToQratio");
  gplab = consumes<std::vector<reco::GenParticle>>(edm::InputTag("prunedGenParticles"));
  float maxim = ratiohist->GetMaximum();
  
  ratiohist->Scale(0.4/maxim);
  
  vtx = consumes<reco::VertexCollection>(edm::InputTag("offlineSlimmedPrimaryVertices"));
  produces<pat::JetCollection>();
  //td::mt19937 gen(rd());

  //std::uniform_real_distribution<>dis(0.0,1.0);
 
  tensorflow::SessionOptions sessionOptions;
  tfsession_ = tensorflow::createSession(cache->graphDef,sessionOptions);
  textout.open("debugout"+iConfig.getParameter<edm::InputTag>("src").label()+".dat");

  if(iConfig.getParameter<edm::InputTag>("src").label()=="slimmedJetsAK8")sdmcoll="ak8PFJetsCHSSoftDropMass";

}
ImageProducer::~ImageProducer(){
 tensorflow::closeSession(tfsession_);
}


template <typename T> std::string to_string_pr(const T a_value, const int n = 10)
{
    std::ostringstream out;
    out.precision(n);
    out << a_value;
    return out.str();
}


void ImageProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


std::unique_ptr<ImageTFCache> ImageProducer::initializeGlobalCache(
  const edm::ParameterSet& iConfig)
{
  tensorflow::setLogging("3");
  ImageTFCache* cache = new ImageTFCache();

  cache->graphDef = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_path", edm::FileInPath("PhysicsTools/NanoHRT/data/Image/NNtraining_preliminary_12032018.pb")).fullPath());
  return std::unique_ptr<ImageTFCache>(cache);
}

double ImageProducer::principal_axis(std::vector<std::vector<float>> partlist)
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


void ImageProducer::globalEndJob(const ImageTFCache* cache)
{
  if (cache->graphDef != nullptr) {
    delete cache->graphDef;
  }
}

void ImageProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
  edm::Handle<edm::View<pat::Jet>> jets;
  edm::Handle<edm::View<pat::Jet>> subjets;

  iEvent.getByToken(src_, jets);
  iEvent.getByToken(sj_, subjets);

  TLorentzVector curtlv;
  TLorentzVector sublv;
  

  bool match = false;
  int ttval=1;
  if(match)ttval=0;
	
  edm::Handle<reco::VertexCollection> vtxs;
  iEvent.getByToken(vtx, vtxs);

  edm::Handle<std::vector<reco::GenParticle>> genparts;
  iEvent.getByToken(gplab, genparts);
  const std::vector<reco::GenParticle>* genpartsvec  = genparts.product();

  int jindex=0;
  std::vector<float> itopdisc = {};
  int ntopinit = -1;
  //std::cout<<std::endl;
  for (const auto &AK8pfjet : *jets)
	{
	ntopinit+=1;
  	itopdisc.push_back(-10.0);


	
        TLorentzVector curtlv;
	curtlv.SetPtEtaPhiM(AK8pfjet.pt(),AK8pfjet.eta(),AK8pfjet.phi(),AK8pfjet.mass());

	//std::cout<<AK8pfjet.pt()<<std::endl;

	if(fabs(AK8pfjet.eta()>2.4))
		{
		itopdisc[ntopinit] = -100.0;
		continue;
		}
	if(AK8pfjet.pt()<600. or AK8pfjet.pt()>1400.)
		{
		itopdisc[ntopinit] = -100.0;
		continue;
		}

  	if(match==false)
		{
		
		double randomnum = dis(gen);
		float histoval = ratiohist->GetBinContent(ratiohist->FindBin(AK8pfjet.pt()));
		itopdisc[ntopinit] = -100.0;
		//std::cout<<randomnum<<" "<<histoval<<std::endl;
		if(randomnum>histoval) continue;
		}

  	if(match)
		{
		itopdisc[ntopinit] = -100.0;
		bool fulltopmatch=false;
		int ntops=0;

	  	for (auto &gp : *genpartsvec)
			{
			


			if(fulltopmatch==true)break;
			bool gpmatch=false;
			int gpid = gp.pdgId();
			

			TLorentzVector gplv;
			if(abs(gpid)==6)
				{
				
				gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
				if((gp.numberOfDaughters()>1)) 
					{
					if((abs(gp.daughter(0)->pdgId())==24) || (abs(gp.daughter(1)->pdgId())==24))
						{
						ntops+=1;
						//std::cout<<gplv.DeltaR(curtlv)<<std::endl;
						if(gplv.DeltaR(curtlv)<0.6)gpmatch=true;
						}

					}
				}
			
			
			if(gpmatch==true)
				{
				int wid = -1;
				int bid = -1;
				//bool bdrmatch=false;
				TLorentzVector gpblv;
				if (abs(gp.daughter(1)->pdgId())==5)
					{
					wid=0;
	  				bid=1;
					}
				if (abs(gp.daughter(0)->pdgId())==5)
					{
					wid=1;
					bid=0;
					}
				gpblv.SetPtEtaPhiM(gp.daughter(bid)->pt(),gp.daughter(bid)->eta(),gp.daughter(bid)->phi(),gp.daughter(bid)->mass());
				//std::cout<<"FB "<<curtlv.DeltaR(gpblv)<<std::endl;
				if(!(curtlv.DeltaR(gpblv)<0.8))continue;
				
				auto gpw = gp.daughter(wid);
				while(fulltopmatch==false)
					{
					//std::cout<<gpw->numberOfDaughters()<<"  "<<gpw->daughter(0)<<std::endl;
					
					if(gpw->numberOfDaughters()==1)
						{
							gpw=gpw->daughter(0);
							continue;
						}
					if(gpw->numberOfDaughters()>1)
						{
						//std::cout<<"d0 "<<gpw->daughter(0)->pdgId()<<" d1 "<<gpw->daughter(1)->pdgId()<<std::endl;
						if((abs(gpw->daughter(0)->pdgId())==24)) 
							{
							gpw=gpw->daughter(0);
							continue;
							}
						if((abs(gpw->daughter(1)->pdgId())==24))
							{
							gpw=gpw->daughter(1);
							continue;
							}
						if ((abs(gpw->daughter(0)->pdgId())<6) && (abs(gpw->daughter(1)->pdgId())<6)) 
							{
							//std::cout<<"d0 "<<gpw->daughter(0)->pdgId()<<" d1 "<<gpw->daughter(1)->pdgId()<<std::endl;
							TLorentzVector gpq1lv;
							gpq1lv.SetPtEtaPhiM(gpw->daughter(0)->pt(),gpw->daughter(0)->eta(),gpw->daughter(0)->phi(),gpw->daughter(0)->mass());


							TLorentzVector gpq2lv;
							gpq2lv.SetPtEtaPhiM(gpw->daughter(1)->pt(),gpw->daughter(1)->eta(),gpw->daughter(1)->phi(),gpw->daughter(1)->mass());
							//std::cout<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq1lv)<<std::endl;
							if ((curtlv.DeltaR(gpq1lv)<0.8) && (curtlv.DeltaR(gpq2lv)<0.8))fulltopmatch=true;
							else break;
							}

						else break;
						}
			
					}
				
				
				}
			if(fulltopmatch==true||ntops>1)break;
			}
		
		if(fulltopmatch!=true) continue;
		itopdisc[ntopinit]=-10.0;
		}
 	//std::cout<<"match"<<std::endl;

	int ndau = AK8pfjet.numberOfDaughters();
        std::vector<pat::PackedCandidate> allpf;

	std::vector<std::vector<float>> partlist = {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}};
	std::vector<float> sjlist = {};
	
	int idaufill = 0;
	for(int idau=0;idau<ndau;idau++)
		{
	        const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>(AK8pfjet.daughter(idau) );

	        //const pat::Jet* lPackjet = dynamic_cast<const pat::Jet *>(AK8pfjet.daughter(idau) );
		//if(lPackjet!=nullptr)std::cout<<"isjet"<<std::endl;
        	TLorentzVector pfclv;
		pfclv.SetPtEtaPhiM(lPack->pt(),lPack->eta(),lPack->phi(),lPack->mass());
	  	if (lPack != nullptr)
			{
        		TLorentzVector pfclv;
			pfclv.SetPtEtaPhiM(lPack->pt(),lPack->eta(),lPack->phi(),lPack->mass());
			if ((pfclv.Pt()<=1.0) and (lPack->charge()!=0)) continue;
			double funcval =(6.62733e-02) + (2.63732e+02)*(1.0/curtlv.Perp());
			double drcorval = 0.6/(funcval);

			int pfcflav = std::abs(lPack->pdgId());

			double neweta = pfclv.Eta()+(pfclv.Eta()-curtlv.Eta())*(drcorval-1.0);
			double newphi = pfclv.Phi()+(pfclv.Phi()-curtlv.Phi())*(drcorval-1.0);

			double newdetafj = (pfclv.Eta()-curtlv.Eta())*drcorval;
			double newdphifj = (pfclv.Phi()-curtlv.Phi())*drcorval;

			if(std::sqrt(newdphifj*newdphifj+newdetafj*newdetafj)>1.6) continue;
			float pw = lPack->puppiWeight();

    			//TrackInfoBuilder tinfo = lPack.buildTrackInfo(builder_, *lPack, AK8pfjet, vertices->at(0));

			//std::cout<<lPack->pt()<<std::endl;
			partlist[0].push_back(lPack->pt()*pw);
			partlist[1].push_back(neweta-curtlv.Eta());
			partlist[2].push_back(newphi-curtlv.Phi());
			if(pfcflav==13)partlist[3].push_back(lPack->pt()*pw);
			else partlist[3].push_back(0.0);
			if(pfcflav==11)partlist[4].push_back(lPack->pt()*pw);
			else partlist[4].push_back(0.0);
			if(pfcflav==211)partlist[5].push_back(lPack->pt()*pw);
			else partlist[5].push_back(0.0);
			if(pfcflav==22)partlist[6].push_back(lPack->pt()*pw);
			else partlist[6].push_back(0.0);
			if(pfcflav==130)partlist[7].push_back(lPack->pt()*pw);
			else partlist[7].push_back(0.0);


			//std::cout<<std::endl;
			if(lPack->hasTrackDetails())
				{
				
	  			const auto &trk = lPack->pseudoTrack();
	  			GlobalVector jetXYZVector(AK8pfjet.px(),AK8pfjet.py(),AK8pfjet.pz());
				reco::TransientTrack transientTrack = builder->build(trk);

	  			Measurement1D meas_ip2d = IPTools::signedTransverseImpactParameter(transientTrack, jetXYZVector, vtxs->at(0)).second;
	  			Measurement1D meas_ip3d = IPTools::signedImpactParameter3D(transientTrack, jetXYZVector, vtxs->at(0)).second;
	  			Measurement1D jetdist = IPTools::jetTrackDistance(transientTrack, jetXYZVector, vtxs->at(0)).second;


	  			float trackSip2dVal_ = meas_ip2d.value();
	  			float trackSip2dSig_ = meas_ip2d.significance();
	  			float trackSip3dVal_ = meas_ip3d.value();
	  			float trackSip3dSig_ = meas_ip3d.significance();

	  			float trackJetDistVal_ = jetdist.value();
	  			
	    			
	    			partlist[8].push_back(trackSip2dVal_);
	    			partlist[9].push_back(trackSip2dSig_);
	    			partlist[10].push_back(trackSip3dVal_);
	    			partlist[11].push_back(trackSip3dSig_);
	    			partlist[12].push_back(trackJetDistVal_);
				}
			else
				{
				//std::cout<<"badtrack"<<std::endl;
	    			partlist[8].push_back(0.0);
	    			partlist[9].push_back(0.0);
	    			partlist[10].push_back(0.0);
	    			partlist[11].push_back(0.0);
	    			partlist[12].push_back(0.0);

				}


	    		partlist[13].push_back(lPack->dz());
	    		partlist[14].push_back(lPack->dxy());
	    		partlist[15].push_back(pw);

			
			//std::cout<<partlist[8][idaufill]<<std::endl;
			//std::cout<<partlist[9][idaufill]<<std::endl;
			//std::cout<<partlist[10][idaufill]<<std::endl;
			//std::cout<<partlist[11][idaufill]<<std::endl;
			//std::cout<<partlist[12][idaufill]<<std::endl;
			//std::cout<<partlist[13][idaufill]<<std::endl;
			//std::cout<<"index14 "<<partlist[14][idaufill]<<std::endl;

			idaufill+=1;
			}
		}
	//std::cout<<"ndau"<<idaufill<<std::endl;
  	for (const auto &subjet : *subjets)
		{
	       
		sublv.SetPtEtaPhiM(subjet.pt(),subjet.eta(),subjet.phi(),subjet.mass());
		if (sublv.DeltaR(curtlv)>0.8 || sjlist.size()>=12) continue;

		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
		sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
		}


	uint sjlsize=sjlist.size();
	for(uint isj=0;isj<(12-sjlsize);isj++) sjlist.push_back(0.);

        //sjlist.push_back(fabs(AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass"))/172.0);
        sjlist.push_back(fabs(AK8pfjet.userFloat(sdmcoll))/172.0);
        sjlist.push_back(AK8pfjet.pt());
        sjlist.push_back(AK8pfjet.eta());
	


        int npoints=38;
	std::vector<int> ietalist = {};
	std::vector<int> iphilist = {};



	//centering and rotating 
	etac/=fullint;
	phic/=fullint;
	for(uint i=0;i < partlist[0].size();++i)
		{
	        partlist[1][i] -= etac;
	        partlist[2][i] -= phic;
		}

	double tan_theta=principal_axis(partlist); 
	double DReta=1.6;
	double DRphi=1.6;
	for(uint i=0;i < partlist[0].size();++i)
		{
		double Reta = partlist[1][i]*std::cos(std::atan(tan_theta))+partlist[2][i]*std::sin(std::atan(tan_theta));
		double Rphi = -1.0*partlist[1][i]*std::sin(std::atan(tan_theta))+partlist[2][i]*std::cos(std::atan(tan_theta));

		partlist[1][i] = Reta;
		partlist[2][i] = Rphi;

		ietalist.push_back(int((partlist[1][i]+DReta)/(2.0*DReta/float(npoints-1))));
		iphilist.push_back(int((partlist[2][i]+DRphi)/(2.0*DRphi/float(npoints-1))));
		}

  	//uint ncolors=6;
  	uint ncolors=14;
	std::vector<std::vector<std::vector<double>>> grid(37,std::vector<std::vector<double>>(37,std::vector<double>(ncolors,0.0)));
	std::vector<std::pair<std::vector<uint>,std::vector<double>>> indexedimage;

	//normalization and digitization
	for(uint i=0;i < partlist[0].size();++i)
		{
	        if((ietalist[i]>=37) or (iphilist[i]>=37) or (ietalist[i]<=0) or (iphilist[i]<=0))continue;
		int filldex=0;

		for(uint j=0;j < partlist.size();++j)
			{

			if(((j>2) or (j==0)) and (j<8)) //1 and 2 are eta,phi
				{
				grid[ietalist[i]][iphilist[i]][filldex]+=partlist[j][i]/fullint;
				filldex+=1;
				}
			if(j>=8)
				{
				//std::cout<<"dex "<<filldex<<" val "<<partlist[j][i]<<std::endl;
				if(partlist[j][i]>grid[ietalist[i]][iphilist[i]][filldex])
					{
					//std::cout<<"dex "<<filldex<<"setting "<<grid[ietalist[i]][iphilist[i]][filldex]<<" to "<<partlist[j][i]<<std::endl;
					grid[ietalist[i]][iphilist[i]][filldex]=partlist[j][i];
					}
				filldex+=1;
				}
			}				
		}

	for(uint i=0;i < grid.size();++i)
		{
		for(uint j=0;j < grid[i].size();++j)
			{
				if(grid[i][j][0]>0.0000000001)
					{
					std::pair<std::vector<uint>,std::vector<double>> elem;
					elem.first = {i,j};

					for(uint k=0;k < grid[i][j].size();++k)
					{
						//std::cout<<"elem "<<i<<":"<<j<<"  -  "<<grid[i][j][k]<<std::endl;
						elem.second.push_back(grid[i][j][k]);					
					}
					indexedimage.push_back(elem);
					}
			}
		}

	//flipping horiz and vert
	uint half_img=(npoints-2)/2;
	float left_sum=0.0;
	float right_sum=0.0;
	for(uint i=0;i < indexedimage.size();++i)
		{
		if(indexedimage[i].first[0]<half_img)left_sum+=indexedimage[i].second[0];
		if(indexedimage[i].first[0]>half_img)right_sum+=indexedimage[i].second[0];
		}
	if(left_sum<right_sum)
		{
		for(uint i=0;i < indexedimage.size();++i)indexedimage[i].first={npoints-2-indexedimage[i].first[0],indexedimage[i].first[1]};	
		}

	float lower_sum=0.0;
	float upper_sum=0.0;
	for(uint i=0;i < indexedimage.size();++i)
		{
		if(indexedimage[i].first[1]>half_img)lower_sum+=indexedimage[i].second[0];
		if(indexedimage[i].first[1]<half_img)upper_sum+=indexedimage[i].second[0];
		}

	if(lower_sum<upper_sum)
		{
		for(uint i=0;i < indexedimage.size();++i)indexedimage[i].first={indexedimage[i].first[0],npoints-2-indexedimage[i].first[1]};		
		}

	//convert scalars to tensorflow
	std::vector<tensorflow::Tensor> outputs1;
	tensorflow::Tensor input_b(tensorflow::DT_FLOAT, { 1, int(sjlist.size()) });
	float* d = input_b.flat<float>().data();
	uint index=-1;
	for(uint i=0;i < sjlist.size();++i,++d)
		{
		if (i<12)index = (i%6)*2 + i/6;
		else index=i;
		*d = sjlist[index];	
		}	
	
	//convert image to tensorflow.  first create tensor of zeros, then fill.  Probably not optimal quite yet
	tensorflow::Tensor input_image(tensorflow::DT_FLOAT, tensorflow::TensorShape({ 1,37, 37 , ncolors }));
	auto input_map = input_image.tensor<float, 4>();

	//Debug printing to ascii
	std::string textevent = "[[";
	for(uint i=0;i < indexedimage.size();++i)
		{
		if(i>0)textevent+=",";
		textevent+="[[";
		textevent+=to_string_pr(indexedimage[i].first[0]);
		textevent+=",";
		textevent+=to_string_pr(indexedimage[i].first[1]);
		textevent+="],[";
		for(uint j=0;j < indexedimage[i].second.size();++j)
			{
			if(j>0)textevent+=",";
			textevent+=to_string_pr(indexedimage[i].second[j]);
			}	
		textevent+="]]";
		}

	textevent+="],";
	textevent+=std::to_string(ttval);
	textevent+=",1.0";
	index=-1;
	for(uint i=0;i < sjlist.size();++i,++d)
		{
		textevent+=",";
		if(i<12)index = (i%6)*2 + i/6;
		else index=i;
		textevent+=to_string_pr(sjlist[index]);	
		}	

	textevent+="]";



	textout<<textevent<<"\n";

        // std::cout<<textevent<<std::endl;
	//End debug printing

	for(uint i=0;i < 37;++i)
		{
		for(uint j=0;j < 37;++j)
			{
			for(uint k=0;k < ncolors;++k)
				{
					input_map(0,i,j,k) = 0.0;
				}
				
			}
			
		}
	
	for(uint i=0;i < indexedimage.size();++i)
		{
		for(uint j=0;j < indexedimage[i].second.size();++j)
			{
				input_map(0,indexedimage[i].first[0], indexedimage[i].first[1], j) = indexedimage[i].second[j];
			}	
		}

	//Actually run tensorflow
  	/*tensorflow::Status status = tfsession_->Run({ { "input_1", input_image }, {"input_2", input_b} },{ "k2tfout_0" }, {}, &outputs1);

	if (!status.ok()) 
		{
		std::cout << "Tensorflow Failed:" << std::endl;
  		std::cout << status.ToString() << "\n";
  		return ;
		}	
        float result_top = outputs1[0].flat<float>()(0);
        float result_qcd = outputs1[0].flat<float>()(1);

	itopdisc[jindex]=result_top/(result_top+result_qcd);
	jindex+=1;*/itopdisc[ntopinit]=0.0;
  }


  //Add to jet userfloats
  auto outputs = std::make_unique<pat::JetCollection>();
  jindex=0;
  for (const auto &jet : *jets){
    pat::Jet newJet(jet);
    std::cout<<itopdisc[jindex]<<std::endl;
    newJet.addUserFloat("Image:top", itopdisc[jindex]);
    outputs->push_back(newJet);
    jindex+=1;
  }

  // put into the event
  iEvent.put(std::move(outputs));
}

//define this as a plug-in
DEFINE_FWK_MODULE(ImageProducer);
