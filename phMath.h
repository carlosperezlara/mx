#ifndef __phMATH_HH__
#define __phMATH_HH__

#include "TMath.h"

float _phi(const float x, const float y, float& ephi, const float ex=0, const float ey=0);
float _eta(const float x, const float y, const float z, float& eeta, const float ex=0, const float ey=0, const float ez=0);
float _theta(const float x, const float y, const float z, float& etheta, const float ex=0, const float ey=0, const float ez=0);

int _octantify(float &x,  float &y) {
  int o=0;
  if(y<0) { // 180 rotation
    x = -x; y = -y;
    o += 4;
  }
  if(x<0) { // -90 rotation
    x = y; y= -x;
    o += 2;
  }
  if(x<y) o += 1;
  return o;
}


inline
float _phi(const float x, const float y, float& ephi, const float ex, const float ey) {
  // works only for r>0
  float phi = TMath::Pi()+TMath::ATan2(-y,-x);
  //float xx = x;
  //float yy = y;
  //int oct = _octantify(xx,yy);
  //float phi = TMath::Pi()/4.0*oct;
  //float xy = x/y;
  //if(xy<0.2) phi += xy;
  //else {
  //  phi += 0;
  //}
  float den = x*x+y*y;
  ephi = TMath::Sqrt(ex*x*ex*x + ey*y*ey*y) / den;
  return phi;
}
inline
float _eta(const float x, const float y, const float z, float& eeta, const float ex, const float ey, const float ez) {
  // works only for r>0 and r!=z
  float x2 = x*x;
  float y2 = y*y;
  float z2 = z*z;
  float p = TMath::Sqrt(x2+y2+z2);
  float r2 = x2+y2;
  float eta = 0.5*TMath::Log( (p+z)/(p-z) );
  float den = r2*(x2+y2-z2)*(x2+y2-z2);
  if(den>0) eeta = TMath::Sqrt( ez*ez*r2*r2 + z2*(ex*ex*x2 + ey*ey*y2) ) / TMath::Sqrt(den);
  return eta;
}
inline
float _theta(const float x, const float y, const float z, float& etheta, const float ex, const float ey, const float ez) {
  // works only for r>0 and z!=0
  float theta;
  float x2 = x*x;
  float y2 = y*y;
  float z2 = z*z;
  float r2 = x2 + y2 + z2;
  float rt2 = x2 + y2;
  float r = TMath::Sqrt(r2);
  float rt = TMath::Sqrt(rt2);
  theta = TMath::ACos(z/r);
  float den = rt*r2;
  etheta = TMath::Sqrt( ez*rt2*ez*rt2 + z2*(ex*ex*x2 + ey*ey*y2) ) / den;
  return theta;
}


#endif /* __phMATH_HH__ */
