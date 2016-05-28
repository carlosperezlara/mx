#include <cstdlib>
#include <iostream>
#include <fstream>

#include "mxReconstruction.h"

int main() {
  mxReconstruction *reco = new mxReconstruction();

  std::ifstream input("input.dat");
  int hits, idx;
  float sgn;
  for(;;) {
    input >> hits;
    if(!input.good()) break;
    reco->Reset();
    // reading event
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;
      //std::cout << idx << " " << sgn << std::endl;;
      reco->Fill(idx,sgn);
    }
    reco->Make();
    reco->DumpStats();
    reco->DumpParties();
    break;
  }
  input.close();
  return 0;
}