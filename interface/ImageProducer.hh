
bool signalmatch(TLorentzVector curtlv ,const std::vector<reco::GenParticle>* genpartsvec ,std::string stype)
		{
		bool fulltopmatch=false;

		if(stype=="top")
			{

			int ntops=0;

		  	for (auto &gp : *genpartsvec)
				{
			
				int gpid = gp.pdgId();


				if(fulltopmatch==true)break;
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
							//std::cout<<gpid<<" - "<<ntops<<std::endl;
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
					if (abs(gp.daughter(0)->pdgId())==5)
						{
						wid=1;
						bid=0;
						}
					gpblv.SetPtEtaPhiM(gp.daughter(bid)->pt(),gp.daughter(bid)->eta(),gp.daughter(bid)->phi(),gp.daughter(bid)->mass());

					if(!(curtlv.DeltaR(gpblv)<0.8))continue;
				
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

								if ((curtlv.DeltaR(gpq1lv)<0.8) && (curtlv.DeltaR(gpq2lv)<0.8))fulltopmatch=true;
								else return false;
								}

							else return false;
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


				if(fulltopmatch==true)break;
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
					if (abs(gp.daughter(0)->pdgId())==22)
						{
						wid=1;
						bid=0;
						}

					gpblv.SetPtEtaPhiM(gp.daughter(bid)->pt(),gp.daughter(bid)->eta(),gp.daughter(bid)->phi(),gp.daughter(bid)->mass());
					if(!(curtlv.DeltaR(gpblv)<0.8))continue;
				
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

								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw->daughter(0)->pt(),gpw->daughter(0)->eta(),gpw->daughter(0)->phi(),gpw->daughter(0)->mass());


								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw->daughter(1)->pt(),gpw->daughter(1)->eta(),gpw->daughter(1)->phi(),gpw->daughter(1)->mass());

								if ((curtlv.DeltaR(gpq1lv)<0.8) && (curtlv.DeltaR(gpq2lv)<0.8))fulltopmatch=true;
								else  return false;;
								}

							else return false;
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
			


				if(fulltopmatch==true)break;
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
	
				
					auto gpw0 = gp.daughter(0);
					auto gpw1 = gp.daughter(1);
					bool found0 = false;
					bool found1 = false;

					while(fulltopmatch==false)
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
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw0->daughter(0)->pt(),gpw0->daughter(0)->eta(),gpw0->daughter(0)->phi(),gpw0->daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw0->daughter(1)->pt(),gpw0->daughter(1)->eta(),gpw0->daughter(1)->phi(),gpw0->daughter(1)->mass());
								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
								if ((curtlv.DeltaR(gpq1lv)<0.8) && (curtlv.DeltaR(gpq2lv)<0.8))found0=true;
								else  return false;;
								}
							else return false;
							
							}
													
						if(gpw1->numberOfDaughters()==1) gpw1=gpw1->daughter(0);	
						else
							{
							//std::cout<<"twodecw1 "<<gpw1->daughter(0)->pdgId()<<","<<gpw1->daughter(1)->pdgId()<<std::endl;
							if((abs(gpw1->daughter(0)->pdgId())==24)) gpw1=gpw1->daughter(0);	
							else if((abs(gpw1->daughter(1)->pdgId())==24)) gpw1=gpw1->daughter(1);
							else if ((abs(gpw1->daughter(0)->pdgId())<6) && (abs(gpw1->daughter(1)->pdgId())<6)) 
								{
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gpw1->daughter(0)->pt(),gpw1->daughter(0)->eta(),gpw1->daughter(0)->phi(),gpw1->daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gpw1->daughter(1)->pt(),gpw1->daughter(1)->eta(),gpw1->daughter(1)->phi(),gpw1->daughter(1)->mass());
								//std::cout<<"DRS "<<curtlv.DeltaR(gpq1lv)<<","<<curtlv.DeltaR(gpq2lv)<<std::endl;
								if ((curtlv.DeltaR(gpq1lv)<0.8) && (curtlv.DeltaR(gpq2lv)<0.8))found1=true;
								else  return false;;
								}
							else return false;
							
							}
						if(found0 and found1)fulltopmatch=true;

						}
					}
				}
			}//stype
		else
			{
			std::cout<<"no matching"<<std::endl;
		 	}
		return fulltopmatch;
	}


