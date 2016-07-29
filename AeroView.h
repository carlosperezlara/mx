#ifndef __AeroView_HH__
#define __AeroView_HH__

class AeroView {
 public:
  AeroView();
  virtual ~AeroView();
  void InitGeo();
  void Fill(const int key, const double adc);
  void SetLogZ() {fLogz=true;}
  void SetZRange(double val,double val2) {fMinz=val;fMaxz=val2;fRangez=true;}
  TH2D* GetLayer(int arm, int lay) { return fLayer[arm][lay]; }
  void PrintKeys(int arm, int lay);
  float GetX(int key);
  float GetY(int key);
  
  TCanvas *Draw();
 protected:
  TH2D *fLayer[2][8];
  int fA[49152];
  int fL[49152];
  double fX[49152];
  double fY[49152];
  bool fLogz;
  bool fRangez;
  double fMinz;
  double fMaxz;
};
#endif /* __AeroView_H__ */
