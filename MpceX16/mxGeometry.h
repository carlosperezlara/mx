#ifndef __mxGeometry_HH__
#define __mxGeometry_HH__

class mxGeometry {
 public:
  static mxGeometry* Instance() {
    if(!fGeo) fGeo = new mxGeometry();
    return fGeo;
  }

  virtual ~mxGeometry() {}
  //=================
  //Hardware Map: first 49152 PS; next 416 EM
  // idx 0 - 49567
  inline int PS_Idx2Arm(int idx) { return idx/24576; } // 0 - 1
  inline int PS_Idx2Pkt(int idx) { return (idx%24576)/3072; } // 0 - 8
  int PS_Idx2Lyr(int idx) { return Reference(idx,3); } // 0 - 17
  int PS_Idx2Sen(int idx) { return Reference(idx,4); } // 0 - 23 (SenLyr)
  int PS_Idx2SenPkt(int idx) { return Reference(idx,5); } // 0 - 23 (SenPkt)
  int PS_SenRing(int sen); // 0 - 1
  inline int PS_Idx2Row(int idx) { return idx%4; } // 0 - 3
  inline int PS_Idx2Col(int idx) { return (idx%128)/4; } // 0 - 31
  inline int EM_Idx2Arm(int idx) { return idx<196?0:1; } // 0 - 1
  inline int PS_Idx2OctLCS(int idx) { return ((idx%128)/32)*4+idx%4; } // 0 - 15
  int PS_Idx2Oct(int idx) { return PS_Idx2Sen(idx)*16+PS_Idx2OctLCS(idx);  } // 0 - 383
  //=================

  //================
  // Dimensions
  float Si_a0() {return fSi_a0;}
  float Si_a1() {return fSi_a1;}
  float Si_a2() {return fSi_a2;}
  float W_a0() {return fW_a0;}
  float W_a1() {return fW_a1;}
  float W_a2() {return fW_a2;}
  float PbWO4_a0() {return fPbWO4_a0;}
  float PbWO4_a1() {return fPbWO4_a1;}
  float PbWO4_a2() {return fPbWO4_a2;}
  //================


  //================
  // Helpers
  int RefKey(int lyr, int tb);
  int RefKey(int lyr, int tb, int sen);
  int Adjacent_Si_0(int idx);
  int Adjacent_Si_1(int idx);
  int LyrIdx(int idx) { return Reference(idx,3); }
  int PbWO4_FindClosestIdx(int arm, float x, float y);
  void PbWO4_GetNeighbours(int idx, int en[4]);
  void PS_GetSensorNeighbours(int sidx, int nei[5]);
  float RZ(int lyr);
  //================

  float X(int idx) { return Reference(idx,0); }
  float Y(int idx) { return Reference(idx,1); }
  float Z(int idx) { return Reference(idx,2); }
  float W_X(int sidx, int arm=0);
  float W_Y(int sidx, int arm=0);
  float W_Z(int lyr);
  float PS_SenX(int arm, int sidx) { return fW_RX[arm][sidx]; }
  float PS_SenY(int arm, int sidx) { return fW_RY[arm][sidx]; }

 protected:
  mxGeometry();

 private:
  static mxGeometry *fGeo;

  float Reference(int,int);
  void UpdateVars(int);
  void UpdateSiW(int);
  void UpdatePbWO4(int);
  int PbWO4_FindIdx(int,float,float);

  int fLastIdx;
  int fLyrIdx;
  int fSenIdx;
  int fSenPktIdx;
  float fX;
  float fY;
  float fZ;

  // ==== PS
  float fSi_a0;
  float fSi_a1;
  float fSi_a2;
  float fSi_RX[96]; // first 48 is south; next 48, north
  float fSi_RY[96]; // first 24 is layer type-x; next 24, type-y
                    // first 12 is top; next 12 is bottom
  float fSi_RZ[16]; // first 8 is south; next 8, north
  int   fSi_LyrNbr[16]; // helps decode layer number
  int   fSi_DLY[4]; // steps for row configuration
  float fW_a0;
  float fW_a1;
  float fW_a2;
  float fW_RX[2][24];
  float fW_RY[2][24];
  int fPS_Sen5N[24][5];
  
  // ==== EM
  float fPbWO4_a0;
  float fPbWO4_a1;
  float fPbWO4_a2;
  int fPbWO4_IdxS;
  float fPbWO4_RX[416]; // first 196 correspond to south
  float fPbWO4_RY[416]; // next 220 correspond to north
  float fPbWO4_RZ[416]; // should reduce this one to just 2?
  int fPbWO4_4N[416][4];
};
#endif /* __mxGeometry_H__ */
