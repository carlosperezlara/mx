#ifndef __mxCalibStageSiW_HH__
#define __mxCalibStageSiW_HH__
class mxCalibStageSiW {
 public:
  mxCalibStageSiW();
  mxCalibStageSiW( const mxCalibStageSiW &cpy );
  virtual ~mxCalibStageSiW() {}
  float Get(int key);
  void Set(int key, float val);
  void FillWith( const float val[384] );
  void FillWithConst( const float val );
 protected:
  float fData[384];
};
#endif /* __mxCalibStageSiW_H__ */
