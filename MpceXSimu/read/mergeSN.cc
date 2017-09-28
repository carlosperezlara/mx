#include <fstream>
#include <iostream>
#include "TString.h"

int main(int narg, char **carg) {
  TString file = "MPCEXfile";
  if(narg>1) {
    file = carg[1];
  }
  TString sif1 = Form("%ss.hit",file.Data());
  TString sif2 = Form("%sn.hit",file.Data());
  TString sif3 = Form("%ss.prim",file.Data());
  TString sif4 = Form("%sn.prim",file.Data());
  TString sof1 = Form("%s.hit",file.Data());
  TString sof2 = Form("%s.prim",file.Data());

  std::cout << "INPUT1: " << sif1.Data() << std::endl;
  std::cout << "INPUT3: " << sif2.Data() << std::endl;
  std::cout << "INPUT2: " << sif3.Data() << std::endl;
  std::cout << "INPUT4: " << sif4.Data() << std::endl;
  std::ifstream if1( sif1.Data() );
  std::ifstream if2( sif2.Data() );
  std::ifstream if3( sif3.Data() );
  std::ifstream if4( sif4.Data() );
  std::ofstream of1( sof1.Data() );
  std::ofstream of2( sof2.Data() );

  for(;;) {
    int nA, nB, mA, mB;
    if1 >> nA; // As.hit
    if2 >> nB; // An.hit
    if3 >> mA; // As.prim
    if4 >> mB; // An.prim
    if(!if1.good()) break;
    if(!if2.good()) break;
    if(!if3.good()) break;
    if(!if4.good()) break;
    //===HITS
    of1 << nA+nB << std::endl; // A.hit
    int key;
    float ene;
    for(int i=0; i!=nA; ++i) {
      if1 >> key >> ene;
      of1 << key << " " << ene << std::endl;
    }
    for(int i=0; i!=nB; ++i) {
      if2 >> key >> ene;
      of1 << key << " " << ene << std::endl;
    }
    //===PART
    of2 << mA+mB << std::endl; // A.prim
    int pdg;
    float phi, eta;
    for(int i=0; i!=mA; ++i) {
      if3 >> pdg >> ene >> phi >> eta;
      of2 << pdg << " " << ene << " " << phi << " " << eta << std::endl;
    }
    for(int i=0; i!=mB; ++i) {
      if4 >> pdg >> ene >> phi >> eta;
      of2 << pdg << " " << ene << " " << phi << " " << eta << std::endl;
    }
  }

  std::cout << "OUTPUT1: " << sof1.Data() << std::endl;
  std::cout << "OUTPUT2: " << sof2.Data() << std::endl;

  return 0;
}
