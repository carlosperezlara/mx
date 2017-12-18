#ifndef __MCARECOPIZEROES_H__
#define __MCARECOPIZEROES_H__

#include "SubsysReco.h"
#include <TString.h>

class TH1F;
class TH2F;
class TProfile;
class TF1;

class Fun4AllServer;
class CentralityReco;
class recoConsts;

class PHCompositeNode;
class PHGlobal;
class TrigLvl1;
class PHCentralTrack;
class emcClusterContainer;
class emcClusterContent;
class ReactionPlaneObject;
class ReactionPlaneSngl;
class RunHeader;

class CNTDE;
class CNTE;
class EMCC;

class mCArecoPizeroes: public SubsysReco
{
 public:
  mCArecoPizeroes(const char *outfile = "TEST.root");
  virtual ~mCArecoPizeroes();
  
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);
  
 protected:   
  bool IsMaskedOut(int sector, int ypos, int zpos);
  void Read_EMC_Files(TString a, TString b, int r);
  void CentralArmTracks(PHCompositeNode *topNode);
  void CentralArmClusters(PHCompositeNode *topNode);
  float NLC_EMC_PbSc(float);
  float NLC_EMC_PbGl(float);

  unsigned int fTriggerBits;
  int fEMCDead[8][48][96];
  float fEMCSector[8];
  float fVZ;
  float fEXPsi[10][2][4];
  float fBBPsi;

  TH1F *fEvents;

  TH1F *fEMC_NClu[2];
  TH1F *fEMC_Chi2[2];
  TH1F *fEMC_ECore[2];
  TH2F *fEMC_ECoreCorr[2];
  TH2F *fEMC_XY[8];
  TH2F *fEMC_YZ[8];
  TH2F *fEMC_YZpos[8];
  TH1F *fPi0_Pt[2];
  TH2F *fPi0_Mass[2];
  TH1F *fPi0_Alpha[2];

  TH1F *fTRK_NTrks[2];
  TH1F *fTRK_ZED[2];
  TH2F *fTRK_EMCDZ[2];
  TH2F *fTRK_EMCDPHI[2];
  TH2F *fTRK_PC3DZ[2];
  TH2F *fTRK_PC3DPHI[2];
  TH1F *fTRK_EMCPC3_DZ[2];
  TH1F *fTRK_EMCPC3_DPHI[2];
  TH1F *fTRK_N0[2];
  TH1F *fTRK_DISP[2];
  TH1F *fTRK_CHI2[2];
  TH1F *fTRK_NPE0[2];
  TH1F *fTRK_PROB[2];
  TH1F *fTRK_CHARGE[2];
  TH1F *fTRK_ALPHA[2];
  TH1F *fTRK_PT[2];
  TH1F *fTRK_PHIPC[2];
  TH2F *fTRK_PC_XY[2];
  TH1F *fTRK_QUALITY[2];
  TProfile *fTRK_COSEX[2];
  TProfile *fTRK_COSBB[2];
};

#endif /* __MCARECOPIZEROES_H__ */
