#ifndef __phMATH_HH__
#define __phMATH_HH__

#include "TMath.h"

float _phi(const float x, const float y);
float _eta(const float x, const float y, const float z);

inline
float _phi(const float x, const float y) {
  return TMath::Pi()+TMath::ATan2(-y,-x);
}
inline
float _eta(const float x, const float y, const float z) {
  return 0.5*TMath::Log( (TMath::Sqrt(x*x+y*y+z*z)+z) / (TMath::Sqrt(x*x+y*y+z*z)-z) );
}


#endif /* __phMATH_HH__ */
