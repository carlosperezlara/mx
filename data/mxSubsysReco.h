#ifndef __mxSubsysReco_HH__
#define __mxSubsysReco_HH__

#include "SubsysReco.h"

class PHCompositeNode;
class mxCalibMaster;
class mxReconstruction;
class TH1F;
class TH2F;
class THnSparse;

class mxSubsysReco: public SubsysReco {
 public:
  mxSubsysReco( const char* name = "mxSubsysReco" );
  virtual ~mxSubsysReco();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
  virtual int Reset(PHCompositeNode*) {fPub->Reset(); return 0;}
  virtual int ResetEvent(PHCompositeNode* a) {return Reset(a);}
  void QA(int level) {fQA=val;}
  void Debug() {fDebug=true;}

 protected:
  bool fDebug;
  int fQA;
  mxCalibMaster *fCal;
  mxReconstruction *fRec;

  // ChipCellID ===> ChipHealth // lvl1
  TH1F *fQAbadchp;
  TH2F *fQAbadchpperchn;
  TH2F *fQAgoodchpcid;

  // RawSub ===> HotDead identification // lvl2
  TH2F *fQAadchi;
  TH2F *fQAadclo;

  // RawSub ===> HighLow calib // lvl3
  THnSparse *fQAadchilo;
  TH2F *fQAadchilor;

  // RawSub ===> Landau calibration // lvl4
  TH2F *fQAadchipty;
};

#endif /* __mxSubsysReco_H__ */ 
