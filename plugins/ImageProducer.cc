#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"
#include "TLorentzVector.h"
#include "TLorentzVector.h"
#include "../interface/ImageProducer.hh"

#include <iostream>
#include <fstream>

#include <memory>
#include <iostream>


struct ImageTFCache {
  ImageTFCache() : graphDef(nullptr) {
  }
  //From deepflavour implementation, for consistency 
  std::atomic<tensorflow::GraphDef*> graphDef;
  std::atomic<tensorflow::GraphDef*> graphDefMD;
};

class ImageProducer : public edm::stream::EDProducer<edm::GlobalCache<ImageTFCache>> {

  public:
    explicit ImageProducer(const edm::ParameterSet&, const ImageTFCache*);
    ~ImageProducer() override;
    void threadwrite(std::ofstream &,std::string);
    double principal_axis(const std::vector<std::vector<float>> &);
    static void fillDescriptions(edm::ConfigurationDescriptions&);
    static void globalEndJob(const ImageTFCache*);
    static std::unique_ptr<ImageTFCache> initializeGlobalCache(const edm::ParameterSet&);


  private:
    void beginStream(edm::StreamID) ;
    void produce(edm::Event&, const edm::EventSetup&) override;
    void endStream() override {}
    tensorflow::Session* tfsession_;
    tensorflow::Session* tfsessionMD_;
    bool isHotVR;
    uint nsubs;
    edm::EDGetTokenT<std::vector<reco::GenParticle>> gplab ;
    const edm::EDGetTokenT<edm::View<pat::Jet>> src_;
    const edm::EDGetTokenT<edm::View<pat::Jet>> sj_;
    std::string sdmcoll_;
    edm::FileInPath pb_path_;
    edm::FileInPath pb_pathMD_;
    std::string extex_;
    std::string stype_;
    std::ofstream textout;
    std::atomic<unsigned int> streamnum_;
    //uint* streamnum_;


    const edm::EDGetTokenT<edm::View<reco::GenParticle>> genparts_;
};

ImageProducer::ImageProducer(const edm::ParameterSet& iConfig,  const ImageTFCache* cache)
: 
 tfsession_(nullptr)
, tfsessionMD_(nullptr)
, src_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("src")))
, sj_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("sj")))
, sdmcoll_(iConfig.getParameter<std::string>("sdmcoll"))
, extex_(iConfig.getParameter<std::string>("extex"))
, stype_(iConfig.getParameter<std::string>("stype"))
{
  produces<pat::JetCollection>();
  gplab = consumes<std::vector<reco::GenParticle>>(edm::InputTag("prunedGenParticles"));
  tensorflow::SessionOptions sessionOptions;
  tfsession_ = tensorflow::createSession(cache->graphDef,sessionOptions);
  tfsessionMD_ = tensorflow::createSession(cache->graphDefMD,sessionOptions);
  isHotVR=false;



}
ImageProducer::~ImageProducer(){
 tensorflow::closeSession(tfsession_);
 tensorflow::closeSession(tfsessionMD_);
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

  cache->graphDef = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_path").fullPath());
  cache->graphDefMD = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathMD").fullPath());
  return std::unique_ptr<ImageTFCache>(cache);
}

template <typename T> std::string to_string_pr(const T a_value, const int n = 10)
{

    std::ostringstream out;
    out.precision(n);
    out << a_value;
    return out.str();
}

void ImageProducer::threadwrite(std::ofstream & wfile,std::string towrite)
    {
	wfile<<towrite<<"\n";
	wfile.flush();

    }
double ImageProducer::principal_axis(const std::vector<std::vector<float>> & partlist)
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
  if (cache->graphDefMD != nullptr) {
    delete cache->graphDefMD;
  }
}

void ImageProducer::beginStream(edm::StreamID ID)
	{
	streamnum_=ID.value();
	if(extex_=="HotVR")
		{
		nsubs=4;
	  	textout.open("debugoutHotVR"+std::to_string(streamnum_)+".dat");
		isHotVR=true;
		}
	else
		{
		nsubs=2;
	  	textout.open("debugout"+std::to_string(streamnum_)+".dat");
		}
	}

void ImageProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<edm::View<pat::Jet>> jets;
  edm::Handle<edm::View<pat::Jet>> subjets;

  iEvent.getByToken(src_, jets);
  iEvent.getByToken(sj_, subjets);

  TLorentzVector curtlv;
  TLorentzVector sublv;


  edm::Handle<std::vector<reco::GenParticle>> genparts;
  iEvent.getByToken(gplab, genparts);
  const std::vector<reco::GenParticle>* genpartsvec  = genparts.product();

  std::vector<float> itopdisc = {};
  std::vector<float> itopdiscMD = {};
  std::vector<float> DRaves = {};
  int ntopinit = -1;
  int jindex=0;

  for (const auto &AK8pfjet : *jets)
	{

	ntopinit+=1;
  	itopdisc.push_back(-10.0);
  	itopdiscMD.push_back(-10.0);
	DRaves.push_back(-10.0);

        TLorentzVector curtlv;
	curtlv.SetPtEtaPhiM(AK8pfjet.pt(),AK8pfjet.eta(),AK8pfjet.phi(),AK8pfjet.mass());
  	int ttval=1;
	std::pair<int,float> matched(0,-10.0);
	//std::cout << extex_ << std::endl;


	//if (stype_="WW" and isHotVR)
	float mergeval=0.8;
	if(isHotVR) mergeval=1.2;
	//std::cout<<"stype_ "<<stype_<<" extex_ "<<extex_<<" mergeval "<<mergeval<<std::endl;

	if(stype_!="QCD") 	{
				matched = signalmatch(curtlv, genpartsvec,stype_,mergeval);			
				DRaves[ntopinit]=matched.second;
				ttval=0;
				if (not matched.first) continue;
				matched.first+=3;

				}
	else 			{
				if(isHotVR) std::cout<<"partonFlavour "<<AK8pfjet.partonFlavour()<<std::endl;
				if(abs(AK8pfjet.partonFlavour())<4  && abs(AK8pfjet.partonFlavour())>0)matched.first=0;
				else if(abs(AK8pfjet.partonFlavour())==4)matched.first=1;
				else if(abs(AK8pfjet.partonFlavour())==5)matched.first=2;
				else if(abs(AK8pfjet.partonFlavour())==21)matched.first=3;
				else continue;
				DRaves[ntopinit]=matched.second;
				}				
	//std::cout<<"TT "<<ttval<<" "<< matched.first << std::endl;
  	itopdisc[ntopinit]=-1.0;
  	itopdiscMD[ntopinit]=-1.0;

	int ndau = AK8pfjet.numberOfDaughters();
        std::vector<pat::PackedCandidate> allpf;

	std::vector<std::vector<float>> partlist = {{},{},{},{},{},{},{},{}};
	std::vector<float> sjlist = {};
	
	double fullint = 0.0;
        double etac=0;
        double phic=0;

	int idaufill = 0;
	//if(isHotVR)std::cout<<"ndau "<<ndau<<std::endl;
  	//edm::Handle<edm::View<reco::Candidate>> particles;
  	//iEvent.getByToken("puppi" particles);
  	//const auto constituents = AK8pfjet.getPFConstituents();
	//std::cout<<"ndauaaa "<<constituents.size()<<std::endl;



	for(int idau=0;idau<ndau;idau++)
		{
	        //const pat::Jet* lJet = dynamic_cast<const pat::Jet *>(AK8pfjet.daughter(idau) );
	  	//if (lJet != nullptr)std::cout << "NDAU "<<idau<<" "<<lJet->numberOfDaughters() << std::endl;
		//std::cout << AK8pfjet.daughter(idau)->numberOfDaughters() << std::endl;
	        const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate *>(AK8pfjet.daughter(idau) );
	  	if (lPack != nullptr)
			{
			float dphi = reco::deltaPhi(lPack->phi(),curtlv.Phi());

        		TLorentzVector pfclv;
			pfclv.SetPtEtaPhiM(lPack->pt(),lPack->eta(),curtlv.Phi()+dphi,lPack->mass());
			if ((pfclv.Pt()<=1.0) and (lPack->charge()!=0)) continue;
			double funcval =(6.62733e-02) + (2.63732e+02)*(1.0/curtlv.Perp());
			double drcorval = 0.6/(funcval);

			int pfcflav = std::abs(lPack->pdgId());

			double neweta = pfclv.Eta()+(pfclv.Eta()-curtlv.Eta())*(drcorval-1.0);
			double newphi = pfclv.Phi()+(dphi)*(drcorval-1.0);


			double newdetafj = (pfclv.Eta()-curtlv.Eta())*drcorval;
			double newdphifj = (dphi)*drcorval;


			if(std::sqrt(newdphifj*newdphifj+newdetafj*newdetafj)>1.6) continue;

		
			//float pw = lPack->puppiWeight();

			partlist[0].push_back(lPack->pt());
			partlist[1].push_back(neweta);
			partlist[2].push_back(newphi);

			fullint+=partlist[0][idaufill];
			etac += partlist[0][idaufill]*partlist[1][idaufill];
			phic += partlist[0][idaufill]*partlist[2][idaufill];

			if(pfcflav==13)partlist[3].push_back(lPack->pt());
			else partlist[3].push_back(0.0);
			if(pfcflav==11)partlist[4].push_back(lPack->pt());
			else partlist[4].push_back(0.0);
			if(pfcflav==211)partlist[5].push_back(lPack->pt());
			else partlist[5].push_back(0.0);
			if(pfcflav==22)partlist[6].push_back(lPack->pt());
			else partlist[6].push_back(0.0);
			if(pfcflav==130)partlist[7].push_back(lPack->pt());
			else partlist[7].push_back(0.0);
			idaufill+=1;
			}
		}

	//std::cout<<"nsubs "<<nsubs<<" hotv "<<isHotVR<<std::endl;
	float gmass = 0.0;
	int nsjh = 0;
	float mmin = 0.;
	float fpt = 0.;
	if(isHotVR)	
		{
		TLorentzVector gjet;
        	std::vector<pat::Jet> sjvec;
		//auto sjhv =  AK8pfjet.subjets();
  		for (const auto &subjet : *subjets)
			{
				sjvec.push_back(subjet);
				sublv.SetPtEtaPhiM(subjet.pt(),subjet.eta(),subjet.phi(),subjet.mass());
				//std::cout<<sublv.DeltaR(curtlv)<<std::endl;
				if (sublv.DeltaR(curtlv)>0.8 || sjlist.size()>=(nsubs*6)) continue;
				if(sjlist.size()==0)gjet=sublv;
				else gjet+=sublv;
				sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
				sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
				sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
				sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
				sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
				sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
											

	  		//for (const auto &clsubjet : sjhv)
			//	{
			//	if(reco::deltaR2( clsubjet->p4(),subjet.p4())<1e-5) 	{
										
				
			}

		nsjh = sjvec.size();
		mmin = 0.;
		fpt = 0.;
		if (nsjh >= 3)	{
			  	fpt = sjvec[0].pt() / AK8pfjet.pt();
			      	mmin = std::min({
				(sjvec[0].p4()+sjvec[1].p4()).mass(),
				(sjvec[0].p4()+sjvec[2].p4()).mass(),
				(sjvec[1].p4()+sjvec[2].p4()).mass(),
			      	});
		    		} 

		gmass=gjet.M();
		//std::cout<<"M "<<gmass<<std::endl;

		}


	else	{
	  	for (const auto &subjet : *subjets)
			{
	  		//const auto constituents = subjet.getPFConstituents();
		        //std::cout<<"stdau3 "<<constituents.size()<<std::endl;
			sublv.SetPtEtaPhiM(subjet.pt(),subjet.eta(),subjet.phi(),subjet.mass());
			if (sublv.DeltaR(curtlv)>0.8 || sjlist.size()>=(nsubs*6)) continue;
			sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
			sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
			sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
			sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
			sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
			sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
			}
		gmass=fabs(AK8pfjet.userFloat(sdmcoll_));
		}
			
	uint sjlsize=sjlist.size();
	//std::cout<<"bef"<<std::endl;
	//for(auto sjsj : sjlist)std::cout<<sjsj<<std::endl;
	for(uint isj=0;isj<(nsubs*6-sjlsize);isj++) sjlist.push_back(0.);
	//std::cout<<"aft"<<std::endl;
	//for(auto sjsj : sjlist)std::cout<<sjsj<<std::endl;
	if(isHotVR)	
		{
		sjlist.push_back(nsjh);
		sjlist.push_back(mmin);
		sjlist.push_back(fpt);
		}
	//std::cout<<matched.first<<std::endl;
        sjlist.push_back(matched.first);
        sjlist.push_back(gmass/172.0);
	int extravars = 2;
	float massval=gmass/172.0;
	float ptval=AK8pfjet.pt()/2000.0;
	if(ptval<0.5 or ptval>0.85) continue ;
	ptval = std::max(float(0.2),ptval);
	ptval = std::min(float(0.85),ptval);
        sjlist.push_back(AK8pfjet.pt()/2000.0);
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

  	uint ncolors=6;
  
	std::vector<std::vector<std::vector<double>>> grid(37,std::vector<std::vector<double>>(37,std::vector<double>(ncolors,0.0)));
	std::vector<std::pair<std::vector<uint>,std::vector<double>>> indexedimage;

	//normalization and digitization
	for(uint i=0;i < partlist[0].size();++i)
		{
	        if((ietalist[i]>=37) or (iphilist[i]>=37) or (ietalist[i]<=0) or (iphilist[i]<=0))continue;
		int filldex=0;

		for(uint j=0;j < partlist.size();++j)
			{
			if(((j>2) or (j==0))) //1 and 2 are eta,phi
				{
				grid[ietalist[i]][iphilist[i]][filldex]+=partlist[j][i]/fullint;
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
					for(uint k=0;k < grid[i][j].size();++k)elem.second.push_back(grid[i][j][k]);					
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
	std::vector<tensorflow::Tensor> tfoutput;
	std::vector<tensorflow::Tensor> tfoutputMD;
	tensorflow::Tensor input_b(tensorflow::DT_FLOAT, { 1, int(sjlist.size()-extravars) });
	float* d = input_b.flat<float>().data();//
	for(uint i=0;i < 14;++i,++d)
		{
		//if (i<nsubs*6)index = (i%6)*nsubs + i/6;
		//else index=i;
		if(i==12) *d = massval;
		else if(i==13) *d = ptval;
		else *d = sjlist[i];	
		//std::cout<<i<<" - "<<*d<<std::endl;
		}	
	
	//convert image to tensorflow.  first create tensor of zeros, then fill.  Probably not optimal quite yet
	tensorflow::Tensor input_image(tensorflow::DT_FLOAT, tensorflow::TensorShape({ 1,37, 37 , ncolors }));
	auto input_map = input_image.tensor<float, 4>();

        std::lock_guard<std::mutex> lock(write_mutex);



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
	//std::cout<<"Indexing"<<std::endl;
	for(uint i=0;i < sjlist.size();++i,++d)
		{
		textevent+=",";
		//if(i<nsubs*6)index = (i%6)*nsubs + i/6;
		//else index=i;
		//std::cout<<i<<" - " <<index<<" - " <<sjlist[index]<<std::endl;
		textevent+=to_string_pr(sjlist[i]);	
		}	

	textevent+="]";

	threadwrite(textout,textevent);
	//std::cout<<"write "<<std::endl;
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

	if(!isHotVR){
	//Actually run tensorflow
  	tensorflow::Status status = tfsession_->Run({ { "input_1", input_image }, {"input_2", input_b} },{ "k2tfout_0" }, {}, &tfoutput);

	if (!status.ok()) 
		{
		std::cout << "Tensorflow Failed:" << std::endl;
  		std::cout << status.ToString() << "\n";
  		return ;
		}	
        //float result_top = tfoutput[0].flat<float>()(0);
        //float result_qcd = tfoutput[0].flat<float>()(1);
        float result_top = tfoutput[0].flat<float>()(4)+tfoutput[0].flat<float>()(5)+tfoutput[0].flat<float>()(6);
        float result_qcd = tfoutput[0].flat<float>()(0)+tfoutput[0].flat<float>()(1)+tfoutput[0].flat<float>()(2)+tfoutput[0].flat<float>()(3);
	//std::cout << result_top << " "<<result_qcd << std::endl;
	itopdisc[jindex]=result_top/(result_top+result_qcd);
	//std::cout <<  "ITOP "<<itopdisc[jindex] << std::endl;


  	tensorflow::Status statusMD = tfsessionMD_->Run({ { "input_1", input_image }, {"input_2", input_b} },{ "k2tfout_0" }, {}, &tfoutputMD);

	if (!statusMD.ok()) 
		{
		std::cout << "Tensorflow Failed:" << std::endl;
  		std::cout << statusMD.ToString() << "\n";
  		return ;
		}	
        //float result_topMD = tfoutputMD[0].flat<float>()(0);
        //float result_qcdMD = tfoutputMD[0].flat<float>()(1);
        float result_topMD = tfoutput[0].flat<float>()(4)+tfoutput[0].flat<float>()(5)+tfoutput[0].flat<float>()(6);
        float result_qcdMD = tfoutput[0].flat<float>()(0)+tfoutput[0].flat<float>()(1)+tfoutput[0].flat<float>()(2)+tfoutput[0].flat<float>()(3);


	itopdiscMD[jindex]=result_topMD/(result_topMD+result_qcdMD);
	}
	jindex+=1;


  }


  //Add to jet userfloats
  auto outputs = std::make_unique<pat::JetCollection>();
  jindex=0;
  for (const auto &jet : *jets){
    pat::Jet newJet(jet);
    //std::cout <<  "ITOPWRITE "<<itopdisc[jindex] << std::endl;
    newJet.addUserFloat("Image"+extex_+":top", itopdisc[jindex]);
    newJet.addUserFloat("ImageMD"+extex_+":top", itopdiscMD[jindex]);
    newJet.addUserFloat("Image"+extex_+":DRave",DRaves[jindex]);
    outputs->push_back(newJet);
    jindex+=1;
  }


  // put into the event
  iEvent.put(std::move(outputs));

}

//define this as a plug-in
DEFINE_FWK_MODULE(ImageProducer);
