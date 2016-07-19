//=========================
// written by Carlos Perez
// 2015-2016
//=========================
#ifndef __mxUnion_HH__
#define __mxUnion_HH__

class mxCoalition;

class mxUnion {
 public:
  mxUnion();
  mxUnion(const mxUnion&);
  mxUnion& operator=(const mxUnion&);
  virtual ~mxUnion();
  void Make(mxCoalition*,mxCoalition*);
  mxCoalition* GetCoalition(int val) {return val<2?fCoalitions[val]:NULL;}
  float GetEta() {return fEta;}
  float GetPhi() {return fPhi;}
  float GetDPhi() {return fDPhi;}
  float GetEnergy() {return fEnergy;}
  float GetEnergyT() {return fEnergyT;}
  float GetEnergyL() {return fEnergyL;}
  float GetMass() {return fMass;}

 protected:
  mxCoalition* fCoalitions[2];
  float fEta;
  float fPhi;
  float fDPhi;
  float fEnergy;
  float fEnergyT;
  float fEnergyL;
  float fMass;
};
#endif /* __mxUnion_H__ */
