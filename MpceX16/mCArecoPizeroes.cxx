#include <fstream>

#include "PHCompositeNode.h"
#include "PHIODataNode.h"
#include "mCArecoPizeroes.h"

#include "phool.h"
#include "TOAD.h"

#include "TLorentzVector.h"
#include "TMath.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include "getClass.h"
#include "PHGlobal.h"
#include "recoConsts.h"
#include "Fun4AllServer.h"
#include "Fun4AllReturnCodes.h"

#include "utiCentrality.h"
#include "TrigLvl1.h"
#include "TriggerHelper.h"
#include "RunHeader.h"

//Track Includes
#include "PHSnglCentralTrack.h"
#include "PHCentralTrack.h"
#include "PHCentralTrackv24.h"
#include "PHSnglCentralTrackv24.h"
//#include "PHGlobal_CENTRAL.h"

//EMCal includes
#include "emcClusterContainer.h"
#include "emcClusterContent.h"
#include "emcTowerContainer.h"
#include "EmcIndexer.h"

#include "PHQCData.h"
#include "qcData.h"
#include "qcQ.h"
#include "qcCalibMaster.h"
#include "qcCalibBase.h"
#include "qcDB.cc"

using namespace findNode;

mCArecoPizeroes::mCArecoPizeroes(const char *outfile) :
  SubsysReco("mCArecoPizeroes"),
  fTriggerBits(0),
  fVZ(0.0),
  fBBPsi(0.0),
  fQCCalib(new qcCalibMaster()),
  fEvents(NULL),
  fPsiEX(NULL) {
  std::cout << "mCAReco::Ctor" << std::endl;
  for(int i=0; i!=2; ++i) {
    fEMC_NClu[i] = NULL;
    fEMC_Chi2[i] = NULL;
    fEMC_ECore[i] = NULL;
    fEMC_ECoreCorr[i] = NULL;
    fPi0_Pt[i] = NULL;
    fPi0_Mass[i] = NULL;
    fPi0_Alpha[i] = NULL;
    fPi0_COSEX[i] = NULL;
    fPi0_COSBB[i] = NULL;
    
    fTRK_NTrks[i] = NULL;
    fTRK_ZED[i] = NULL;
    fTRK_EMCDZ[i] = NULL;
    fTRK_EMCDPHI[i] = NULL;
    fTRK_PC3DZ[i] = NULL;
    fTRK_PC3DPHI[i] = NULL;
    fTRK_EMCPC3_DZ[i] = NULL;
    fTRK_EMCPC3_DPHI[i] = NULL;
    fTRK_N0[i] = NULL;
    fTRK_DISP[i] = NULL;
    fTRK_CHI2[i] = NULL;
    fTRK_NPE0[i] = NULL;
    fTRK_PROB[i] = NULL;
    fTRK_CHARGE[i] = NULL;
    fTRK_ALPHA[i] = NULL;
    fTRK_PT[i] = NULL;
    fTRK_PHI[i] = NULL;
    fTRK_DPHI[i] = NULL;
    fTRK_PHIPC[i] = NULL;
    fTRK_PC_XY[i] = NULL;
    fTRK_QUALITY[i] = NULL;
    fTRK_COSEX[i] = NULL;
    fTRK_COSBB[i] = NULL;
  }
  for(int i=0; i!=8; ++i) {
    fEMC_XY[i] = NULL;
    fEMC_YZ[i] = NULL;
    fEMC_YZpos[i] = NULL;
    fEMCSector[i] = 1.0;
    for(int j=0; j!=48; ++j)
      for(int k=0; k!=96; ++k)
	fEMCDead[i][j][k] = 0;
  }
  for(int i=0; i!=10; ++i)
    for(int j=0; j!=2; ++j)
      for(int k=0; k!=4; ++k)
	fEXPsi[i][j][k]=0.0;
  return;
}

