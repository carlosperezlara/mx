#ifndef __mQCRinit_HH__
#define __mQCRinit_HH__

#include <fstream>
#include "SubsysReco.h"
#include "TString.h"

class PHCompositeNode;

class qcQ;
class qcData;
class qcCalibBase;
class mxGeometry;
class BbcCalib;
class BbcGeo;

class TH1F;
class TH2F;
class TProfile2D;

class mQCRinit: public SubsysReco {
 public:
  mQCRinit( const char* name = "mcounter" );
  virtual ~mQCRinit();

  virtual int Init(PHCompositeNode*);
  virtual int InitRun(PHCompositeNode*);
  virtual int process_event(PHCompositeNode*);
  virtual int End(PHCompositeNode *topNode);
  void SetQA() {fQA=true;}

 protected:
  mxGeometry *fGeo;
  qcCalibBase *fCalib;

  BbcCalib *fBBCcalib;
  BbcGeo *fBBCgeo;

  TString fName;
  bool fQA;

  qcData *fData;

  qcQ *fQex[10][2][2][4]; // [ord][arm][ioc][det]
  qcQ *fQmpc[2]; // [arm]
  qcQ *fQbbc[2]; // [arm]

  TH1F *fEvents;
  TH2F *fCentrality;

  TH2F *fHMultBBC[2][2][4]; // [arm][ioc][det]
  TH2F *fHPsiex[10][2][2][4]; // [ord][arm][ioc][det]
  TH2F *fHPsiexN[10][2][2][4];
  TH2F *fHPsiexM[10][2][2][4];
  TH2F *fHQexNorm[10][2][2][4];
  TH2F *fHQex[10][2][2][4][2]; // [ord][arm][ioc][det][xy]
  int fEXPair_IOC[16][2];
  int fEXPair_DET[16][2];
  TH2F *fHPsi2Rex[2][16]; // [arm][pair]
  TH2F *fHPsi2Reb[2][2][4]; // [arm][ioc][det]
  TH2F *fHPsi2Cox[2][16]; // [arm][pair]
  TH2F *fHPsi2Com[2][2][4]; // [arm][ioc][det]
  TH2F *fHPsi2Cob[2][2][4]; // [arm][ioc][det]
  TProfile2D *fPPsiex[10][2][2][4]; // [ord][arm][ioc][det]

};

#endif /* __mQCRinit_H__ */ 
