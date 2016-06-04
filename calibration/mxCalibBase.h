#ifndef __mxCalibBase_HH__
#define __mxCalibBase_HH__
class mxCalibBase {
 public:
  mxCalibBase();
  mxCalibBase( const mxCalibBase &cpy );
  virtual ~mxCalibBase() {}
  float Get(int key);
  void Set(int key, float val);
  void FillWith( const float val[49152] );
 protected:
  float fData[49152];
};
#endif /* __mxCalibBase_H__ */
