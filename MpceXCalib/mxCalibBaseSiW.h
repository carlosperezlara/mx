#ifndef __mxCalibBaseSiW_HH__
#define __mxCalibBaseSiW_HH__
class mxCalibBaseSiW {
 public:
  mxCalibBaseSiW();
  mxCalibBaseSiW( const mxCalibBaseSiW &cpy );
  virtual ~mxCalibBaseSiW() {}
  float Get(int key);
  void Set(int key, float val);
  void FillWith( const float val[49152] );
 protected:
  float fData[49152];
};
#endif /* __mxCalibBaseSiW_H__ */
