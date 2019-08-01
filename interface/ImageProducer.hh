std::mutex write_mutex;
std::pair<int,float> signalmatch(TLorentzVector curtlv ,const std::vector<reco::GenParticle>* genpartsvec ,std::string stype,float mergeval)
		{
		int fulltopmatch=0;
		float maxdr=-1;
		float avedr=-1;
		/*
		if(stype=="QCD")
			{
		  	for (auto &gp : *genpartsvec)
				{
				int gpid = gp.pdgId();	

				TLorentzVector gplv;
				gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				if(gplv.DeltaR(curtlv)<0.6)
					{
					std::cout<<gplv.Pt()<<" "<<gpid<<" M "<<gp.mother()->pdgId()<<std::endl;
					if (abs(gpid)<=3)return std::pair<int,float>(1,0.0);
					if (abs(gpid)==4)return std::pair<int,float>(2,0.0);
					if (abs(gpid)==5)return std::pair<int,float>(3,0.0);
					if (abs(gpid)==21)	{
								std::cout<<"GLU! "<<std::endl;
								auto gpdau = &gp; 
								while(fulltopmatch==0)	{
											if(gpdau->numberOfDaughters()==1) 	{
															
																gpdau = dynamic_cast<const reco::GenParticle*>(gpdau->daughter(0));
																continue;
																}
											if(gpdau->numberOfDaughters()>1) 	{
																std::cout<< gpdau->daughter(0)->pdgId()<<" "<< gpdau->daughter(0)->pdgId()<<std::endl;
																if(abs( gpdau->daughter(0)->pdgId())==4 or abs( gpdau->daughter(1)->pdgId())==4)return std::pair<int,float>(2,0.0);
																else if(abs( gpdau->daughter(0)->pdgId())==5 or abs( gpdau->daughter(1)->pdgId())==4)return std::pair<int,float>(3,0.0);
																else return std::pair<int,float>(4,0.0);
																}
								
								
											}
								}

						
					}
				}
			}//stype
		*/


		//std::cout<<stype<<std::endl;
		if(stype=="HRTSEL")
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

							if(gplv.DeltaR(curtlv)<0.6)gpmatch=true;
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

					if(!(curtlv.DeltaR(gpblv)<mergeval))continue;

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
								maxdr=std::max(std::max(gpq1lv.DeltaR(curtlv),gpblv.DeltaR(curtlv)),gpq2lv.DeltaR(curtlv));
								return std::pair<int,float>(true,maxdr);
								
								}
							else return std::pair<int,float>(false,avedr);

							}
			
						}


				
					}
				if(ntops>1)break;
				}
			}//stype


		else if(stype=="top")
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

							if(gplv.DeltaR(curtlv)<0.6)gpmatch=true;
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

					if(!(curtlv.DeltaR(gpblv)<mergeval))continue;

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
								maxdr=std::max(std::max(gpq1lv.DeltaR(gpq2lv),gpblv.DeltaR(gpq1lv)),gpblv.DeltaR(gpq2lv));
								avedr=(gpq1lv.DeltaR(gpq2lv)+gpblv.DeltaR(gpq1lv)+gpblv.DeltaR(gpq2lv))/3.0;
								//std::cout<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq2lv)<<std::endl;
								if ((curtlv.DeltaR(gpq1lv)<mergeval) && (curtlv.DeltaR(gpq2lv)<mergeval))
									{
									//std::cout<<"2Fs "<<abs(gpw->daughter(0)->pdgId())<<"," <<abs(gpw->daughter(1)->pdgId())<<std::endl;
									fulltopmatch=1;
									if(abs(gpw->daughter(0)->pdgId())==4 || abs(gpw->daughter(1)->pdgId())==4)fulltopmatch=2;
									}
								else if ((curtlv.DeltaR(gpq1lv)<mergeval) || (curtlv.DeltaR(gpq2lv)<mergeval))
									{
									fulltopmatch=3;
									if ((curtlv.DeltaR(gpq1lv)<curtlv.DeltaR(gpq2lv)) and (abs(gpw->daughter(0)->pdgId()==4)))fulltopmatch=4;
									else if ((curtlv.DeltaR(gpq1lv)>curtlv.DeltaR(gpq2lv)) and (abs(gpw->daughter(1)->pdgId()==4)))fulltopmatch=4;
									}
								else return std::pair<int,float>(false,avedr);
								}
							else return std::pair<int,float>(false,avedr);

							}
			
						}


				
					}
				if(ntops>1)break;
				}
			}//stype


		else if(stype=="pho")
			{
			int nphos=0;
		  	for (auto &gp : *genpartsvec)
				{
			
				int gpid = gp.pdgId();


				if(fulltopmatch)break;
				bool gpmatch=false;

				
				TLorentzVector gplv;
				if(abs(gpid)==9000025)
					{
				
					gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
					if((gp.numberOfDaughters()>1)) 
						{
						if((abs(gp.daughter(0)->pdgId())==22) || (abs(gp.daughter(1)->pdgId())==22))
							{
							nphos+=1;
							//std::cout<<gpid<<" - "<<nphos<<std::endl;
							if(gplv.DeltaR(curtlv)<0.6)gpmatch=true;
							}

						}
					}
			
			

				if(gpmatch==true)
					{
					int wid = -1;
					int bid = -1;

					TLorentzVector gpblv;
					if (abs(gp.daughter(1)->pdgId())==22)
						{
						wid=0;
		  				bid=1;
						}
					else if (abs(gp.daughter(0)->pdgId())==22)
						{
						wid=1;
						bid=0;
						}
					else continue;

					gpblv.SetPtEtaPhiM(gp.daughter(bid)->pt(),gp.daughter(bid)->eta(),gp.daughter(bid)->phi(),gp.daughter(bid)->mass());
					if(!(curtlv.DeltaR(gpblv)<mergeval))continue;
				
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

							if((abs(gpw->daughter(0)->pdgId())==23)) 
								{
								gpw=gpw->daughter(0);
								continue;
								}
							if((abs(gpw->daughter(1)->pdgId())==23))
								{
								gpw=gpw->daughter(1);
								continue;
								}
							if ((abs(gpw->daughter(0)->pdgId())<6) && (abs(gpw->daughter(1)->pdgId())<6)) 
								{
								//std::cout<<abs(gpw->daughter(0)->pdgId())<<" "<<abs(gpw->daughter(1)->pdgId())<<std::endl;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw->daughter(0)->pt(),gpw->daughter(0)->eta(),gpw->daughter(0)->phi(),gpw->daughter(0)->mass());


								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw->daughter(1)->pt(),gpw->daughter(1)->eta(),gpw->daughter(1)->phi(),gpw->daughter(1)->mass());
								maxdr=std::max(std::max(gpq1lv.DeltaR(gpq2lv),gpblv.DeltaR(gpq1lv)),gpblv.DeltaR(gpq2lv));
								avedr=(gpq1lv.DeltaR(gpq2lv)+gpblv.DeltaR(gpq1lv)+gpblv.DeltaR(gpq2lv))/3.0;
								if ((curtlv.DeltaR(gpq1lv)<mergeval) && (curtlv.DeltaR(gpq2lv)<mergeval))	{
																		if ((abs(gpw->daughter(0)->pdgId())==5) && (abs(gpw->daughter(1)->pdgId())==5)) fulltopmatch=3;
																		else if ((abs(gpw->daughter(0)->pdgId())==4) && (abs(gpw->daughter(1)->pdgId())==4)) fulltopmatch=2;
																		else fulltopmatch=1;
																		}
								else if ((curtlv.DeltaR(gpq1lv)<mergeval) || (curtlv.DeltaR(gpq2lv)<mergeval))fulltopmatch=4;
								else  return std::pair<int,float>(false,avedr);
								}

							else return std::pair<int,float>(false,avedr);
							}
			
						}
					}
				if(nphos>0)break;
				}
			}//stype


		else if(stype=="ww")
			{		
			int ntops=0;
		  	for (auto &gp : *genpartsvec)
				{
			

				if(fulltopmatch)break;
				bool gpmatch=false;
				int gpid = gp.pdgId();
			

				TLorentzVector gplv;
				//if((gp.numberOfDaughters()>1)) std::cout<<(gp.daughter(0))->pdgId()<<","<<(gp.daughter(1))->pdgId()<<std::endl;
				//if(abs(gpid)==9000024) std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
				if(abs(gpid)==9000025)
					{
					//std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
					gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
					if((gp.numberOfDaughters()>1)) 
						{
						if((abs(gp.daughter(0)->pdgId())==24) && (abs(gp.daughter(1)->pdgId())==24))
							{
							ntops+=1;
							if(gplv.DeltaR(curtlv)<0.6)gpmatch=true;
							}

						}
					}
			
				if(gpmatch==true)
					{
	
				
					std::vector<TLorentzVector> alltlvs;
					auto gpw0 = gp.daughter(0);
					auto gpw1 = gp.daughter(1);
					bool foundhad0 = false;
					bool foundhad1 = false;
					bool foundlep0 = false;
					bool foundlep1 = false;
					while(fulltopmatch==0)
						{
						//std::cout<<"curw0 "<<gpw0->pdgId()<<std::endl;
						//std::cout<<"curw1 "<<gpw1->pdgId()<<std::endl;

						if(gpw0->numberOfDaughters()==1) gpw0=gpw0->daughter(0);	
						else
							{
							//std::cout<<"twodecw0 "<<gpw0->daughter(0)->pdgId()<<","<<gpw0->daughter(1)->pdgId()<<std::endl;

							if((abs(gpw0->daughter(0)->pdgId())==24)) gpw0=gpw0->daughter(0);	
							else if((abs(gpw0->daughter(1)->pdgId())==24)) gpw0=gpw0->daughter(1);
							else if ((abs(gpw0->daughter(0)->pdgId())<6) && (abs(gpw0->daughter(1)->pdgId())<6)) 
								{

								foundhad0=true;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw0->daughter(0)->pt(),gpw0->daughter(0)->eta(),gpw0->daughter(0)->phi(),gpw0->daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw0->daughter(1)->pt(),gpw0->daughter(1)->eta(),gpw0->daughter(1)->phi(),gpw0->daughter(1)->mass());

								alltlvs.push_back(gpq1lv);
								alltlvs.push_back(gpq2lv);

								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
							
								}
							else if ((10<abs(gpw0->daughter(0)->pdgId()) && abs(gpw0->daughter(0)->pdgId())<19) && (10<abs(gpw0->daughter(1)->pdgId()) && abs(gpw0->daughter(1)->pdgId())<19)) 
								{
								foundlep0=true;

								//std::cout<<abs(gpw0->daughter(0)->pdgId())<<std::endl;
								//std::cout<<abs(gpw0->daughter(1)->pdgId())<<std::endl;
								int neun=1;
								if(abs(gpw0->daughter(1)->pdgId())%2==0)neun=0;

								//std::cout<<abs(gpw0->daughter(1)->pdgId())<<" "<<neun<<std::endl;
								TLorentzVector gpq1lv;
								//std::cout<<"lepid "<<abs(gpw0->daughter(neun)->pdgId())<<std::endl;
								gpq1lv.SetPtEtaPhiM(gpw0->daughter(neun)->pt(),gpw0->daughter(neun)->eta(),gpw0->daughter(neun)->phi(),gpw0->daughter(neun)->mass());

								alltlvs.push_back(gpq1lv);
								if(curtlv.DeltaR(gpq1lv)>mergeval)return std::pair<int,float>(false,avedr);
								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
						
								}
							else return std::pair<int,float>(false,avedr);
							
							}
													
						if(gpw1->numberOfDaughters()==1) gpw1=gpw1->daughter(0);	
						else
							{

							//std::cout<<"twodecw1 "<<gpw1->daughter(0)->pdgId()<<","<<gpw1->daughter(1)->pdgId()<<std::endl;
							if((abs(gpw1->daughter(0)->pdgId())==24)) gpw1=gpw1->daughter(0);	
							else if((abs(gpw1->daughter(1)->pdgId())==24)) gpw1=gpw1->daughter(1);
							else if ((abs(gpw1->daughter(0)->pdgId())<6) && (abs(gpw1->daughter(1)->pdgId())<6)) 
								{
								foundhad1=true;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw1->daughter(0)->pt(),gpw1->daughter(0)->eta(),gpw1->daughter(0)->phi(),gpw1->daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw1->daughter(1)->pt(),gpw1->daughter(1)->eta(),gpw1->daughter(1)->phi(),gpw1->daughter(1)->mass());

								alltlvs.push_back(gpq1lv);
								alltlvs.push_back(gpq2lv);

								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
						
								}
							else if ((10<abs(gpw1->daughter(0)->pdgId()) && abs(gpw1->daughter(0)->pdgId())<19) && (10<abs(gpw1->daughter(1)->pdgId()) && abs(gpw1->daughter(1)->pdgId())<19)) 
								{
								foundlep1=true;
								//std::cout<<abs(gpw1->daughter(0)->pdgId())<<std::endl;
								//std::cout<<abs(gpw1->daughter(1)->pdgId())<<std::endl;
								int neun=1;
								if(abs(gpw1->daughter(1)->pdgId())%2==0)neun=0;
								//std::cout<<abs(gpw1->daughter(1)->pdgId())<<" "<<neun<<std::endl;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw1->daughter(neun)->pt(),gpw1->daughter(neun)->eta(),gpw1->daughter(neun)->phi(),gpw1->daughter(neun)->mass());
								//std::cout<<"lepid "<<abs(gpw1->daughter(neun)->pdgId())<<std::endl;
								alltlvs.push_back(gpq1lv);

								if(curtlv.DeltaR(gpq1lv)>mergeval)return std::pair<int,float>(false,avedr);
								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl
							
								}
							else return std::pair<int,float>(false,avedr);
							
							}
						if((foundhad0 and foundlep1)or(foundhad1 and foundlep0)or(foundhad0 and foundhad1)) 
							{
							//std::cout<<"foundem"<<std::endl;
							float maxdrfromlv = -1.0;
							float sumdr = 0.0;
							float numsum = 0.0;
							int nabove = 0;
							for (auto jj : alltlvs) 
								{								
								if(curtlv.DeltaR(jj)>mergeval)	nabove+=1;						
								maxdrfromlv=std::max(maxdrfromlv,float(curtlv.DeltaR(jj)));									
								for (auto ii : alltlvs) 
									{
									//std::cout<<ii.DeltaR(jj)<<std::endl;
									
									maxdr=std::max(maxdr,float(ii.DeltaR(jj)));	
									if (ii!=jj)
										{
										sumdr+=ii.DeltaR(jj);
										numsum+=1.0;
										}
									}
								}
							//std::cout<<"Ms"<<std::endl;
							//std::cout<<maxdrfromlv<<std::endl;
							//std::cout<<maxdr<<std::endl;
							avedr = sumdr/numsum;
							if (maxdrfromlv>0.0){
										if((foundhad0 and foundlep1)or(foundhad1 and foundlep0))
											{
											if(nabove==1)fulltopmatch=3;
											else if(maxdrfromlv<mergeval)fulltopmatch=4;
											else return std::pair<int,float>(false,avedr);
											return  std::pair<int,float>(fulltopmatch,avedr);
											}
										else
											{
											if(nabove==1)fulltopmatch=1;
											else if(maxdrfromlv<mergeval)fulltopmatch=2;
											else return std::pair<int,float>(false,avedr);
											return  std::pair<int,float>(fulltopmatch,avedr);
											}
									     }
							else return std::pair<int,float>(false,avedr);
							
							}
						if((foundlep1 and foundlep0)) return std::pair<int,float>(false,avedr);
						


						}
					}
				}
			}//stype
		else if(stype=="w")
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
				
					if((gp.numberOfDaughters()>1)) 
						{
						//std::cout<<abs(gp.daughter(0)->pdgId())<<" "<<abs(gp.daughter(1)->pdgId())<<std::endl;
						if ((abs(gp.daughter(0)->pdgId())<6) && (abs(gp.daughter(1)->pdgId())<6)) 
							{

							nws+=1;
							if(gplv.DeltaR(curtlv)<0.6)
								{
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gp.daughter(0)->pt(),gp.daughter(0)->eta(),gp.daughter(0)->phi(),gp.daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gp.daughter(1)->pt(),gp.daughter(1)->eta(),gp.daughter(1)->phi(),gp.daughter(1)->mass());

								//std::cout<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq2lv)<<std::endl;
								if ((curtlv.DeltaR(gpq1lv)<mergeval) && (curtlv.DeltaR(gpq2lv)<mergeval))
									{
									fulltopmatch=1;	
									if ((abs(gp.daughter(0)->pdgId())==4) || (abs(gp.daughter(1)->pdgId())==4))fulltopmatch=2	;
									}
								}
							
							}
						else if ((abs(gp.daughter(0)->pdgId())==24) || (abs(gp.daughter(1)->pdgId())==24))continue;
						else 
							{
							nws+=1; 
							continue;
							}
						}
					}
				if(nws>1)break;
				}
			}//stype

		else if(stype=="z")
			{		
			int nzs=0;

		  	for (auto &gp : *genpartsvec)
				{
			


				if(fulltopmatch)break;
				int gpid = gp.pdgId();
			

				TLorentzVector gplv;
				//std::cout<<abs(gpid)<<std::endl;
				if(abs(gpid)==23)
					{
					//std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
					gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
					if((gp.numberOfDaughters()>1)) 
						{
						//std::cout<<abs(gp.daughter(0)->pdgId())<<" "<<abs(gp.daughter(1)->pdgId())<<std::endl;
						if ((abs(gp.daughter(0)->pdgId())<6) && (abs(gp.daughter(1)->pdgId())<6)) 
							{

							nzs+=1;
							if(gplv.DeltaR(curtlv)<0.6)
								{
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gp.daughter(0)->pt(),gp.daughter(0)->eta(),gp.daughter(0)->phi(),gp.daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gp.daughter(1)->pt(),gp.daughter(1)->eta(),gp.daughter(1)->phi(),gp.daughter(1)->mass());

								//std::cout<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq2lv)<<std::endl;
								if ((curtlv.DeltaR(gpq1lv)<mergeval) && (curtlv.DeltaR(gpq2lv)<mergeval))
									{
									fulltopmatch=1;	
									if ((abs(gp.daughter(0)->pdgId())==4) && (abs(gp.daughter(1)->pdgId())==4))fulltopmatch=2;
									if ((abs(gp.daughter(0)->pdgId())==5) && (abs(gp.daughter(1)->pdgId())==5))fulltopmatch=3;
									}

								}
							
							}
						else if ((abs(gp.daughter(0)->pdgId())==23) || (abs(gp.daughter(1)->pdgId())==23))continue;
						else 
							{
							nzs+=1; 
							continue;
							}
						}
					}
				if(nzs>1)break;
				}
			}//stype


		else if(stype=="h")
			{		
			int nhs=0;

		  	for (auto &gp : *genpartsvec)
				{
			


				if(fulltopmatch)break;
				int gpid = gp.pdgId();
			

				TLorentzVector gplv;
				//std::cout<<abs(gpid)<<std::endl;
				if(abs(gpid)==25)
					{
					//std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
					gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
					if((gp.numberOfDaughters()>1)) 
						{
						//std::cout<<abs(gp.daughter(0)->pdgId())<<" "<<abs(gp.daughter(1)->pdgId())<<std::endl;
						if ((abs(gp.daughter(0)->pdgId())<6) && (abs(gp.daughter(1)->pdgId())<6)) 
							{

							nhs+=1;
							if(gplv.DeltaR(curtlv)<0.6)
								{
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gp.daughter(0)->pt(),gp.daughter(0)->eta(),gp.daughter(0)->phi(),gp.daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gp.daughter(1)->pt(),gp.daughter(1)->eta(),gp.daughter(1)->phi(),gp.daughter(1)->mass());

								//std::cout<<curtlv.DeltaR(gpq1lv)<<" "<<curtlv.DeltaR(gpq2lv)<<std::endl;
								if ((curtlv.DeltaR(gpq1lv)<mergeval) && (curtlv.DeltaR(gpq2lv)<mergeval))
									{
									if ((abs(gp.daughter(0)->pdgId())==4) && (abs(gp.daughter(1)->pdgId())==4))fulltopmatch=1;
									if ((abs(gp.daughter(0)->pdgId())==5) && (abs(gp.daughter(1)->pdgId())==5))fulltopmatch=2;
									}
	
								

								}
							
							}
						else if ((abs(gp.daughter(0)->pdgId())==25) || (abs(gp.daughter(1)->pdgId())==25))continue;
						else 
							{
							nhs+=1; 
							continue;
							}
						}
					}
				if(nhs>1)break;
				}
				
			}//stype



		else if(stype=="hww")
			{		
			int ntops=0;
		  	for (auto &gp : *genpartsvec)
				{
			

				if(fulltopmatch)break;
				bool gpmatch=false;
				int gpid = gp.pdgId();
			

				TLorentzVector gplv;
				//if((gp.numberOfDaughters()>1)) std::cout<<(gp.daughter(0))->pdgId()<<","<<(gp.daughter(1))->pdgId()<<std::endl;
				//if(abs(gpid)==9000024) std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
				int wind = 0;
				int wdind0 = 0;
				int wdind1 = 0;
				if(abs(gpid)==25)
					{
					//std::cout<<abs(gpid)<< " " <<gp.mass()<<std::endl;
					gplv.SetPtEtaPhiM(gp.pt(),gp.eta(),gp.phi(),gp.mass());
				
					if((gp.numberOfDaughters()>2)) 
						{
						if((abs(gp.daughter(0)->pdgId())==24) || (abs(gp.daughter(1)->pdgId())==24) || (abs(gp.daughter(2)->pdgId())==24))
							{
							ntops+=1;

							if (abs(gp.daughter(0)->pdgId())==24)
									{
									wind=0;
									wdind0=1;
									wdind1=2;
									}
							if (abs(gp.daughter(1)->pdgId())==24)
									{
									wind=1;
									wdind0=0;
									wdind1=2;
									}
							if (abs(gp.daughter(2)->pdgId())==24)
									{
									wind=2;
									wdind0=0;
									wdind1=1;
									}
							if(gplv.DeltaR(curtlv)<0.6)gpmatch=true;
							}

						}
					}
			
				if(gpmatch==true)
					{
	
				
					std::vector<TLorentzVector> alltlvs;
					auto gpw0 = gp.daughter(wind);
					auto gpw10 = gp.daughter(wdind0);
					auto gpw11 = gp.daughter(wdind1);
					bool foundhad0 = false;
					bool foundhad1 = false;
					bool foundlep0 = false;
					bool foundlep1 = false;
					while(fulltopmatch==0)
						{
						//std::cout<<"curw0 "<<gpw0->pdgId()<<std::endl;
						//std::cout<<"curw1 "<<gpw1->pdgId()<<std::endl;

						if(gpw0->numberOfDaughters()==1) gpw0=gpw0->daughter(0);	
						else
							{
							//std::cout<<"twodecw0 "<<gpw0->daughter(0)->pdgId()<<","<<gpw0->daughter(1)->pdgId()<<std::endl;

							if((abs(gpw0->daughter(0)->pdgId())==24)) gpw0=gpw0->daughter(0);	
							else if((abs(gpw0->daughter(1)->pdgId())==24)) gpw0=gpw0->daughter(1);
							else if ((abs(gpw0->daughter(0)->pdgId())<6) && (abs(gpw0->daughter(1)->pdgId())<6)) 
								{

								foundhad0=true;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw0->daughter(0)->pt(),gpw0->daughter(0)->eta(),gpw0->daughter(0)->phi(),gpw0->daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw0->daughter(1)->pt(),gpw0->daughter(1)->eta(),gpw0->daughter(1)->phi(),gpw0->daughter(1)->mass());

								alltlvs.push_back(gpq1lv);
								alltlvs.push_back(gpq2lv);

								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
							
								}
							else if ((10<abs(gpw0->daughter(0)->pdgId()) && abs(gpw0->daughter(0)->pdgId())<19) && (10<abs(gpw0->daughter(1)->pdgId()) && abs(gpw0->daughter(1)->pdgId())<19)) 
								{
								foundlep0=true;

								//std::cout<<abs(gpw0->daughter(0)->pdgId())<<std::endl;
								//std::cout<<abs(gpw0->daughter(1)->pdgId())<<std::endl;
								int neun=1;
								if(abs(gpw0->daughter(1)->pdgId())%2==0)neun=0;

								//std::cout<<abs(gpw0->daughter(1)->pdgId())<<" "<<neun<<std::endl;
								TLorentzVector gpq1lv;
								//std::cout<<"lepid "<<abs(gpw0->daughter(neun)->pdgId())<<std::endl;
								gpq1lv.SetPtEtaPhiM(gpw0->daughter(neun)->pt(),gpw0->daughter(neun)->eta(),gpw0->daughter(neun)->phi(),gpw0->daughter(neun)->mass());

								alltlvs.push_back(gpq1lv);
								if(curtlv.DeltaR(gpq1lv)>mergeval)return std::pair<int,float>(false,avedr);
								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
						
								}
							else return std::pair<int,float>(false,avedr);
							
							}
						
						if ((abs(gpw10->pdgId())<6) && (abs(gpw11->pdgId())<6)) 
								{
								foundhad1=true;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw10->pt(),gpw10->eta(),gpw10->phi(),gpw10->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw11->pt(),gpw11->eta(),gpw11->phi(),gpw11->mass());

								alltlvs.push_back(gpq1lv);
								alltlvs.push_back(gpq2lv);

								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
						
								}
						else if ((10<abs(gpw10->pdgId()) && abs(gpw10->pdgId())<19) && (10<abs(gpw11->pdgId()) && abs(gpw11->pdgId())<19)) 
								{
								foundlep1=true;
								//std::cout<<abs(gpw10->pdgId())<<std::endl;
								//std::cout<<abs(gpw11->pdgId())<<std::endl;
								auto neun=gpw11;
								if(abs(gpw11->pdgId())%2==0) neun=gpw10;
								//std::cout<<abs(gpw11->pdgId())<<" "<<neun<<std::endl;
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(neun->pt(),neun->eta(),neun->phi(),neun->mass());
								//std::cout<<"lepid "<<abs(gpw1neun->pdgId())<<std::endl;
								alltlvs.push_back(gpq1lv);

								if(curtlv.DeltaR(gpq1lv)>mergeval)return std::pair<int,float>(false,avedr);
								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl
							
								}
						else return std::pair<int,float>(false,avedr);
							
							
						if((foundhad0 and foundlep1)or(foundhad1 and foundlep0)or(foundhad0 and foundhad1)) 
							{
							//std::cout<<"foundem"<<std::endl;
							float maxdrfromlv = -1.0;
							float sumdr = 0.0;
							float numsum = 0.0;
							int nabove = 0;
							for (auto jj : alltlvs) 
								{								
								if(curtlv.DeltaR(jj)>mergeval)	nabove+=1;						
								maxdrfromlv=std::max(maxdrfromlv,float(curtlv.DeltaR(jj)));									
								for (auto ii : alltlvs) 
									{
									//std::cout<<ii.DeltaR(jj)<<std::endl;
									
									maxdr=std::max(maxdr,float(ii.DeltaR(jj)));	
									if (ii!=jj)
										{
										sumdr+=ii.DeltaR(jj);
										numsum+=1.0;
										}
									}
								}
							//std::cout<<"Ms"<<std::endl;
							//std::cout<<maxdrfromlv<<std::endl;
							//std::cout<<maxdr<<std::endl;
							avedr = sumdr/numsum;

							if (maxdrfromlv>0.0){

										if((foundhad0 and foundlep1)or(foundhad1 and foundlep0))
											{
											//std::cout<< "foundhad0 " <<foundhad0<< " foundlep1 " <<foundlep1 << " foundhad1 " <<foundhad1<< " foundlep0 " <<foundlep0<<std::endl;
											if(nabove==1)fulltopmatch=5;
											else if(maxdrfromlv<mergeval)fulltopmatch=6;
											else return std::pair<int,float>(false,avedr);
											return  std::pair<int,float>(fulltopmatch,avedr);
											}
										else
											{
											if(nabove==1)fulltopmatch=3;
											else if(maxdrfromlv<mergeval)fulltopmatch=4;
											else return std::pair<int,float>(false,avedr);
											return  std::pair<int,float>(fulltopmatch,avedr);
											}
	
									     }
							else return std::pair<int,float>(false,avedr);
							
							}
						if((foundlep1 and foundlep0)) return std::pair<int,float>(false,avedr);
						


						}
					}

				if(ntops>0)break;
				}
			}//stype



		else
			{
			std::cout<<"no matching"<<std::endl;
		 	}
		return  std::pair<int,float>(fulltopmatch,avedr);
	}


