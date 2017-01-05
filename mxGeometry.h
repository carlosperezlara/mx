#ifndef __mxGeometry_HH__
#define __mxGeometry_HH__
class mxGeometry {
 public:
  mxGeometry();
  ~mxGeometry() {}
  int Si_Idx2Sen(int idx) { return (idx%3072)/128; }
  int Si_Idx2Row(int idx) { return idx%4; }
  int Si_Idx2Col(int idx) { return (idx%128)/4; }
  float X(int idx) { return Reference(idx,0); }
  float Y(int idx) { return Reference(idx,1); }
  float Z(int idx) { return Reference(idx,2); }
  int RefKey(int lyr, int tb);
  int RefKey(int lyr, int tb, int sen);
  int Adjacent_Si_0(int idx);
  int Adjacent_Si_1(int idx);
  int LyrIdx(int idx) { return Reference(idx,3); }
  float Si_a0() {return fSi_a0;}
  float Si_a1() {return fSi_a1;}
  float Si_a2() {return fSi_a2;}
  float W_a0() {return fW_a0;}
  float W_a1() {return fW_a1;}
  float W_a2() {return fW_a2;}
  float PWO4_a0() {return fPWO4_a0;}
  float PWO4_a1() {return fPWO4_a1;}
  float PWO4_a2() {return fPWO4_a2;}
  float W_X(int sidx);
  float W_Y(int sidx);
  float W_Z(int sidx);
  int PWO4_FindClosestIdx(int arm, float x, float y);
  float RZ(int lyr);

 private:
  float Reference(int,int);
  void UpdateVars(int);
  void UpdateSiW(int);
  void UpdatePWO4(int);
  int PWO4_FindIdx(int,float,float);

  int fLastIdx;
  int fLyrIdx;
  float fX;
  float fY;
  float fZ;
  // ==== SiW
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
  
  // ==== PWO4
  float fPWO4_a0;
  float fPWO4_a1;
  float fPWO4_a2;
  float fPWO4_RX[576];
  float fPWO4_RY[576];
  float fPWO4_RZ[576];
};
#endif /* __mxGeometry_H__ */
