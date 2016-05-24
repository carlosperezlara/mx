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
  float fSiW_a0;
  float fSiW_a1;
  float fSiW_RX[96];
  float fSiW_RY[96];
  float fSiW_RZ[16];
  int   fSiW_LyrNbr[16];
  int   fSiW_DLY[4];
  // ==== PbSc

};
#endif /* __mxGeometry_H__ */
