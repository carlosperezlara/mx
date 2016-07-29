#include <iostream>
#include <vector>
#include <algorithm>

#include "phMath.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2.h"
#include "AeroView.h"
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
  mxGeometry *geo = new mxGeometry();
  mxReconstruction *Reco = new mxReconstruction();
  mxQAReconstruction *QAReco = new mxQAReconstruction();
  
  //note that we use "new" to create the TFile and TTree objects !
  //because we want to keep these objects alive when we leave this function.
  TFile *f = new TFile("/Users/stacykarthas/Geant4/geant4.10.02.p01/examples/basic/MPCEXsim-build/B2a/MPCEXfile.root");
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
  // TFile *outfile2 = new TFile("outfile2.root", "RECREATE");
  //create histograms
  TH1D *minien   = new TH1D("minien","Minipad Energy; Key; Energy",49152,-0.5,49151.5);
  TH1D *minihits = new TH1D("minihits","Minipad Hits; Key; Hits", 49152,-0.5,49151.5);
  TH1D *mpcen    = new TH1D("mpcen","MPC Crystal Energy; Crystal id; Energy", 576,-0.5,575.5);
  TH1D *mpchits  = new TH1D("mpchits","MPC Crystal Hits; Crystal id; Hits", 576, -0.5,575.5);
  TH2F *mpcdisplay = new TH2F("mpcdisp","MPC Crystal Hits;X;Y",61,-30,30,61,-30,30);
  
  //create MPCEX display

  AeroView *display = new AeroView();
  
  //read all entries and fill the histograms
  Long64_t nentries = t1->GetEntries();
  for (Long64_t i=0;i<nentries;i++) {
    Reco->Reset();
    Long64_t tentry = t1->LoadTree(i);

    bmmxhits->GetEntry(tentry);
    bmmxen->GetEntry(tentry);
    bcmhits->GetEntry(tentry);
    bcmen->GetEntry(tentry);
    t1->GetEntry(i);
    double mpcexenergy = mxen;//.GetEntry(tentry);
    double mpcenergy = men;//.GetEnty(tentry);
    
    //sums variables
    double totalminienergy = 0;
    double totalmpcenergy = 0;
    
    for (UInt_t j = 0; j < mmxhits->size(); ++j){
      minihits->Fill(mmxhits->at(j));
      minien->Fill(mmxhits->at(j), mmxen->at(j));
      totalminienergy +=mmxen->at(j);
      display->Fill(mmxhits->at(j),mmxen->at(j)); //Fill display hits
      Reco->Fill(mmxhits->at(j),mmxen->at(j));
    }
    for (UInt_t k = 0; k < cmhits->size(); ++k){
      mpcen->Fill(cmhits->at(k),cmen->at(k));
      mpchits->Fill(cmhits->at(k));
      totalmpcenergy +=cmen->at(k);
      mpcdisplay->Fill(geo->X(49152+cmhits->at(k)),geo->Y(49152+cmhits->at(k)),cmen->at(k));
      Reco->Fill(cmhits->at(k)+49152,cmen->at(k));
    }
    Reco->Make();
    QAReco->Make(Reco);
    std::cout << "MPCEX ENERGY " << mpcexenergy << " | MPCEX TOTAL ENERGY " << totalminienergy << std::endl;
    std::cout << "MPC ENERGY " << mpcenergy << " |MPC TOTAL ENERGY " << totalmpcenergy << std::endl; 
    std::cout << "ENERGY " << mpcexenergy + mpcenergy << " |TOTAL ENERGY " << totalminienergy + totalmpcenergy << std::endl;
  }
  //////////
  //Draw Energy Histograms
  //////////
  TCanvas *minicanvas = new TCanvas();
  //  minien->Draw();
  //minihits->SetLineColor(2);
  minien->Draw("same");
  minicanvas->SaveAs("MiniCanvas.eps");
  TCanvas *mpccanvas = new TCanvas();
  mpcen->Draw();
  //mpchits->SetLineColor(2);
  mpccanvas->SaveAs("MpcCanvas.eps");
 
  ///////////
  //Draw MPCEX Display
  ///////////

  TH2D *al[2][8];
  TCanvas *main = new TCanvas();
  main->SetLogz();
  main->Divide(3,3,0,0);
  for(int l=0; l!=8; ++l) {
    main->cd( l+1 );
    al[1][l] = display->GetLayer(1,l);
    al[1][l]->Draw("colz");
  }

  main->SaveAs("MPCEXDisplay.eps");
  
  TCanvas *mpcdispcanvas = new TCanvas();
  mpcdisplay->Draw("colz");
  mpcdispcanvas->SaveAs("MPCDisplay.eps");


    // outfile2->Write();
  /*  TH2F *mpcdisplay2 = new TH2F("mpcdisp2","MPC Crystal Displays;X;Y",61,-30,30,61,-30,30);
  for (int jj = 0; jj<575; jj++){
    if (geo->Z(49152+jj)>1){
      mpcdisplay2->Fill(geo->X(49152+jj),geo->Y(49152+jj),jj);
    }
    }
  new TCanvas;
  mpcdisplay2->Draw("colz");
  */

  QAReco->GetList()->SaveAs("ouput.root");
  
  t1->ResetBranchAddresses();

  return 0;
}
