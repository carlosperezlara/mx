#ifndef __phMATH_HH__
#define __phMATH_HH__

#include "TMath.h"

float _phi(const float x, const float y, float& ephi, const float ex=0, const float ey=0);
float _eta(const float x, const float y, const float z, float& eeta, const float ex=0, const float ey=0, const float ez=0);

inline
float _phi(const float x, const float y, float& ephi, const float ex, const float ey) {
  float phi = TMath::Pi()+TMath::ATan2(-y,-x);
  float den = x*x+y*y;
  if(den>0) ephi = TMath::Sqrt(ex*x*ex*x + ey*y*ey*y) / den;
  return phi;
}
inline
float _eta(const float x, const float y, const float z, float& eeta, const float ex, const float ey, const float ez) {
  float x2 = x*x;
  float y2 = y*y;
  float z2 = z*z;
  float p = TMath::Sqrt(x2+y2+z2);
  float r2 = x2+y2;
  float eta = 0.5*TMath::Log( (p+z)/(p-z) );
  float den = r2*r2*p*p;
  if(den>0) eeta = TMath::Sqrt( ez*ez*r2*r2 + z2*(ex*ex*x2 + ey*ey*y2) ) / TMath::Sqrt(den);
  return eta;
}


#endif /* __phMATH_HH__ */
