#ifndef __PHMXDATA_H__
#define __PHMXDATA_H__

#include <vector>
#include "PHObject.h"
#include "mxReconstruction.h"

class PHMXData:public PHObject {
 public:
  PHMXData() {}
  virtual ~PHMXData() {fReco.Reset();}
  mxReconstruction* GetReconstruction() {return &fReco;}
 private:
  mxReconstruction fReco;
};
#endif /* __PHMXDATA_H__ */
