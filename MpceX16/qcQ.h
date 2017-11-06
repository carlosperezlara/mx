//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __qcQ_HH__
#define __qcQ_HH__

class qcQ {
 public:
  qcQ(int n=0);
  virtual ~qcQ();
  int Order() {return fN;}
  int NP() {return fNP;}
  float M() {return fM;}
  float X() {return fX;}
  float Y() {return fY;}
  float Psi();
  void Fill(float, float);
  float ModulusSquared() {return (fX*fX+fY*fY);}
  void Reset();
  void SetOrder(int val) {fN=val;}

 protected:
  int fN;
  int fNP;
  float fM;
  float fX;
  float fY;
};
#endif /* __qcQ_H__ */
