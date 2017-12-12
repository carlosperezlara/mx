#ifndef __EMCC_H__
#define __EMCC_H__

class EMCC {
 private:
  int   fArm;
  int   fsector;
  int   fID;
  float fX;
  float fY;           
  float fZ;           
  float fEcore;
  float fProbPhoton;
  float fEMCdz;
  float fEMCdphi;
  int fToweriypos;
  int fTowerizpos;
  
 public:
  EMCC();
  virtual ~EMCC();
  
  int      GetArm() const { return fArm; };
  int      Getsector() const { return fsector; };
  int      GetID() const { return fID; };
  float    GetX() const { return fX; };
  float    GetY() const { return fY; };
  float    GetZ() const { return fZ; };
  float    GetEcore() const { return fEcore; };
  float    GetProbPhoton() const { return fProbPhoton; };
  float    GetEMCdz() const { return fEMCdz; };
  float    GetEMCdphi() const { return fEMCdphi; };
  int      GetToweriypos() const { return fToweriypos; };
  int      GetTowerizpos() const { return fTowerizpos; };
  
  
  void     SetArm(int val) { fArm = val; }
  void     Setsector(int val) { fsector = val; }
  void     SetID(int val) { fID = val; }
  void     SetX(float val) { fX = val; };
  void     SetY(float val) { fY = val; };
  void     SetZ(float val) { fZ = val; };
  void     SetEcore(float val) { fEcore = val; };
  void     SetProb(float val) { fProbPhoton = val; };
  void     SetEmcdz(float val) { fEMCdz = val; };
  void     SetEmcdphi(float val) { fEMCdphi = val; };
  void     SetToweriypos(int val) { fToweriypos = val; };
  void     SetTowerizpos(int val) { fTowerizpos = val; };
};

#endif
