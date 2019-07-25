
std::pair<int,float> signalmatch(TLorentzVector curtlv ,const std::vector<reco::GenParticle>* genpartsvec ,float mergeval)
		{
		int fulltopmatch=0;
		//float maxdr=-1;
		float avedr=-1;	
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


						std::cout<<abs(gp.daughter(0)->pdgId())<<" "<<abs(gp.daughter(1)->pdgId())<<std::endl;
						if ((abs(gp.daughter(0)->pdgId())<6) && (abs(gp.daughter(1)->pdgId())<6)) 
							{

							nws+=1;
							std::cout<<"DRlv "<<gplv.DeltaR(curtlv)<<std::endl;
							if(gplv.DeltaR(curtlv)<0.6)
								{
								TLorentzVector gpq1lv;
								gpq1lv.SetPtEtaPhiM(gp.daughter(0)->pt(),gp.daughter(0)->eta(),gp.daughter(0)->phi(),gp.daughter(0)->mass());

								TLorentzVector gpq2lv;
								gpq2lv.SetPtEtaPhiM(gp.daughter(1)->pt(),gp.daughter(1)->eta(),gp.daughter(1)->phi(),gp.daughter(1)->mass());

								std::cout<<"DRs "<<gplv.DeltaR(gpq1lv)<<" "<<gplv.DeltaR(gpq2lv)<<std::endl;
								if ((gplv.DeltaR(gpq1lv)<mergeval) && (gplv.DeltaR(gpq2lv)<mergeval))
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

		return  std::pair<int,float>(fulltopmatch,avedr);
	}


