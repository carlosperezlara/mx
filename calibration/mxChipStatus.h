#ifndef __mxChipStatus_HH__
#define __mxChipStatus_HH__

class mxChipStatus {
 public:
  mxChipStatus();
  virtual ~mxChipStatus() {}
  bool IsBad(int val) {return CellID(val)<1;}
  bool IsSouthLock() {return fSLock;}
  bool IsNorthLock() {return fNLock;}
  bool IsArmLock(int idx) {return idx<384?IsSouthLock():IsNorthLock();}
  int CellID(int);
  void Set(int,int);
  void Print();
  void Read(int mySVXMap[2][8][48]);
  void SetDebug(bool val) {fDebug=val;}

 protected:
  bool fDebug;
  bool IsTag(int val) {return fData[val]==1;}
  int fData[768];
  int fCID[768];
  bool fSLock;
  bool fNLock;
};
#endif /* __mxChipStatus_H__ */
