#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TString.h"
#include "TList.h"
#include "TError.h"

#include "mxReconstruction.h"
#include "mxParty.h"
#include "mxQAReconstruction.h"

int main(int narg, char **carg) {
  gErrorIgnoreLevel = kWarning;
  mxReconstruction *reco = new mxReconstruction();
  mxQAReconstruction *QAReco = new mxQAReconstruction();

  TString file = "input";
  if(narg>1) {
    file = carg[1];
  }
  std::cout << "INPUT: " << Form("%s.hit",file.Data()) << std::endl;
  std::ifstream input( Form("%s.hit",file.Data()));
  std::ofstream outputP(Form("%s.pty",file.Data()));
  //std::ofstream outputC(Form("%s.coa",file.Data()));
  //std::ofstream outputU(Form("%s.uni",file.Data()));

  int hits, idx;
  float sgn;
  for(;;) {
    input >> hits;
    if(!input.good()) break;
    reco->Reset();
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;
      reco->Fill(idx,sgn);
    }
    reco->Make();
    //reco->DumpStats();
    //reco->DumpHits();
    //reco->DumpParties();
    QAReco->Make(reco);

    int ntot=0;
    for(int lyr=0; lyr!=18; ++lyr)
      ntot += reco->GetNParties(lyr);
    outputP << ntot << std::endl;
    mxParty *party;
    for(int lyr=0; lyr!=18; ++lyr) {
      int n = reco->GetNParties(lyr);
      for(int i=0; i!=n; ++i) {
	party = reco->GetParty(lyr,i);
	outputP << lyr << " " << party->GetX() << " " << party->GetY() << " " << party->Signal() << " ";
	outputP << party->GetCov(0) << " " << party->GetCov(1) << " " << party->GetCov(2) << " ";
	outputP << party->GetSpreadX() << " " << party->GetSpreadY() << " ";
	outputP << std::endl;
      }
    }
  }
  input.close();
  outputP.close();
  std::cout << "OUTPUT: " << Form("%s.pty",file.Data()) << std::endl;
  //std::cout << "OUTPUT: " << Form("%s.coa",file.Data()) << std::endl;
  //std::cout << "OUTPUT: " << Form("%s.uni",file.Data()) << std::endl;
  std::cout << "OUTPUT: " << Form("%s_qa.root",file.Data()) << std::endl;
  QAReco->GetList()->SaveAs( Form("%s_qa.root",file.Data()) );

  return 0;
}
