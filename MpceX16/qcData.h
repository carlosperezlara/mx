//=========================
// written by Carlos Perez 
//=========================
#ifndef __qcData_HH__
#define __qcData_HH__

#include "qcQ.h"

class qcData {
 public:
  qcData();
  ~qcData();
  void Reset();
  qcQ* GetQex(int ord,int ioc,int det);
  qcQ* GetQmp() {return fQmp;}
  qcQ* GetQbb() {return fQbb;}
  qcQ* GetQfv() {return fQfv;}

 private:
  qcQ *fQex[10][2][4]; // [ord][ioc][det]
  qcQ *fQmp;
  qcQ *fQbb;
  qcQ *fQfv;

};

#endif /* __qcData_H__ */ 
