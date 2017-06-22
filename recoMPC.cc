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
#include "TChain.h"

#include "phMath.h"

#include "mxGeometry.h"
#include "mxParty.h"
#include "mxReconstruction.h"
#include "mxQAReconstruction.h"
#include "mxCoalitionCuts.h"

float mpcSF(float ene) {
  //float fr = 0.881-0.304*TMath::Exp(-0.124*ene);
  float fr = 0.651-0.452*TMath::Exp(-0.141*ene);
  return fr;
}

int main(int narg, char **carg) {
  TString dirr = "taxi/mb/step1/";
  TString dirs = "taxi/mb/step2/";
  //TString dirs = "taxi/mb/step3/";
  std::cout << "reading file from directory " << dirr.Data() << std::endl;
  mxGeometry *geo = new mxGeometry();
  gErrorIgnoreLevel = kWarning;
  TString file = "input.root";
  float maxe = 100;
  int combo = 5;
  if(narg>1) {
    file = carg[1];
  }
  float CUT_ASYM = 0.6;
  float CUT_DMIN = 2.0;
  float CUT_EMIN = -99;//10;
  float CUT_EMAX = 9999999;//15;

  float gains[416];
  ifstream gainsFile("gains.pass1");
  for(int i=0; i!=416; ++i)
    gainsFile >> gains[i];

  mxReconstruction *reco = new mxReconstruction();
  //reco->SetDebug(6);
  reco->SetIdentificationAlgorithm(combo);
  reco->SetPtyAlgMPCBreaker_NCrystals(5);
  reco->SetPtyAlgMPCBreaker_Threshold(0.01);
  mxCoalitionCuts *cuts0 = new mxCoalitionCuts("Cut0");
  //cuts0->SetEneMax( maxe );
  cuts0->SetQA();
  TChain *chain = new TChain("MPC");
  chain->AddFile( Form("%s%s",dirr.Data(),file.Data()) );
  Float_t vertexz, adc[416], bbcs, bbcn;
  UChar_t trigger;
  chain->SetBranchAddress("vertexZ",&vertexz);
  chain->SetBranchAddress("trigger",&trigger);
  chain->SetBranchAddress("bbcS",&bbcs);
  chain->SetBranchAddress("bbcN",&bbcn);
  chain->SetBranchAddress("adc",adc);
  TH1F *vtx = new TH1F("vertexz","vertexz",100,-10,+10);
  TH1F *mss = new TH1F("mass","mass",1000,0,2.2);

  //TH1F *phl = new TH1F("phiall","phiall",100,0,TMath::TwoPi());
  TH1F *ene = new TH1F("energy","energy",1000,0,40);
  TH2F *dvsm = new TH2F("d8VSmass","d8VSmass;mass;d8",1000,0,2.2,100,0.0,40);
  TH2F *avsm = new TH2F("asymmetryVSmass","asymmetryVSmass;mass;asymmetry",1000,0,2.2,100,-0.1,+1.1);
  TH2F *evsm = new TH2F("energyVSmass","energyVSmass;mass;energy",1000,0,2.2,100,0,40);
  TH2F *evsd = new TH2F("energyVSd8","energyVSd8;d8;energy",100,0,40,100,0,40);

  TH2F *dvsmmix = new TH2F("d8VSmassmix","d8VSmassmix;massmix;d8mix",1000,0,2.2,100,0.0,40);
  TH2F *avsmmix = new TH2F("asymmetryVSmassmix","asymmetryVSmassmix;massmix;asymmetrymix",1000,0,2.2,100,-0.1,+1.1);
  TH2F *evsmmix = new TH2F("energyVSmassmix","energyVSmassmix;massmix;energymix",1000,0,2.2,100,0,40);
  TH2F *evsdmix = new TH2F("energyVSd8mix","energyVSd8mix;d8mix;energymix",1000,0,40,100,0,40);
  
  TH2F *etvsm = new TH2F("energytVSmass","energytVSmass;mass;energyt",1000,0,2.2,100,0,2);
  //TH1F *uncotheta = new TH1F("unioncoalitiontheta","unioncoalitiontheta",100,3.0,3.2);
  //TH1F *uncophi = new TH1F("unioncoalitionphi","unioncoalitionphi",100,0,TMath::TwoPi());
  //TH1F *uncopx = new TH1F("unioncoalitionpx","unioncoalitionpx",100,0,2);

  TH1F *hmass[416], *hmassmix[416];
  TH2F *hap[416];
  for(int i=0; i!=416; ++i) {
    hmass[i] = new TH1F( Form("hmass_%03d",i),Form("hmass_%03d",i),1000,0,2.2);
    hmassmix[i] = new TH1F( Form("hmassmix_%03d",i),Form("hmassmix_%03d",i),1000,0,2.2);
    hap[i] = new TH2F( Form("henergyVSd8_%03d",i),Form("henergyVSd8_%03d;d8;energy",i),100,0,40,100,0,40);
  }
  TH2F *hmult = new TH2F("multMPCS","multMPCS",1000,0.1,150000,100,0,300);
  TH1F *hmult2 = new TH1F("multMPCS2","multMPCS2",1000,0.1,500);
  int nTot = chain->GetEntries();
  //for(int nevs=0;nevs!=100;++nevs) {
  for(int nevs=0;nevs!=nTot;++nevs) {
  //for(int nevs=5312;nevs!=5313;++nevs) {
    chain->GetEntry(nevs);
    // =========
    // CUTS
    if(trigger!=4) continue;
    // =========
    reco->Reset();
    reco->SetVertex(0,0,vertexz);
    for(int i=0; i!=416; ++i) {
      if(adc[i]>0.1) {
	float energy = adc[i];
	energy *= gains[i];
	reco->Fill(49152+i,energy);
      }
    }
    float mult = reco->GetMultiplicity(8);
    hmult->Fill( mult, bbcs );
    hmult2->Fill( mult );
    vtx->Fill( reco->GetVertexZ() );

    if(bbcs<100) continue;
    if(bbcs>200) continue;
    // =========

    //std::cout << "EVENT " << nevs << std::endl;
    //reco->DumpHits();

    reco->Parties();
    //reco->DumpParties();
    //reco->DumpMPCLayer(0);
    //continue;

    reco->Coalitions();

    //int ngam = reco->GetNCoalitions(0);
    //for(int i=0; i!=ngam; ++i) {
    //  mxCoalition *gam = reco->GetCoalition(0,i);
    //  phl->Fill( gam->GetPhi() );
    //}

    reco->Unions();
    //reco->DumpUnions();
    int npi0s = reco->GetNUnions(0);
    if(npi0s<1) continue; // does not histogram nor promote preevent

    //std::cout << "DUMPING=====>" << std::endl;
    //reco->DumpPreEventCoalitions();
    //reco->DumpCoalitions();

    for(int i=0; i!=npi0s; ++i) {
      mxUnion *pi0 = reco->GetUnion(0,i);
      //if(pi0->GetEnergy()<18) continue;
      //if(pi0->GetEnergy()>22) continue;
      float asymm = pi0->GetEnergyAsymmetry();
      float energ = pi0->GetEnergy();
      //float enert = pi0->GetEnergyT();
      float d8 = pi0->GetD8();
      if(TMath::Abs(asymm)>CUT_ASYM) continue;
      mss->Fill( pi0->GetMass() );
      ene->Fill( pi0->GetEnergy() );
      dvsm->Fill( pi0->GetMass(), pi0->GetD8() );
      avsm->Fill( pi0->GetMass(), pi0->GetEnergyAsymmetry() );
      evsm->Fill( pi0->GetMass(), pi0->GetEnergy() );
      evsd->Fill( pi0->GetEnergy(), pi0->GetD8() );
      etvsm->Fill( pi0->GetMass(), pi0->GetEnergyT() );
      if(energ<CUT_EMIN) continue;
      if(energ>CUT_EMAX) continue;
      if(d8<CUT_DMIN) continue; // remove small angles
      //if(d8>15.0) continue; // remove small angles
      mxCoalition *ccc = pi0->GetCoalition(0);
      //uncotheta->Fill( ccc->GetTheta() );
      //uncophi->Fill( ccc->GetPhi() );
      //uncopx->Fill( ccc->GetEy() );
      float ex0 = ccc->GetEx();
      float ey0 = ccc->GetEy();
      float ez0 = ccc->GetEz();
      ccc = pi0->GetCoalition(1);
      //uncotheta->Fill( ccc->GetTheta() );
      //uncophi->Fill( ccc->GetPhi() );
      //uncopx->Fill( ccc->GetEy() );
      float ex1 = ccc->GetEx();
      float ey1 = ccc->GetEy();
      float ez1 = ccc->GetEz();
      //if(asymm>0.2) continue;
      //if(energ>18) continue;
      //if(energ<14) continue;
      ccc = pi0->GetCoalition(0);
      mxParty *ppp = ccc->GetParty(8); // get hit from MPClayer
      mxHit *hhh = ppp->GetHit(0); // get most energetic tower
      int crys = hhh->Idx() - 49152;
      //std::cout << " MPC IDX " << hhh->Idx() << " party " << ppp->GetX() << " " << ppp->GetY() << std::endl;
      hmass[crys]->Fill( pi0->GetMass() );
      //hap[crys]->Fill( pi0->GetMass(), pi0->GetEnergyT() );
      hap[crys]->Fill( pi0->GetD8(), pi0->GetEnergy() );
      /*
      float alpha = (ez0-ez1)/(ez0+ez1);
      float et0 = TMath::Sqrt( ex0*ex0 + ey0*ey0 );
      float exm = pi0->GetEx();
      float eym = pi0->GetEy();
      float ezm = pi0->GetEz();
      float epi0 = pi0->GetEnergy();
      float qtx = ey0*ezm - ez0*eym;
      float qty = ez0*exm - ex0*ezm;
      float qtz = ex0*eym - ey0*exm;
      float qt = (qtx*qtx + qty*qty + qtz*qtz) / epi0;
      hap[crys]->Fill(alpha,qt);
      //std::cout << qt << std::endl;
      */
    }

    //continue;

    reco->MixUnions();
    int nfakepi0s = reco->GetNUnions(0);
    for(int i=0; i!=nfakepi0s; ++i) {
      mxUnion *pi0 = reco->GetUnion(0,i);
      //if(pi0->GetEnergy()<18) continue;
      //if(pi0->GetEnergy()>22) continue;
      float asymm = pi0->GetEnergyAsymmetry();
      float energ = pi0->GetEnergy();
      float d8 = pi0->GetD8();
      if(TMath::Abs(asymm)>CUT_ASYM) continue;
      dvsmmix->Fill( pi0->GetMass(), pi0->GetD8() );
      avsmmix->Fill( pi0->GetMass(), pi0->GetEnergyAsymmetry() );
      evsmmix->Fill( pi0->GetMass(), pi0->GetEnergy() );
      evsdmix->Fill( pi0->GetEnergy(), pi0->GetD8() );
      if(energ<CUT_EMIN) continue;
      if(energ>CUT_EMAX) continue;
      if(d8<CUT_DMIN) continue; // remove small angles
      //if(d8>15.0) continue; // remove small angles

      mxCoalition *ccc = pi0->GetCoalition(0);
      mxParty *ppp = ccc->GetParty(8); // get hit from MPClayer
      mxHit *hhh = ppp->GetHit(0); // get most energetic tower
      int crys = hhh->Idx() - 49152;
      //std::cout << " MPC IDX " << hhh->Idx() << " party " << ppp->GetX() << " " << ppp->GetY() << std::endl;
      //std::cout << pi0->GetMass() << std::endl;
      hmassmix[crys]->Fill( pi0->GetMass() );
    }

    reco->FillPreEvent();
    //if(nevs>100) break;
  }
  TFile *ofile = new TFile( Form("%sOUT_%s",dirs.Data(),file.Data()), "RECREATE" );
  //TFile *ofile = new TFile( "OUT.root", "RECREATE" );
  vtx->Write();
  mss->Write();
  ene->Write();
  //phl->Write();
  dvsm->Write();
  avsm->Write();
  evsm->Write();
  evsd->Write();
  dvsmmix->Write();
  avsmmix->Write();
  evsmmix->Write();
  evsdmix->Write();
  etvsm->Write();
  //uncotheta->Write();
  //uncophi->Write();
  //uncopx->Write();
  hmult->Write();
  hmult2->Write();
  for(int i=0; i!=416; ++i) {
    hmass[i]->Write();
    hmassmix[i]->Write();
    hap[i]->Write();
  }
  ofile->Close();
  //c = cuts0->GetList();
  //for(int i=0; i!=c->GetEntries(); ++i) (c->At(i))->Write();
  std::cout << "DONE" << std::endl;
  return 0;
}
