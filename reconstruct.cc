//=========================
// written by Carlos Perez 
// 2015-2016               
//=========================
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TString.h"
#include "TList.h"
#include "TError.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TFile.h"

#include "phMath.h"

#include "mxGeometry.h"
#include "mxParty.h"
#include "mxReconstruction.h"
#include "mxQAReconstruction.h"
#include "mxCoalitionCuts.h"

int main(int narg, char **carg) {
	mxGeometry *geo = new mxGeometry();

	TH1F *gen = new TH1F("gen","gen",100,0,101);

	TH1F *rec = new TH1F("rec","rec",100,0,101);
	TH1F *recX = new TH1F("recX","recX",100,0,0.5);
	TH1F *recN = new TH1F("recN","recN",9,-0.5,8.5);
	TH1F *recdphi = new TH1F("recdphi","recdphi",101,-0.5,+0.5);
	TH2F *recprob = new TH2F("recprob","recprob",100,0,0.5,100,0,1);
	TH1F *recdeta = new TH1F("recdeta","recdeta",101,-0.5,+0.5);
	TProfile *rechits = new TProfile("rechits","rechits",9,-0.5,8.5);
	TProfile *recene = new TProfile("recene","recene",9,-0.5,8.5);
	TH2F *recXpro = new TH2F("recXpro","recXpro",9,-0.5,8.5,101,-5,+5);

	TH1F *sel = new TH1F("sel","sel",100,0,101);
	TH1F *selX = new TH1F("selX","selX",100,0,0.5);
	TH1F *selN = new TH1F("selN","selN",9,-0.5,8.5);
	TH1F *seldphi = new TH1F("seldphi","seldphi",101,-0.5,+0.5);
	TH2F *selprob = new TH2F("selprob","selprob",100,0,0.5,100,0,1);
	TH1F *seldeta = new TH1F("seldeta","seldeta",101,-0.5,+0.5);
	TProfile *selhits = new TProfile("selhits","selhits",9,-0.5,8.5);
	TProfile *selene = new TProfile("selene","selene",9,-0.5,8.5);
	TH2F *selXpro = new TH2F("selXpro","selXpro",9,-0.5,8.5,101,-5,+5);

  gErrorIgnoreLevel = kWarning;
  TString file = "input";
  float maxe = 100;
  if(narg>1) {
    file = carg[1];
  }
  if(narg>2) {
    TString sMaxE = carg[2];
    maxe = sMaxE.Atof();
  }
  mxReconstruction *reco = new mxReconstruction();
  if(1) {
	  reco->SetPartyAlgorithm(1);
  	reco->SetCoalitionAlgorithm(1);
  } else {
	  reco->SetPartyAlgorithm(0);
  	reco->SetCoalitionAlgorithm(0);
  }
  mxQAReconstruction *QAReco = new mxQAReconstruction(maxe);
  mxCoalitionCuts *cuts = new mxCoalitionCuts();
	cuts->Set_HitLayer(5);
	cuts->Set_HitLayer(6);
	cuts->Set_HitLayer(7);
	cuts->Set_PS_minChi2Prob(0.2);
	cuts->Set_PS_minSignal(0.05);

  std::cout << "INPUT: " << Form("%s.hit",file.Data()) << std::endl;
  std::cout << "INPUT: " << Form("%s.prim",file.Data()) << std::endl;
  std::ifstream input( Form("%s.hit",file.Data()));
  std::ifstream inputP( Form("%s.prim",file.Data()));
  std::ofstream outputP(Form("%s.pty",file.Data()));
  std::ofstream outputC(Form("%s.coa",file.Data()));
  //std::ofstream outputU(Form("%s.uni",file.Data()));

  int hitP,ppdg;
  int hits, idx;
  float sgn;
  float penergy,peta,pphi;
  for(;;) {
    input >> hits;
    inputP >> hitP;
    if(!input.good()) break;
    if(!inputP.good()) break;
    reco->Reset();
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;
      if(idx>49152 && sgn<0.3) continue;
      reco->Fill(idx,sgn);
    }
    for(int l=0; l!=hitP; ++l) {
      inputP >> ppdg >> penergy >> pphi >> peta;
      penergy /= 1000;
      gen->Fill(penergy);
      reco->FillPP(penergy,peta,pphi,ppdg);
      //std::cout << penergy << std::endl;

    }
    reco->Make();
    QAReco->Make(reco);

    std::cout << "DUMPING" << std::endl;

    //dumping parties
    int ntot=0;
    for(int lyr=0; lyr!=18; ++lyr)
      ntot += reco->GetNParties(lyr);
    outputP << ntot << std::endl;
    mxParty *party;
    for(int lyr=0; lyr!=18; ++lyr) {
      int n = reco->GetNParties(lyr);
      for(int i=0; i!=n; ++i) {
				party = reco->GetParty(lyr,i);
				outputP << lyr << " " << party->GetX() << " " << party->GetY() << " ";
				outputP << party->Signal() << " ";
				outputP << party->GetCov(0) << " " << party->GetCov(1) << " " << party->GetCov(2) << " ";
				outputP << party->GetSpreadX() << " " << party->GetSpreadY() << " ";
				outputP << std::endl;
      }
    }

    //dumping coalitions
    ntot=0;
    for(int arm=0; arm!=2; ++arm)
      ntot += reco->GetNCoalitions(arm);
    outputC << ntot << std::endl;
    mxCoalition *coalition;
    for(int arm=0; arm!=2; ++arm) {
      int n = reco->GetNCoalitions(arm);
      for(int i=0; i!=n; ++i) {
	      coalition = reco->GetCoalition(arm,i);
	      outputC << coalition->GetEnergy() << " " << coalition->GetPhi() << " " << coalition->GetTheta() << " " << coalition->GetEta() << " ";
	      outputC << coalition->GetPhiVar() << " " << coalition->GetThetaVar() << " " << coalition->GetEtaVar() << " ";
	      outputC << coalition->SignalPreShower() << " " << coalition->NPreShower();
	      outputC << std::endl;
	      rec->Fill( coalition->GetEnergy() );
	      recX->Fill( coalition->SignalPreShower() );
	      recprob->Fill( coalition->SignalPreShower(), coalition->GetPSChi2Prob() );
	      recN->Fill( coalition->N() );
	      recdphi->Fill( coalition->GetPhi() - pphi );
	      recdeta->Fill( coalition->GetEta() - peta );
	      for(int j=0; j!=9; ++j) {
	      	mxParty *pty = coalition->GetParty(j);
	      	if(!pty) continue;
		      rechits->Fill(j,pty->N());
		      recene->Fill(j,pty->Signal());
		      if(j==8) continue;
		      for(int k=0; k!=pty->N(); ++k) {
		      	mxHit *hit = pty->GetHit(k);
		      	if(j%2==0) recXpro->Fill(j, geo->X(hit->Idx()) - pty->GetX() );
		      	if(j%2==1) recXpro->Fill(j, geo->Y(hit->Idx()) - pty->GetY() );
		  	  }
	  	  }
		    //selection
	      if(cuts->PassesCuts(coalition)) {
      		sel->Fill( coalition->GetEnergy() );
      		selX->Fill( coalition->SignalPreShower() );
		      selprob->Fill( coalition->SignalPreShower(), coalition->GetPSChi2Prob() );
		      selN->Fill( coalition->N() );
		      seldphi->Fill( coalition->GetPhi() - pphi );
		      seldeta->Fill( coalition->GetEta() - peta );
		      for(int j=0; j!=9; ++j) {
		      	mxParty *pty = coalition->GetParty(j);
		      	if(!pty) continue;
			      selhits->Fill(j,pty->N());
			      selene->Fill(j,pty->Signal());
			      if(j==8) continue;
			      for(int k=0; k!=pty->N(); ++k) {
			      	mxHit *hit = pty->GetHit(k);
			      	if(j%2==0) selXpro->Fill(j, geo->X(hit->Idx()) - pty->GetX() );
			      	if(j%2==1) selXpro->Fill(j, geo->Y(hit->Idx()) - pty->GetY() );
			  	  }
		  	  }
	      	//here!
	      }
      }
    }

    //break;
  }
  input.close();
  inputP.close();
  outputP.close();
  outputC.close();
  std::cout << "OUTPUT: " << Form("%s.pty",file.Data()) << std::endl;
  std::cout << "OUTPUT: " << Form("%s.coa",file.Data()) << std::endl;
  //std::cout << "OUTPUT: " << Form("%s.uni",file.Data()) << std::endl;
  std::cout << "OUTPUT: " << Form("%s_qa.root",file.Data()) << std::endl;
  QAReco->GetList()->SaveAs( Form("%s_qa.root",file.Data()) );
  TFile *ofile = new TFile( Form("%s_eff.root",file.Data()), "RECREATE" );
  gen->Write();
  rec->Write();
  recX->Write();
  recprob->Write();
  recN->Write();
  recdphi->Write();
  recdeta->Write();
  rechits->Write();
  recene->Write();
  recXpro->Write();

  sel->Write();
  selX->Write();
  selprob->Write();
  selN->Write();
  seldphi->Write();
  seldeta->Write();
  selhits->Write();
  selene->Write();
  selXpro->Write();

  ofile->Close();
  return 0;
}
