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
  std::cout << "INPUT: " << Form("%s.hit",file.Data()) << std::endl;
  std::ifstream input( Form("%s.hit",file.Data()));
  std::ofstream outputA(Form("%s_ex_energy.raw",file.Data()));
  std::ofstream outputB(Form("%s_mpc_energy.raw",file.Data()));
  int hits, idx;
  float sgn;
  for(;;) {
    input >> hits;
    if(!input.good()) break;
    float ex = 0;
    float mpc = 0;
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;
      if(idx<49152) ex += sgn;
      else mpc += sgn;
    }
    outputA << ex << std::endl;
    outputB << mpc << std::endl;
  }
  input.close();
  outputA.close();
  outputB.close();
  std::cout << "OUTPUT: " << Form("%s_ex_energy.raw",file.Data()) << std::endl;
  std::cout << "OUTPUT: " << Form("%s_mpc_energy.raw",file.Data()) << std::endl;
  return 0;
}
