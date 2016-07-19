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
  void QA(int level) {fQA=level;}
  void Debug() {fDebug=true;}

 protected:
  bool PassEventCuts(PHCompositeNode*);
  bool fDebug;
  int fQA;
  mxCalibMaster *fCal;
  mxReconstruction *fRec;

  TH1F *fEvents;
  TH2F *fPtySgn;
  TH2F *fPtySgn1;

  // EventQA ===> ChipHealth // lvl1
  TH1F *fQAbadchp;
  TH2F *fQAbadchpperchn;
  TH2F *fQAgoodchpcid;

  // RawSub ===> HotDead identification  Landau calibration // lvl2
  TH2F *fQAadchi;
  TH2F *fQAadclo;
  TH2F *fQAadchipty10;
  TH2F *fQAadchipty20;
  // RawSub ===> HighLow calib // lvl3
  THnSparse *fQAadchilo;
  TH2F *fQAadchilor;

  TH2F *fQAadchiNO;
  TH2F *fQAadcloNO;

};

#endif /* __mxSubsysReco_H__ */ 
