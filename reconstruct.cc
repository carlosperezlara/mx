#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TString.h"
#include "TList.h"
#include "TError.h"

#include "phMath.h"

#include "mxParty.h"
#include "mxReconstruction.h"
#include "mxQAReconstruction.h"

int main(int narg, char **carg) {
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
  mxQAReconstruction *QAReco = new mxQAReconstruction(maxe);

  std::cout << "INPUT: " << Form("%s.hit",file.Data()) << std::endl;
  std::cout << "INPUT: " << Form("%s.prim",file.Data()) << std::endl;
  std::ifstream input( Form("%s.hit",file.Data()));
  std::ifstream inputP( Form("%s.prim",file.Data()));
  std::ofstream outputP(Form("%s.pty",file.Data()));
  //std::ofstream outputC(Form("%s.coa",file.Data()));
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
      reco->FillPP(penergy,peta,pphi,ppdg);
      // std::cout << penergy << std::endl;
    }
    
    reco->DumpStats();
    reco->Make();
    //reco->DumpHits();
    //reco->DumpParties();
    QAReco->Make(reco);

    //    std::cout << penergy << std::endl;
    
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
    //break;
  }
  input.close();
  inputP.close();
  outputP.close();
  std::cout << "OUTPUT: " << Form("%s.pty",file.Data()) << std::endl;
  //std::cout << "OUTPUT: " << Form("%s.coa",file.Data()) << std::endl;
  //std::cout << "OUTPUT: " << Form("%s.uni",file.Data()) << std::endl;
  std::cout << "OUTPUT: " << Form("%s_qa.root",file.Data()) << std::endl;
  QAReco->GetList()->SaveAs( Form("%s_qa.root",file.Data()) );

  return 0;
}
