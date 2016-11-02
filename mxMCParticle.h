//=========================
// written by Stacy Karthas
// 2016
//=========================
#ifndef __mxMCParticle_HH__
#define __mxMCParticle_HH__

class mxMCParticle {
 public:
  mxMCParticle();
  mxMCParticle(const mxMCParticle&);
  mxMCParticle& operator=(const mxMCParticle&);
  virtual ~mxMCParticle();
  void Reset();
  void Fill(float, float, float, int);
  float GetEnergy() {return fPEnergy;}
  float GetEta() {return fPEta;}
  float GetPhi() {return fPPhi;}
  int GetPdg() {return fPPdg;}

 protected:
  float fPEnergy;
  float fPEta;
  float fPPhi;
  int fPPdg;

};
#endif /* __mxMCParticle_H__ */
