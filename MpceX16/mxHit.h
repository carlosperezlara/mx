//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxHit_HH__
#define __mxHit_HH__

class mxHit {
 public:
  mxHit();
  virtual ~mxHit();
  int Idx() {return fIdx;}
  float Signal() {return fSignal;}
  void Reset();
  void Fill(int, float);
  bool IsAssigned() {return fAssigned;}
  void SetAssigned(bool val) {fAssigned=val;}

 protected:
  int fIdx;
  float fSignal;
  bool fAssigned;
};
#endif /* __mxHit_H__ */
