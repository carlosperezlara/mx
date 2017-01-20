#ifndef __mSubsysReco_HH__
#define __mSubsysReco_HH__

#include <fstream>
#include <map>
#include "SubsysReco.h"

class TList;
class TH1F;
class TH2F;
class TH3F;
class TStopwatch;

class PHCompositeNode;
class mxCalibMaster;
class mxReconstruction;
class mxQAReconstruction;
class mxCoalitionCuts;

class mSubsysReco: public SubsysReco {
 public:
  mSubsysReco( const char* name = "mSubsysReco" );
  virtual ~mSubsysReco();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
  void SetEnableQA() {fDoQA=true;}
  void SetFloshToHitFile() {fFlush=true;}
  void ByPassCalibration() {fByPassEXCalibration=true;}
  void SetAlgorithmCombo(int v) {fAlgorithmCombo=v;}
  void SetSkipSouth() {fSkipSouth = true;}
  void SetSkipNorth() {fSkipNorth = true;}

 protected:
  void FillHistos(int,PHCompositeNode*);
  bool PassEventCuts(PHCompositeNode*);
  bool fDoQA;
  bool fSkipSouth;
  bool fSkipNorth;
  mxCalibMaster *fCal;
  mxReconstruction *fRec;
  mxQAReconstruction *fQA;
  ofstream fFileOut;
  bool fFlush;
  TStopwatch *fStopwatch;

  TList *fList;
  bool fCheckMpcRaw2;
  bool fCheckMpcExRawHit;
  std::map<int,int> fMPCIDX;

  float fNSigmaCut;
  bool fByPassEXCalibration;
  int fAlgorithmCombo;
  mxCoalitionCuts *fNoCuts;
  mxCoalitionCuts *fCalibrationCuts;

  TH1F *fTime;
  TH1F *fHstk[2];
  TH1F *fHpar[2];
  TH1F *fHsph[2];
  TH2F *fHcid[2];
  TH2F *fHadc[2];
  TH2F *fHlhf[2];

  TH3F *fHcry[2];
};

#endif /* __mSubsysReco_H__ */ 
