std::mutex write_mutex;
std::pair<int,float> signalmatch(TLorentzVector curtlv ,TLorentzVector curtlvb ,const std::vector<reco::GenParticle>* genpartsvec ,std::string stype ,float mergeval)
		{
		int fulltopmatch=0;
		//float maxdr=-1;
		float avedr=-1;	
		if(stype=="w")
			{
			int nws=0;
			for (auto &gp : *genpartsvec)
					{
				


					if(fulltopmatch)break;
					int gpid = gp.pdgId();
				

					TLorentzVector gplv;
					//std::cout<<abs(gpid)<<std::endl;
					if(abs(gpid)==24)
						{
						//std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
						gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
						//std::cout<<"ndauz "<<gp.numberOfDaughters()<<std::endl;
						if((gp.numberOfDaughters()>1)) 
							{


							std::cout<<abs(gp.daughter(0)->pdgId())<<" "<<abs(gp.daughter(1)->pdgId())<<std::endl;
							if ((abs(gp.daughter(0)->pdgId())<6) && (abs(gp.daughter(1)->pdgId())<6)) 
								{

								nws+=1;
								//std::cout<<"DRlv "<<gplv.DeltaR(curtlv)<<std::endl;
								if(gplv.DeltaR(curtlv)<0.6)
									{
									TLorentzVector gpq1lv;
									gpq1lv.SetPtEtaPhiM(gp.daughter(0)->pt(),gp.daughter(0)->eta(),gp.daughter(0)->phi(),gp.daughter(0)->mass());

									TLorentzVector gpq2lv;
									gpq2lv.SetPtEtaPhiM(gp.daughter(1)->pt(),gp.daughter(1)->eta(),gp.daughter(1)->phi(),gp.daughter(1)->mass());

									//std::cout<<"DRgps "<<gplv.DeltaR(gpq1lv)<<" "<<gplv.DeltaR(gpq2lv)<<std::endl;
									//std::cout<<"DRcurtlvs "<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq2lv)<<std::endl;
									avedr = std::max(curtlv.DeltaR(gpq1lv),curtlv.DeltaR(gpq2lv));
									if ((curtlv.DeltaR(gpq1lv)<mergeval) && (curtlv.DeltaR(gpq2lv)<mergeval))
										{
										fulltopmatch=1;	
										//std::cout<<"pdg0 "<<gp.daughter(0)->pdgId()<<" pdg1 "<<gp.daughter(1)->pdgId()<<std::endl;
										if ((abs(gp.daughter(0)->pdgId())==4) || (abs(gp.daughter(1)->pdgId())==4))fulltopmatch=2;
										//std::cout<<"return "<<fulltopmatch<<" "<<avedr<<std::endl;
										return  std::pair<int,float>(fulltopmatch,avedr);
										}
									}
								
								}
							else if ((abs(gp.daughter(0)->pdgId())==24) || (abs(gp.daughter(1)->pdgId())==24))continue;
							else 
								{

								//std::cout<<"nws "<<nws<<std::endl;
								nws+=1; 
								continue;
								}
							}
						}
					if(nws>1)
						{
						//std::cout<<"nomatch "<<std::endl; 
						//break;
						return std::pair<int,float>(0,0.0);
						}
					}
			}
		else if(stype=="wb")
			{

			int ntops=0;

		  	for (auto &gp : *genpartsvec)
				{
			
				int gpid = gp.pdgId();


				if(fulltopmatch)break;
				bool gpmatch=false;

			
				TLorentzVector gplv;
				if(abs(gpid)==6)
					{
				
					gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
					if((gp.numberOfDaughters()>1)) 
						{
						if((abs(gp.daughter(0)->pdgId())==24) || (abs(gp.daughter(1)->pdgId())==24))
							{
							ntops+=1;

							if(gplv.DeltaR(curtlv+curtlvb)<0.6)gpmatch=true;
							}

						}
					}
			
			
				if(gpmatch==true)
					{

					int wid = -1;
					int bid = -1;
					TLorentzVector gpblv;
					if (abs(gp.daughter(1)->pdgId())==5)
						{
						wid=0;
		  				bid=1;
						}
					else if (abs(gp.daughter(0)->pdgId())==5)
						{
						wid=1;
						bid=0;
						}
					else continue;

					gpblv.SetPtEtaPhiM(gp.daughter(bid)->pt(),gp.daughter(bid)->eta(),gp.daughter(bid)->phi(),gp.daughter(bid)->mass());

					if(!((curtlv+curtlvb).DeltaR(gpblv)<mergeval))continue;

					auto gpw = gp.daughter(wid);

					while(fulltopmatch==false)
						{

						if(gpw->numberOfDaughters()==1)
							{
								gpw=gpw->daughter(0);
								continue;
							}
						if(gpw->numberOfDaughters()>1)
							{

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
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw->daughter(0)->pt(),gpw->daughter(0)->eta(),gpw->daughter(0)->phi(),gpw->daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw->daughter(1)->pt(),gpw->daughter(1)->eta(),gpw->daughter(1)->phi(),gpw->daughter(1)->mass());

								avedr=(gpq1lv.DeltaR(gpq2lv)+gpblv.DeltaR(gpq1lv)+gpblv.DeltaR(gpq2lv))/3.0;
								//std::cout<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq2lv)<<std::endl;
								std::vector<float> wdrs = {};
								std::vector<float> bdrs = {};
								std::vector<float> mindrs = {};
								wdrs.push_back(curtlv.DeltaR(gpq1lv));
								wdrs.push_back(curtlv.DeltaR(gpq2lv));
								wdrs.push_back(curtlv.DeltaR(gpblv));
								bdrs.push_back(curtlvb.DeltaR(gpq1lv));
								bdrs.push_back(curtlvb.DeltaR(gpq2lv));
								bdrs.push_back(curtlvb.DeltaR(gpblv));
								mindrs.push_back(std::min(wdrs[0],bdrs[0]));
								mindrs.push_back(std::min(wdrs[1],bdrs[1]));
								mindrs.push_back(std::min(wdrs[2],bdrs[2]));
								//std::cout<<"wds "<<wdrs[0]<<" "<<wdrs[1]<<" "<<wdrs[2]<<std::endl;
								//std::cout<<"bds "<<bdrs[0]<<" "<<bdrs[1]<<" "<<bdrs[2]<<std::endl;
								if (wdrs[0]<bdrs[0] and wdrs[1]<bdrs[1] and wdrs[2]>bdrs[2] )fulltopmatch=1;
								else if ((wdrs[0]<bdrs[0] and wdrs[1]>bdrs[1] and wdrs[2]<bdrs[2]) or (wdrs[0]>bdrs[0] and wdrs[1]<bdrs[1] and wdrs[2]<bdrs[2]))fulltopmatch=2;
								else fulltopmatch=3;
								avedr = std::max(mindrs[2],(std::max(mindrs[0],mindrs[1])));
								//std::cout<<"return "<<fulltopmatch<<" "<<avedr<<std::endl;
								if (avedr<mergeval)return std::pair<int,float>(fulltopmatch,avedr);
								else return std::pair<int,float>(false,avedr);
								}
							else return std::pair<int,float>(false,avedr);

							}
			
						}


				
					}
				if(ntops>1)break;
				}
			}//stype





		//std::cout<<"return "<<fulltopmatch<<" "<<avedr<<std::endl;
		return  std::pair<int,float>(fulltopmatch,avedr);
	}


