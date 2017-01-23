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
	TH1F *recdphi = new TH1F("recdphi","recdphi",101,-0.5,+0.5);
	TH1F *recdeta = new TH1F("recdeta","recdeta",101,-0.5,+0.5);
	TH1F *seldphi = new TH1F("seldphi","seldphi",101,-0.5,+0.5);
	TH1F *seldeta = new TH1F("seldeta","seldeta",101,-0.5,+0.5);

  gErrorIgnoreLevel = kWarning;
  TString file = "input";
  float maxe = 100;
  int combo = 1;
  if(narg>1) {
    file = carg[1];
  }
  if(narg>2) {
    TString sMaxE = carg[2];
    combo = sMaxE.Atof();
  }
  if(narg>3) {
    TString sMaxE = carg[3];
    maxe = sMaxE.Atof();
  }
  mxReconstruction *reco = new mxReconstruction();
  //reco->SetDebug(2);
  reco->SetIdentificationAlgorithm(combo);
  mxQAReconstruction *QAReco = new mxQAReconstruction(maxe);

  mxCoalitionCuts *cuts0 = new mxCoalitionCuts("Cut0");
  cuts0->SetEneMax( maxe );
  cuts0->SetQA();
  mxCoalitionCuts *cuts1 = cuts0->Clone("Cut1");
  cuts1->SetEneMax( maxe );
  cuts1->SetQA();
  cuts1->Set_HitLayer(5);
  cuts1->Set_HitLayer(6);
  cuts1->Set_HitLayer(7);
  mxCoalitionCuts *cuts2 = cuts1->Clone("Cut2");
  cuts2->SetEneMax( maxe );
  cuts2->SetQA();
  cuts2->Set_PS_minChi2Prob(0.2);
  mxCoalitionCuts *cuts3 = cuts2->Clone("Cut3");
  cuts3->SetEneMax( maxe );
  cuts3->SetQA();
  cuts3->Set_HitLayer(8);

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
  //std::cout << "INIT" << std::endl;
  for(int nevs=0;;++nevs) {
    input >> hits;
    inputP >> hitP;
    if(!input.good()) break;
    //if(!inputP.good()) break;
    reco->Reset();
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;
      //if(idx<49152 && sgn<0.003) continue;
      //if(idx>49152 && sgn<0.3) continue;
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
    //std::cout << "DUMPING" << std::endl;
    //reco->DumpParties();
    //if(nevs>2) break;

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
	      outputC << arm << " " << coalition->GetEnergy() << " " << coalition->GetPhi() << " " << coalition->GetTheta() << " " << coalition->GetEta() << " ";
	      outputC << coalition->GetPhiVar() << " " << coalition->GetThetaVar() << " " << coalition->GetEtaVar() << " ";
	      outputC << coalition->SignalPreShower() << " " << coalition->NPreShower();
	      outputC << std::endl;
	      cuts0->PassesCuts(coalition);
	      cuts1->PassesCuts(coalition);
	      cuts2->PassesCuts(coalition);
	      cuts3->PassesCuts(coalition);
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
  std::cout << "OUTPUT: " << Form("%s_%d_qa.root",file.Data(),combo) << std::endl;
  TFile *ofile = new TFile( Form("%s_%d_qa.root",file.Data(),combo), "RECREATE" );
  gen->Write();
  recdphi->Write();
  recdeta->Write();
  seldphi->Write();
  seldeta->Write();
  TList *c;
  c = QAReco->GetList();
  for(int i=0; i!=c->GetEntries(); ++i) (c->At(i))->Write();
  c = cuts0->GetList();
  for(int i=0; i!=c->GetEntries(); ++i) (c->At(i))->Write();
  c = cuts1->GetList();
  for(int i=0; i!=c->GetEntries(); ++i) (c->At(i))->Write();
  c = cuts2->GetList();
  for(int i=0; i!=c->GetEntries(); ++i) (c->At(i))->Write();
  c = cuts3->GetList();
  for(int i=0; i!=c->GetEntries(); ++i) (c->At(i))->Write();
  ofile->Close();
  return 0;
}
