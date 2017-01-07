#ifndef __mxCalibBasePbWO4_HH__
#define __mxCalibBasePbWO4_HH__
class mxCalibBasePbWO4 {
 public:
  mxCalibBasePbWO4();
  mxCalibBasePbWO4( const mxCalibBasePbWO4 &cpy );
  virtual ~mxCalibBasePbWO4() {}
  float Get(int key);
  void Set(int key, float val);
  void FillWith( const float val[416] );
 protected:
  float fData[416];
};
#endif /* __mxCalibBasePbWO4_H__ */
