#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "phMath.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2.h"
#include "mxGeometry.h"
#include "mxReconstruction.h"
#include "mxQAReconstruction.h"

/////////////////////////////
//QA of Geant4 Simulation
//
// 1. Read in TTree from MPCEXfile.root
// 2. Create energy vs minipad histogram
//
////////////////////////////

int main()
{
  /*
  ifstream ifile;
  ifile.open("/Users/stacykarthas/Geant4/geant4.10.02.p01/examples/basic/MPCEXsim-build/100direction.txt");
  if (!ifile) {
    std::cerr << "Can't open input file " <<"100direction.txt" << std::endl;
      exit(1);
    }
    double x;
    double y;
    double z;
    const int length = 100;
    double xarray[length];
    double yarray[length];
    double zarray[length];
    int i = 0;
    while (!ifile.eof()) {
      ifile >> x >> y >> z;
      if (!ifile.good()) break;
      xarray[i] = x;
      yarray[i] = y;
      zarray[i] = z;
      //std::cout << x << " " << y << " " << z << std::endl;
      i++;
    }
    ifile.close();
  */      
  mxGeometry *geo = new mxGeometry();
  mxReconstruction *Reco = new mxReconstruction();
  mxQAReconstruction *QAReco = new mxQAReconstruction();
  
  //note that we use "new" to create the TFile and TTree objects !
  //because we want to keep these objects alive when we leave this function.
  TFile *f = new TFile("MPCEXfile.root");
  TTree *t1 = (TTree*)f->Get("MPCEXfile");

  Int_t mxhits, mhits;
  Double_t mxen, men;
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

  //create histograms
  TH1D *minien   = new TH1D("minien","Minipad Energy; Key; Energy",49152,-0.5,49151.5);
  TH1D *minihits = new TH1D("minihits","Minipad Hits; Key; Hits", 49152,-0.5,49151.5);
  TH1D *mpcen    = new TH1D("mpcen","MPC Crystal Energy; Crystal id; Energy", 576,-0.5,575.5);
  TH1D *mpchits  = new TH1D("mpchits","MPC Crystal Hits; Crystal id; Hits", 576, -0.5,575.5);
  TH2F *mpcdisplay = new TH2F("mpcdisp","MPC Crystal Hits;X;Y",61,-30,30,61,-30,30);
  TH1F *mpcexenhist   = new TH1F("mpcexenhist","MPCEX Energy; Energy;", 2000,0,1);
  TH1F *mpcenhist   = new TH1F("mpcenhist","MPC Energy; Energy(MeV);", 100,0,1.5e5);
  TH2F *aero[18];
  for(int i=0; i!=9; ++i) {
    if(i%2==0) {
      aero[i]   = new TH2F( Form("S%d",i), Form("S%d",i), 32*8, -18, +18, 32, -18, +18 );
      aero[9+i] = new TH2F( Form("N%d",i), Form("N%d",i), 32*8, -18, +18, 32, -18, +18 );
    } else {
      aero[i]   = new TH2F( Form("S%d",i), Form("S%d",i), 32, -18, +18, 32*8, -18, +18 );
      aero[9+i] = new TH2F( Form("N%d",i), Form("N%d",i), 32, -18, +18, 32*8, -18, +18 );
    }
  }

  //Create Display Canvas
  TCanvas *MPCEXD = new TCanvas();
  MPCEXD->Divide(3,3);
  MPCEXD->SaveAs("MPCEXDisplay.pdf[","pdf");

  //read all entries and fill the histograms
  Long64_t nentries = t1->GetEntries();
  for (Long64_t i=0;i<nentries;i++) {
    std::cout <<"|*|*|*|*|*|*| Event: " << i << std::endl;

    MPCEXD->Clear();
    MPCEXD->Divide(3,3);
    //Reset Histograms
    for(int n=0; n!=8; ++n) {
      aero[n]->Reset();
      aero[9+n]->Reset();
      //      MPCEXD->cd(n);
      //      MPCEXD->Clear();
    }
    mpcdisplay->Reset();
    Reco->Reset();

    //Load the Tree and Get Entries
    Long64_t tentry = t1->LoadTree(i);
    bmmxen->GetEntry(tentry);
    bcmhits->GetEntry(tentry);
    bcmen->GetEntry(tentry);
    t1->GetEntry(i);

    
    double mpcexenergy = mxen;
    double mpcenergy = men;
    
    //sums variables
    double totalminienergy = 0;
    double totalmpcenergy = 0;
    double energyarray[49152+288*2] = {0.0};

    //loop over MPCEX events
    for (UInt_t j = 0; j < mmxhits->size(); ++j){
      minihits->Fill(mmxhits->at(j));
      minien->Fill(mmxhits->at(j), mmxen->at(j));
      //      std::cout << mmxhits->at(j) << " " << mmxen->at(j) << std::endl;
      totalminienergy +=mmxen->at(j);
      energyarray[mmxhits->at(j)] = mmxen->at(j);
      mpcexenhist->Fill(mmxen->at(j));
    }

    //Loop over MPC events
    for (UInt_t k = 0; k < cmhits->size(); ++k){
      mpcen->Fill(cmhits->at(k),cmen->at(k));
      mpchits->Fill(cmhits->at(k));
      totalmpcenergy +=cmen->at(k);
      mpcdisplay->Fill(geo->X(49152+cmhits->at(k)),geo->Y(49152+cmhits->at(k)),cmen->at(k));
      energyarray[49152+cmhits->at(k)] = cmen->at(k);
      //      std::cout << cmhits->at(k)+49152 << " " << cmen->at(k) << std::endl;
      //      mpcenhist->Fill(cmen->at(k));
    }
    //std::cout << std::endl;
    //    std::cout <<"I made it here"<< std::endl;
    //Reco for MPCEX events
    for (int m = 0; m < 49152+288*2;m++){
      if (energyarray[m] != 0.0) {
	Reco->Fill(m, energyarray[m]);
	aero[geo->LyrIdx(m)]->Fill(geo->X(m),geo->Y(m),energyarray[m]);
      }
    }
    Reco->Make();
    Reco->DumpParties();
    Reco->DumpStats();
    QAReco->Make(Reco);

    std::cout << "MPCEX ENERGY " << mpcexenergy << " | MPCEX TOTAL ENERGY " << totalminienergy << std::endl;
    std::cout << "MPC ENERGY " << mpcenergy << " |MPC TOTAL ENERGY " << totalmpcenergy << std::endl; 
    std::cout << "ENERGY " << mpcexenergy + mpcenergy << " |TOTAL ENERGY " << totalminienergy + totalmpcenergy << std::endl;

    mpcenhist->Fill(mpcexenergy + mpcenergy);
    
    //Draw detector Display
    /*    MPCEXD->cd(1); aero[9]->Draw("colz");
    MPCEXD->cd(2); aero[10]->Draw("colz");
    MPCEXD->cd(3); aero[11]->Draw("colz");
    MPCEXD->cd(4); aero[12]->Draw("colz");
    MPCEXD->cd(5); aero[13]->Draw("colz");
    MPCEXD->cd(6); aero[14]->Draw("colz");
    MPCEXD->cd(7); aero[15]->Draw("colz");
    MPCEXD->cd(8); aero[16]->Draw("colz");*/
    MPCEXD->cd(1); aero[0]->Draw("colz");
    MPCEXD->cd(2); aero[1]->Draw("colz");
    MPCEXD->cd(3); aero[2]->Draw("colz");
    MPCEXD->cd(4); aero[3]->Draw("colz");
    MPCEXD->cd(5); aero[4]->Draw("colz");
    MPCEXD->cd(6); aero[5]->Draw("colz");
    MPCEXD->cd(7); aero[6]->Draw("colz");
    MPCEXD->cd(8); aero[7]->Draw("colz");
    MPCEXD->cd(9); gPad->SetLogz(); mpcdisplay->Draw("colz");
    //std::cout << "HERE" << std::endl;
    MPCEXD->SaveAs("MPCEXDisplay.pdf","pdf");
    // std::cout << "HERE" << std::endl;
    //    MPCEXD->Clear();
  }
  //std::cout << "HERE" << std::endl;
  MPCEXD->SaveAs("MPCEXDisplay.pdf]","pdf");
  // std::cout << "HERE" << std::endl;
  //Draw Energy Histograms
  TCanvas *minicanvas = new TCanvas();
  //  minien->Draw("same");
  mpcexenhist->Draw();
  minicanvas->SaveAs("MiniCanvas.eps");
  TCanvas *mpccanvas = new TCanvas();
  //  mpcen->Draw();
  mpcenhist->Draw();
  mpccanvas->SaveAs("MpcCanvas.eps");

  std::cout << "HERE" << std::endl;
  /*  TH2F *mpcdisplay2 = new TH2F("mpcdisp2","MPC Crystal Displays;X;Y",61,-30,30,61,-30,30);
  for (int jj = 0; jj<575; jj++){
  if (geo->Z(49152+jj)>1){
  mpcdisplay2->Fill(geo->X(49152+jj),geo->Y(49152+jj),jj);
  }
  }
  new TCanvas;
  mpcdisplay2->Draw("colz");
  */

  //Save Reco Histograms
  QAReco->GetList()->SaveAs("ouput.root");

  //Reset Tree
  t1->ResetBranchAddresses();
  return 0;
}
