#ifndef __PHMXDATA_H__
#define __PHMXDATA_H__

#include <vector>
#include "PHObject.h"
#include "mxData.h"

class PHMXData:public PHObject {
 public:
  PHMXData() {}
  virtual ~PHMXData() {fData.Reset();}
  mxData* GetData() {return &fData;}
 private:
  mxData fData;
};
#endif /* __PHMXDATA_H__ */
