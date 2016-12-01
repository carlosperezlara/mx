#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

int main(int narg, char **carg) {
  TString file = "MPCEXfile";
  if(narg>1) {
    file = carg[1];
  }
  std::cout << "INPUT: " << Form("%s.root",file.Data()) << std::endl;
  TFile *f = new TFile(Form("%s.root",file.Data()));
  TTree *t1 = (TTree*)f->Get("MPCEXfile");
  Int_t mxhits, mhits;
  Double_t mxen, men;
  Double_t ppenergy, ppeta, ppphi;
  Int_t pppdg;
  std::vector<int> *mmxhits=0;
  std::vector<int> *cmhits=0;
  std::vector<double> *mmxen, *cmen;

  TBranch *bmmxhits = 0;
  TBranch *bcmhits = 0;
  TBranch *bmmxen = 0;
  TBranch *bcmen = 0;
  
  //Set Branch Addresses
  t1->SetBranchAddress("MPCEXHits",&mxhits);
  t1->SetBranchAddress("MPCHits",&mhits);
  t1->SetBranchAddress("MPCEXEnergy",&mxen);
  t1->SetBranchAddress("MPCEnergy",&men);
  t1->SetBranchAddress("MPCEXHitminipads",&mmxhits,&bmmxhits);
  t1->SetBranchAddress("MPCEXEnergysens",&mmxen,&bmmxen);
  t1->SetBranchAddress("MPCHitcrystals",&cmhits,&bcmhits);
  t1->SetBranchAddress("MPCEnergysens",&cmen,&bcmen);
  t1->SetBranchAddress("PPEnergy",&ppenergy);
  t1->SetBranchAddress("PPEta",&ppeta);
  t1->SetBranchAddress("PPPhi",&ppphi);
  t1->SetBranchAddress("PPpdg",&pppdg);
  
  std::ofstream fout(Form("%s.hit",file.Data()));
  std::ofstream pout(Form("%s.prim",file.Data()));
  //Read all entries and fill the histograms
  Long64_t nentries = t1->GetEntries();
  for(Long64_t i=0;i<nentries;i++) {
    Long64_t tentry = t1->LoadTree(i);
    bmmxen->GetEntry(tentry);
    bcmhits->GetEntry(tentry);
    bcmen->GetEntry(tentry);
    t1->GetEntry(i);
    double energyarray[49152+288*2] = {0};
    for(UInt_t j=0; j!=mmxhits->size(); ++j) // EX
      energyarray[mmxhits->at(j)] += mmxen->at(j);
    for(UInt_t k = 0; k < cmhits->size(); ++k) // MPC
      energyarray[49152+cmhits->at(k)] += cmen->at(k);
    int nhits = 0;
    for(int j=0; j!=49152+288*2; ++j) 
      if(energyarray[j]>1e-6)
	nhits++;
    fout << nhits << std::endl;
    for(int j=0; j!=49152+288*2; ++j) 
      if(energyarray[j]>1e-6)
	fout << j << " " << energyarray[j]/1000 << std::endl; // to GeV
    pout << 1 << std::endl;
    pout << pppdg <<" "<<ppenergy << " " << ppeta << " " << ppphi << std::endl;
  }
  fout.close();
  std::cout << "OUTPUT: " << Form("%s.hit",file.Data()) << std::endl;
  std::cout << "OUTPUT: " << Form("%s.prim",file.Data()) << std::endl;
  return 0;
}
