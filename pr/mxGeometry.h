#ifndef __mxGeometry_HH__
#define __mxGeometry_HH__
class mxGeometry {
 public:
  mxGeometry();
  ~mxGeometry() {}
  float X(int idx) { return Reference(idx,0); }
  float Y(int idx) { return Reference(idx,1); }
  float Z(int idx) { return Reference(idx,2); }
  int LyrIdx(int idx) { return Reference(idx,3); }
  float Si_a0() {return fSi_a0;}
  float Si_a1() {return fSi_a1;}
  float Si_a2() {return fSi_a2;}
  float W_a0() {return fW_a0;}
  float W_a1() {return fW_a1;}
  float W_a2() {return fW_a2;}
  float W_X(int sidx);
  float W_Y(int sidx);
  float W_Z(int sidx);

 private:
  float Reference(int,int);
  void UpdateVars(int);
  void UpdateSiW(int);
  void UpdatePbSc(int);

  int fLastIdx;
  int fLyrIdx;
  float fX;
  float fY;
  float fZ;
  // ==== SiW
  float fSiW_Sen;
  float fSi_a0;
  float fSi_a1;
  float fSi_a2;
  float fSi_RX[96];
  float fSi_RY[96];
  float fSi_RZ[16];
  int   fSi_LyrNbr[16];
  int   fSi_DLY[4];
  float fW_a0;
  float fW_a1;
  float fW_a2;
  float fW_RX[24];
  float fW_RY[24];
  
  // ==== PbSc

};
#endif /* __mxGeometry_H__ */
