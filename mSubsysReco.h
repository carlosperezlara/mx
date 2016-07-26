#ifndef __mSubsysReco_HH__
#define __mSubsysReco_HH__

#include "SubsysReco.h"

class TList;
class TH1F;
class TH2F;
class TH3F;

class PHCompositeNode;
class mxCalibMaster;
class mxReconstruction;
class mxQAReconstruction;

class mSubsysReco: public SubsysReco {
 public:
  mSubsysReco( const char* name = "mSubsysReco" );
  virtual ~mSubsysReco();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);

 protected:
  void FillHistos(int,PHCompositeNode*);
  bool PassEventCuts(PHCompositeNode*);
  bool fDoQA;
  mxCalibMaster *fCal;
  mxReconstruction *fRec;
  mxQAReconstruction *fQA;

  TList *fList;
  TH1F *fHstk[2];
  TH1F *fHpar[2];
  TH1F *fHsph[2];
  TH2F *fHcid[2];
  TH3F *fHcry;
  TH1F *fHcrytofS;
  TH1F *fHcryeneS;
  TH1F *fHcrytofN;
  TH1F *fHcryeneN;
};

#endif /* __mSubsysReco_H__ */ 