mCArecoPizeroes::~mCArecoPizeroes() {
  delete fQCCalib;
}

int mCArecoPizeroes::Init(PHCompositeNode *topNode) {
  Fun4AllServer *se = Fun4AllServer::instance();
  fEvents = new TH1F("caReco_Events","caReco_Events",5,-0.5,4.5);
  se->registerHisto( ((TH1*) (fEvents) ) );
  fEvents->GetXaxis()->SetBinLabel(1,"All Raw");
  fEvents->GetXaxis()->SetBinLabel(2,"Data Reached");
  fEvents->GetXaxis()->SetBinLabel(3,"Centrality Cut");
  fEvents->GetXaxis()->SetBinLabel(4,"Futher Event Cuts");

  fPsiEX = new TH2F("caReco_PsiEx","caReco_PsiEx",100,0,TMath::TwoPi(),100,0,TMath::TwoPi());
  se->registerHisto( ((TH2F*) (fPsiEX) ) );

  for(int i=0; i!=2; ++i) {
    fEMC_NClu[i] = new TH1F( Form("EMC_NClu_%d",i), Form("EMC_NClu_%d",i), 100, 0, 400 );
    fEMC_Chi2[i] = new TH1F( Form("EMC_Chi2_%d",i), Form("EMC_Chi2_%d",i), 100, 0, 5 );
    fEMC_ECore[i] = new TH1F( Form("EMC_ECore_%d",i), Form("EMC_ECore_%d",i), 100, 0, 15 );
    fEMC_ECoreCorr[i] = new TH2F( Form("EMC_ECoreCorr_%d",i), Form("EMC_ECoreCorr_%d",i),
				  8, -0.5, 7.5, 100, -2, +2 );
    fPi0_Pt[i] = new TH1F( Form("Pi0_Pt_%d",i), Form("Pi0_Pt_%d",i), 100, 0, 10 );
    fPi0_Mass[i] = new TH2F( Form("Pi0_Mass_%d",i), Form("Pi0_Mass_%d",i),
			     100,0,10, 1000, 0, 1.0 );
    fPi0_Alpha[i] = new TH1F( Form("Pi0_Alpha_%d",i), Form("Pi0_Alpha_%d",i), 100, 0, 1.0 );
    fPi0_COSEX[i] = new TProfile2D( Form("Pi0_COSEX_%d",i), Form("Pi0_COSEX_%d",i), 100, 0, 10, 1000, 0, 1.0);
    fPi0_COSBB[i] = new TProfile2D( Form("Pi0_COSBB_%d",i), Form("Pi0_COSBB_%d",i), 100, 0, 10, 1000, 0, 1.0);
    se->registerHisto( ((TH1F*) (fEMC_NClu[i]) ) );
    se->registerHisto( ((TH1F*) (fEMC_Chi2[i]) ) );
    se->registerHisto( ((TH1F*) (fEMC_ECore[i]) ) );
    se->registerHisto( ((TH2F*) (fEMC_ECoreCorr[i]) ) );
    se->registerHisto( ((TH1F*) (fPi0_Pt[i]) ) );
    se->registerHisto( ((TH2F*) (fPi0_Mass[i]) ) );
    se->registerHisto( ((TH1F*) (fPi0_Alpha[i]) ) );
    se->registerHisto( ((TProfile2D*) (fPi0_COSEX[i]) ) );
    se->registerHisto( ((TProfile2D*) (fPi0_COSBB[i]) ) );

    fTRK_NTrks[i] = new TH1F( Form("TRK_NTrks_%d",i), Form("TRK_NTrks_%d",i), 100, 0, 100);
    fTRK_ZED[i] = new TH1F( Form("TRK_ZED_%d",i), Form("TRK_ZED_%d",i), 100, -100, +100);
    fTRK_EMCDZ[i] = new TH2F( Form("TRK_EMCDZ_%d",i), Form("TRK_EMCDZ_%d",i), 100, -50., +50., 100, -50., +50.);
    fTRK_EMCDPHI[i] = new TH2F( Form("TRK_EMCDPHI_%d",i), Form("TRK_EMCDPHI_%d",i), 100, -0.12, +0.12, 100, -0.12, +0.12);
    fTRK_PC3DZ[i] = new TH2F( Form("TRK_PC3DZ_%d",i), Form("TRK_PC3DZ_%d",i), 100, -50., +50., 100, -10., +10.);
    fTRK_PC3DPHI[i] = new TH2F( Form("TRK_PC3DPHI_%d",i), Form("TRK_PC3DPHI_%d",i), 100, -0.12, +0.12, 100, -10, +10);
    fTRK_EMCPC3_DZ[i] = new TH1F( Form("TRK_EMCPC3_DZ_%d",i), Form("TRK_EMCPC3_DZ_%d",i), 100, -0.2, +0.2);
    fTRK_EMCPC3_DPHI[i] = new TH1F( Form("TRK_EMCPC3_DPHI_%d",i), Form("TRK_EMCPC3_DPHI_%d",i), 100, -0.2, +0.2);
    fTRK_N0[i] = new TH1F( Form("TRK_N0_%d",i), Form("TRK_N0_%d",i), 100, -0.5, 99.5);
    fTRK_DISP[i] = new TH1F( Form("TRK_DISP_%d",i), Form("TRK_DISP_%d",i), 100, 0., +10.);
    fTRK_CHI2[i] = new TH1F( Form("TRK_CHI2_%d",i), Form("TRK_CHI2_%d",i), 100, 0., +200.);
    fTRK_NPE0[i] = new TH1F( Form("TRK_NPE0_%d",i), Form("TRK_NPE0_%d",i), 100, 0., 40.);
    fTRK_PROB[i] = new TH1F( Form("TRK_PROB_%d",i), Form("TRK_PROB_%d",i), 100, 0., 1.0);
    fTRK_CHARGE[i] = new TH1F( Form("TRK_CHARGE_%d",i), Form("TRK_CHARGE_%d",i), 100, -3., +3.);
    fTRK_ALPHA[i] = new TH1F( Form("TRK_ALPHA_%d",i), Form("TRK_ALPHA_%d",i), 100, -2., +2.);
    fTRK_PT[i] = new TH1F( Form("TRK_PT_%d",i), Form("TRK_PT_%d",i), 100, -10., +10.);
    fTRK_PHI[i] = new TH1F( Form("TRK_PHI_%d",i), Form("TRK_PHI_%d",i), 100, 0., TMath::TwoPi());
    fTRK_DPHI[i] = new TH1F( Form("TRK_DPHI_%d",i), Form("TRK_DPHI_%d",i), 100, -TMath::TwoPi(), TMath::TwoPi());
    fTRK_PHIPC[i] = new TH1F( Form("TRK_PHIPC_%d",i), Form("TRK_PHIPC_%d",i), 100, 0., +7.);
    fTRK_PC_XY[i] = new TH2F( Form("TRK_PC_XY_%d",i), Form("TRK_PC_XY_%d",i), 100, -600., +600., 100, -600., +600.);
    fTRK_QUALITY[i] = new TH1F( Form("TRK_QUALITY_%d",i), Form("TRK_QUALITY_%d",i), 100, -0.5, 99.5);
    fTRK_COSEX[i] = new TProfile( Form("TRK_COSEX_%d",i), Form("TRK_COSEX_%d",i), 100, 0, 10);
    fTRK_COSBB[i] = new TProfile( Form("TRK_COSBB_%d",i), Form("TRK_COSBB_%d",i), 100, 0, 10);
    se->registerHisto( ((TH1F*) (fTRK_NTrks[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_ZED[i]) ) );
    se->registerHisto( ((TH2F*) (fTRK_EMCDZ[i]) ) );
    se->registerHisto( ((TH2F*) (fTRK_EMCDPHI[i]) ) );
    se->registerHisto( ((TH2F*) (fTRK_PC3DZ[i]) ) );
    se->registerHisto( ((TH2F*) (fTRK_PC3DPHI[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_EMCPC3_DZ[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_EMCPC3_DPHI[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_N0[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_DISP[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_CHI2[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_NPE0[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_PROB[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_CHARGE[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_ALPHA[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_PT[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_PHI[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_DPHI[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_PHIPC[i]) ) );
    se->registerHisto( ((TH2F*) (fTRK_PC_XY[i]) ) );
    se->registerHisto( ((TH1F*) (fTRK_QUALITY[i]) ) );
    se->registerHisto( ((TProfile*) (fTRK_COSEX[i]) ) );
    se->registerHisto( ((TProfile*) (fTRK_COSBB[i]) ) );
  }

  for(int i=0; i!=8; ++i) {
    fEMC_XY[i] = new TH2F( Form("EMC_XY_Sector%d",i), Form("EMC_XY_Sector%d",i),
			   100, -600., +600., 100, -600., +600. );
    fEMC_YZ[i] = new TH2F( Form("EMC_YZ_Sector%d",i), Form("EMC_YZ_Sector%d",i),
			   100, -600., +600., 100, -600., +600. );
    fEMC_YZpos[i] = new TH2F( Form("EMC_YZpos_Sector%d",i), Form("EMC_YZpos_Sector%d",i),
			      100, -0.5, 99.5, 100, -0.5, +99.5 );
    se->registerHisto( ((TH2F*) (fEMC_XY[i]) ) );
    se->registerHisto( ((TH2F*) (fEMC_YZ[i]) ) );
    se->registerHisto( ((TH2F*) (fEMC_YZpos[i]) ) );
  }
  return 0;
}

int mCArecoPizeroes::InitRun(PHCompositeNode *topNode) {
  RunHeader *header = getClass<RunHeader>(topNode,"RunHeader");
  if(!header) std::cout << "can't find RunHeader" << std::endl;
  int runnumber=header->get_RunNumber();

  printf("mQCRinit::InitRun || Run number %d\n",runnumber);
  fQCCalib = new qcCalibMaster();
  qcDB::read(runnumber,fQCCalib);

  //==== Loading triggers
  unsigned int trigger_FVTXNSBBCScentral = 0x00100000;
  unsigned int trigger_FVTXNSBBCS        = 0x00400000;     
  unsigned int trigger_BBCLL1narrowcent  = 0x00000008;
  unsigned int trigger_BBCLL1narrow      = 0x00000010;
  // --- Run16dAu200
  if( runnumber >= 454774 && runnumber <= 455639) {
    fTriggerBits = trigger_BBCLL1narrowcent | trigger_BBCLL1narrow;
    Read_EMC_Files("Run16dAuEmcalDeadMap.txt","run_vs_ratiopersector.dat",runnumber);
  }
  // --- Run16dAu62   
  if( runnumber >= 455792 && runnumber <= 456283)
    fTriggerBits = trigger_BBCLL1narrowcent | trigger_BBCLL1narrow;
  // --- Run16dAu20     
  if( runnumber >= 456652 && runnumber <= 457298)
    fTriggerBits = trigger_FVTXNSBBCScentral | trigger_FVTXNSBBCS;
  // --- Run16dAu39     
  if( runnumber >= 457634 && runnumber <= 458167)
    fTriggerBits = trigger_FVTXNSBBCScentral | trigger_FVTXNSBBCS;
  return 0;
}

int mCArecoPizeroes::process_event(PHCompositeNode *topNode) {
  fEvents->Fill(0);
  PHGlobal *phg = getClass<PHGlobal>(topNode,"PHGlobal");
  if(!phg) return DISCARDEVENT;
  fVZ = phg->getBbcZVertex();
  fEvents->Fill(1);
  fEvents->Fill(2);
  //------------------------
  TrigLvl1 *_Trig_ptr = getClass<TrigLvl1>(topNode, "TrigLvl1");
  if(!_Trig_ptr) return DISCARDEVENT;
  unsigned int trigger_scaled = _Trig_ptr->get_lvl1_trigscaled();
  unsigned int passes_trigger = trigger_scaled & fTriggerBits;
  if(passes_trigger==0) return DISCARDEVENT;
  //------------------------
  PHQCData *phqcdata = getClass<PHQCData> (topNode,"QCR");
  if(!phqcdata) return ABORTEVENT;
  qcData *qcdata = phqcdata->GetData();
  //std::cout << qcdata->GetQex(1,0,0)->M() << std::endl;
  qcQ psi2(2);
  int icc = int(phg->getCentrality()/5.0);
  psi2.SetXY(qcdata->GetQex(1,0,0)->X() - fQCCalib->GetEXSecond(0,0)->GetRe(icc),
	    qcdata->GetQex(1,0,0)->Y() - fQCCalib->GetEXSecond(0,0)->GetIm(icc)
	    );
  fEXPsi[1][0][0] = psi2.Psi();
  fBBPsi = qcdata->GetQbb()->Psi();
  fPsiEX->Fill(fEXPsi[1][0][0],qcdata->GetQex(1,0,0)->Psi());
  fEvents->Fill(3);
  fEvents->Fill(4);

  CentralArmTracks(topNode);
  CentralArmClusters(topNode);

  return EVENT_OK;        
}
//========================
void mCArecoPizeroes::CentralArmTracks(PHCompositeNode *topNode) {
  //  0 (1)   X1 used
  //  1 (2)   X2 used
  //  2 (4)   UV found
  //  3 (8)   UV unique
  //  4 (16)  PC1 found
  //  5 (48)  PC1 unique
  PHCentralTrack *trk = getClass<PHCentralTrack>(topNode,"PHCentralTrack");
  if(!trk) return;
  int ntrks = trk->get_npart();
  int ntrks2=0;
  for(int i=0; i!=ntrks; ++i) {
    // PC
    float zed = trk->get_zed(i); // z coord at which track crosses PC1
    double pc1_x=trk->get_ppc1x(i); // track model X-projection to PC1
    double pc1_y=trk->get_ppc1y(i); // track model Y-projection to PC1
    double pc2_x=trk->get_ppc2x(i); // track model X-projection to PC2
    double pc2_y=trk->get_ppc2y(i); // track model Y-projection to PC2
    double pc3_x=trk->get_ppc3x(i); // track model X-projection to PC3
    double pc3_y=trk->get_ppc3y(i); // track model Y-projection to PC3
    double pc3_dphi=trk->get_pc3dphi(i); // diff in rads btw trk model and hit in PC3
    double pc3_dz=trk->get_pc3dz(i); // diff in cm btw trk model and hit in PC3
    double pc3_sdphi=trk->get_pc3sdphi(i); // diff in rads btw trk model and hit in PC3 (sigmalized)
    double pc3_sdz=trk->get_pc3sdz(i); // diff in cm btw trk model and hit in PC3 (sgimalized)
    //double pc1_z=trk->get_ppc1z(i); // track model Z-projection to PC1
    double phi_pc= TMath::ACos(pc1_x/sqrt(pc1_x*pc1_x+pc1_y*pc1_y));
    // EMC
    float prob = trk->get_prob(i); // probability that particle shower is electromagnetic
    float emcdz = trk->get_emcdz(i); // diff in cm btw track model projection and hit in EMC
    float emcdphi = trk->get_emcdphi(i); // diff in rads btw track model projection and hit in EMC
    // RICH (not working based on entries diff than NAN)
    float n0 = trk->get_n0(i); // no of phototubes that fired in normally sized ring area RICH
    float npe0 = trk->get_npe0(i); // summed of photoelectrons over normally sized ring area RICH
    float disp = trk->get_disp(i); // displacement of ring center wrt Track projection in the RICH PMT array
    float chi2 = trk->get_chi2(i); // chi2 in units of cm^2. It is not per degree of freedom. Typically cuts are placed on chi2/npe0 as a seme-normalized chi2/dof.
    // TOF
    //float tofphi = trk->get_tofphi(i); // phi coord in rads of measured hit at TOF
    //float ptofx = trk->get_ptofx(i); // track model x-projection to tof (cm)
    //float ptofy = trk->get_ptofy(i); // track model y-projection to tof (cm)
    //float pltof = trk->get_pltof(i); // path length of particle trajectory from vertex to TOF

    // TRACKING
    float charge = trk->get_charge(i); // -1 +1
    float alpha = trk->get_alpha(i); // inclination of track wrt infinite mom trck. pt is roughly proportinal to 1/alpha.
    //int arm  = trk->get_dcarm(i);
    double px = trk->get_px(i);
    double py = trk->get_py(i);
    //double pz = trk->get_pz(i);
    double pT = sqrt(px*px + py*py);
    //double p = sqrt(px*px + py*py+pz*pz);
    //double theta=0.5*TMath::Pi()/TMath::ACos(pz/p);
    int qua = trk->get_quality(i);
    float phi = trk->get_phi(i);
    if(phi<0) phi += TMath::TwoPi();
    float dcose = TMath::Cos(2*(phi-fEXPsi[1][0][0]));
    float dcosb = TMath::Cos(2*(phi-fBBPsi));
    fTRK_ZED[0]->Fill(zed);
    fTRK_EMCDZ[0]->Fill(emcdz,pc3_dz);
    fTRK_EMCDPHI[0]->Fill(emcdphi,pc3_dphi);
    fTRK_PC3DZ[0]->Fill(pc3_dz,pc3_sdz);
    fTRK_PC3DPHI[0]->Fill(pc3_dphi,pc3_sdphi);
    fTRK_EMCPC3_DZ[0]->Fill(emcdz-pc3_dz);
    fTRK_EMCPC3_DPHI[0]->Fill(emcdphi-pc3_dphi);
    fTRK_N0[0]->Fill(n0);
    fTRK_DISP[0]->Fill(disp);
    fTRK_CHI2[0]->Fill(chi2);
    fTRK_NPE0[0]->Fill(npe0);
    fTRK_PROB[0]->Fill(prob);
    fTRK_CHARGE[0]->Fill(charge);
    fTRK_ALPHA[0]->Fill(alpha);
    fTRK_PT[0]->Fill(charge*pT);
    fTRK_PHI[0]->Fill(phi);
    fTRK_DPHI[0]->Fill(phi-fEXPsi[1][0][0]);
    fTRK_PHIPC[0]->Fill(phi_pc);
    fTRK_PC_XY[0]->Fill(pc1_x,pc1_y);
    //fTRK_PC_XY[0]->Fill(pc2_x,pc2_y);
    fTRK_PC_XY[0]->Fill(pc3_x,pc3_y);
    fTRK_QUALITY[0]->Fill(qua);
    fTRK_COSEX[0]->Fill(pT,dcose);
    fTRK_COSBB[0]->Fill(pT,dcosb);
    if(qua!=63) continue;
    ++ntrks2;
    fTRK_ZED[1]->Fill(zed);
    fTRK_EMCDZ[1]->Fill(emcdz,pc3_dz);
    fTRK_EMCDPHI[1]->Fill(emcdphi,pc3_dphi);
    fTRK_PC3DZ[1]->Fill(pc3_dz,pc3_sdz);
    fTRK_PC3DPHI[1]->Fill(pc3_dphi,pc3_sdphi);
    fTRK_EMCPC3_DZ[1]->Fill(emcdz-pc3_dz);
    fTRK_EMCPC3_DPHI[1]->Fill(emcdphi-pc3_dphi);
    fTRK_N0[1]->Fill(n0);
    fTRK_DISP[1]->Fill(disp);
    fTRK_CHI2[1]->Fill(chi2);
    fTRK_NPE0[1]->Fill(npe0);
    fTRK_PROB[1]->Fill(prob);
    fTRK_CHARGE[1]->Fill(charge);
    fTRK_ALPHA[1]->Fill(alpha);
    fTRK_PT[1]->Fill(charge*pT);
    fTRK_PHI[1]->Fill(phi);
    fTRK_DPHI[1]->Fill(phi-fEXPsi[1][0][0]);
    fTRK_PHIPC[1]->Fill(phi_pc);
    fTRK_PC_XY[1]->Fill(pc1_x,pc1_y);
    fTRK_PC_XY[1]->Fill(pc2_x,pc2_y);
    fTRK_PC_XY[1]->Fill(pc3_x,pc3_y);
    fTRK_QUALITY[1]->Fill(qua);
    fTRK_COSEX[1]->Fill(pT,dcose);
    fTRK_COSBB[1]->Fill(pT,dcosb);
  }
  fTRK_NTrks[0]->Fill( ntrks );
  fTRK_NTrks[1]->Fill( ntrks2 );
}
//========================
void mCArecoPizeroes::CentralArmClusters(PHCompositeNode *topNode) {
  emcClusterContainer *emccont = getClass<emcClusterContainer>(topNode, "emcClusterContainer");
  if(!emccont) return;
  int nclu = emccont->size();
  int nclu2 = 0;
  fEMC_NClu[0]->Fill(nclu);
  emcClusterContent *emc;
  std::vector<TLorentzVector> photons;
  for(int i=0; i!=nclu; ++i) {
    emc = emccont->getCluster(i);
    int arm = emc->arm(); // in EMC arm=0 is (+x) and arm=1 is (-x)
    int sect = emc->sector(); // in EMC sector goes from 0 to 3 bottom to top in y
    int mysector = arm*4+sect; // 0-3 PbSc || 4-5 PbGl || 6-7 PbSc
    bool isPbSc = true;
    if(mysector==4||mysector==5) isPbSc = false;
    float x = emc->x(); // in cm
    float y = emc->y(); // in cm
    float z = emc->z()-fVZ; // in cm
    int yp = emc->iypos(); // 0 35 PbSc || 0-47 PbGl
    int zp = emc->izpos(); // 0-71 PbSc || 0-95 PbGl
    float chi2 = emc->chi2();
    float ecore_ori = emc->ecore();
    float ecore;
    if(isPbSc)
      ecore = ecore_ori/NLC_EMC_PbSc(ecore_ori) * fEMCSector[mysector];
    else
      ecore = ecore_ori/NLC_EMC_PbGl(ecore_ori) * fEMCSector[mysector];
    float len = TMath::Sqrt( x*x + y*y + z*z );
    float px = ecore*x/len;
    float py = ecore*y/len;
    float pz = ecore*z/len;
    float deltaEcore = (ecore-ecore_ori)/ecore_ori;
    fEMC_XY[mysector]->Fill(x,y);
    fEMC_YZ[mysector]->Fill(y,z);
    fEMC_YZpos[mysector]->Fill(yp,zp);
    fEMC_ECoreCorr[0]->Fill( mysector, deltaEcore );
    fEMC_Chi2[0]->Fill( chi2 );
    fEMC_ECore[0]->Fill( ecore );
    if(IsMaskedOut(mysector,yp,zp)) continue;
    if(chi2>3.0) continue;
    if(ecore<0.2) continue;
    ++nclu2;
    fEMC_ECoreCorr[1]->Fill( mysector, deltaEcore );
    fEMC_Chi2[1]->Fill( chi2 );
    fEMC_ECore[1]->Fill( ecore );
    photons.push_back( TLorentzVector(px,py,pz,ecore) );
  }
  fEMC_NClu[1]->Fill(nclu2);

  for(unsigned int i=0; i!=photons.size(); ++i) {
    TLorentzVector a = photons.at( i );
    float eneA = a.E();
    for(unsigned int j=i+1; j!=photons.size(); ++j) {
      float eneB = photons.at( j ).E();
      TLorentzVector b = a + photons.at( j );
      float pt = b.Pt();
      float m = b.M();
      float alpha = TMath::Abs(eneA-eneB)/(eneA+eneB);
      float phi = b.Phi();
      float dcose = TMath::Cos(2*(phi-fEXPsi[1][0][0]));
      float dcosb = TMath::Cos(2*(phi-fBBPsi));
      fPi0_Pt[0]->Fill( pt );
      fPi0_Mass[0]->Fill( pt, m );
      fPi0_Alpha[0]->Fill( alpha );
      fPi0_COSEX[0]->Fill(pt,m,dcose);
      fPi0_COSBB[0]->Fill(pt,m,dcosb);
      if(pt<0.1) continue;
      if(pt>20.0) continue;
      if(alpha>0.8) continue;
      fPi0_Pt[1]->Fill( pt );
      fPi0_Mass[1]->Fill( pt, m );
      fPi0_Alpha[1]->Fill( alpha );
      fPi0_COSEX[1]->Fill(pt,m,dcose);
      fPi0_COSBB[1]->Fill(pt,m,dcosb);
    }
  }

}
//========================
bool mCArecoPizeroes::IsMaskedOut(int sector, int ypos, int zpos) {
  if( ypos==0 || zpos==0 ) return true;
  if(sector==4||sector==5) {
    if(ypos==35||zpos==71) return true;
  } else {
    if(ypos==47||zpos==95) return true;
  }
  int sectorVeronica = sector;
  if(sector>3) {
    sectorVeronica = 7 - sector%4;
  }
  if( fEMCDead[sectorVeronica][ypos][zpos] ) return true;
  return false;
}
//========================
int mCArecoPizeroes::End(PHCompositeNode *topNode){
  return 0;
}
//========================
void mCArecoPizeroes::Read_EMC_Files(TString deadmap, TString gainmap, int trun) {
  TOAD toad_loader("MXphnx");

  //======= DEADMAP =======
  TString dead = toad_loader.location( deadmap.Data() );
  std::ifstream fdead( dead.Data() );
  if(!fdead.is_open()) exit(0);
  int sect = 0, ypos = 0, zpos = 0, status = 0;
  while(fdead >> sect >> ypos >> zpos >> status) {
    fEMCDead[sect][ypos][zpos] = status;
  }
  fdead.close();

  //======= DEADMAP =======
  TString gain = toad_loader.location( gainmap.Data() );
  std::ifstream fgain( gain.Data() );
  if(!fgain.is_open()) exit(0);
  int run;
  float s[8];
  while(fgain >> run >>s[0]>>s[1]>>s[2]>>s[3]>>s[4]>>s[5]>>s[6]>>s[7]) {
    if(trun==run) {
      fEMCSector[0] = s[0];
      fEMCSector[1] = s[1];
      fEMCSector[2] = s[2];
      fEMCSector[3] = s[3];
      fEMCSector[4] = s[4];
      fEMCSector[5] = s[5];
      fEMCSector[6] = s[6];
      fEMCSector[7] = s[7];
    }
  }

  return;
}
//========================
float mCArecoPizeroes::NLC_EMC_PbSc(float ene) {
  if(ene<0.01) return 0;
  if(ene>100.) return 0;
  return 0.003+(1-0.010/ene);
}

//========================
float mCArecoPizeroes::NLC_EMC_PbGl(float ene) {
  if(ene<0.01) return 0;
  if(ene>100.) return 0;
  return 0.021+(1-0.020/ene);
}
