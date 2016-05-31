#include "mxGeometry.h"

mxGeometry::mxGeometry() :
  fLastIdx(-1),
  fLyrIdx(-1),
  fX(0),
  fY(0),
  fZ(0),
  fSi_a0(0.186),
  fSi_a1(1.490),
  fSi_a2(0.050),
  fW_a0(6.400),
  fW_a1(6.400),
  fW_a2(0.204),
  fPbSc_a0(2.100),
  fPbSc_a1(2.100),
  fPbSc_a2(25.00)
{
  float mx[96] = {
    +3.007, -3.593, -3.593, -10.193, -6.893, -13.493, +9.607, +3.007, +9.607, +16.207, +12.907, +19.507, 
    -3.007, +3.593, +3.593, +10.193, +6.893, +13.493, -9.607, -3.007, -9.607, -16.207, -12.907, -19.507, 
    -0.757, -7.357, -7.357, -13.957, -10.657, -17.257, +5.843, -0.757, +5.843, +12.443, +9.143, +15.743, 
    +0.757, +7.357, +7.357, +13.957, +10.657, +17.257, -5.843, +0.757, -5.843, -12.443, -9.143, -15.743, 
    -3.007, +3.593, +3.593, +10.193, +6.893, +13.493, -9.607, -3.007, -9.607, -16.207, -12.907, -19.507, 
    +3.007, -3.593, -3.593, -10.193, -6.893, -13.493, +9.607, +3.007, +9.607, +16.207, +12.907, +19.507, 
    +0.757, +7.357, +7.357, +13.957, +10.657, +17.257, -5.843, +0.757, -5.843, -12.443, -9.143, -15.743, 
    -0.757, -7.357, -7.357, -13.957, -10.657, -17.257, +5.843, -0.757, +5.843, +12.443, +9.143, +15.743};
  for(int i=0; i!=96; ++i) fSi_RX[i] = mx[i];
  float my[96] = {
    +17.257, +17.257, +10.657, +10.657, +4.057, +4.057, +17.257, +10.657, +10.657, +10.657, +4.057, +4.057, 
    -17.257, -17.257, -10.657, -10.657, -4.057, -4.057, -17.257, -10.657, -10.657, -10.657, -4.057, -4.057, 
    +19.507, +19.507, +12.907, +12.907, +6.307, +6.307, +19.507, +12.907, +12.907, +12.907, +6.307, +6.307, 
    -19.507, -19.507, -12.907, -12.907, -6.307, -6.307, -19.507, -12.907, -12.907, -12.907, -6.307, -6.307, 
    +17.257, +17.257, +10.657, +10.657, +4.057, +4.057, +17.257, +10.657, +10.657, +10.657, +4.057, +4.057, 
    -17.257, -17.257, -10.657, -10.657, -4.057, -4.057, -17.257, -10.657, -10.657, -10.657, -4.057, -4.057, 
    +19.507, +19.507, +12.907, +12.907, +6.307, +6.307, +19.507, +12.907, +12.907, +12.907, +6.307, +6.307, 
    -19.507, -19.507, -12.907, -12.907, -6.307, -6.307, -19.507, -12.907, -12.907, -12.907, -6.307, -6.307};
  for(int i=0; i!=96; ++i) fSi_RY[i] = my[i];
  float mz[16] = {-203.982, -204.636, -205.29, -205.944, -206.598, -207.252, -207.906, -208.560,
		  +203.982, +204.636, +205.29, +205.944, +206.598, +207.252, +207.906, +208.560};
  for(int i=0; i!=16; ++i) fSi_RZ[i] = mz[i];
  int lyrno[16] = { 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3 };
  for(int i=0; i!=16; ++i) fSi_LyrNbr[i] = lyrno[i];
  int dly[4] = {0,1,-1,-2};
  for(int i=0; i!=4; ++i) fSi_DLY[i] = dly[i];
  float sx[24] = {-6.600, +0.000, +6.600, -13.200, -6.600, +0.000, +6.600, +13.200, -16.500, -9.900, +9.900, +16.500,
		  -16.500, -9.900, +9.900, +16.500, -13.200, -6.600, +0.000, +6.600, +13.200, -6.600, +0.000, +6.600}; 
  for(int i=0; i!=24; ++i) fW_RX[i] = sx[i];
  float sy[24] = {+16.500, +16.500, +16.500, +9.900, +9.900, +9.900, +9.900, +9.900, +3.300, +3.300, +3.300, +3.300,
		  -3.300, -3.300, -3.300, -3.300, -9.900, -9.900, -9.900, -9.900, -9.900, -16.500, -16.500, -16.500};
  for(int i=0; i!=24; ++i) fW_RY[i] = sy[i];
  // MPC
  float mpcx[576] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -5.8, -3.5, -1.3, 0.0, 0.0, 
    0.0, -10.3, -8.1, -5.8, -3.5, -1.3, 0.0, 0.0, -12.6, -10.3, -8.1, -5.8, -3.5, -1.3, 0.0, -14.8, -12.6, -10.3, 
    -8.1, -5.8, -3.5, -1.3, -17.1, -14.8, -12.6, -10.3, -8.1, -5.8, -3.5, -1.3, 0.0, -19.7, -19.7, -19.7, -19.7, -19.7, 
    -19.7, 0.0, -17.4, -17.4, -17.4, -17.4, -17.4, -17.4, -17.4, -17.4, -15.2, -15.2, -15.2, -15.2, -15.2, -15.2, -15.2, -15.2, 
    -12.9, -12.9, -12.9, -12.9, -12.9, -12.9, -12.9, -12.9, -10.6, -10.6, -10.6, -10.6, -10.6, -10.6, -10.6, -10.6, -8.4, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, -8.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, -5.8, -3.5, -1.3, 0.0, 0.0, 0.0, -10.3, -8.1, -5.8, -3.5, -1.3, 0.0, 0.0, -12.6, -10.3, -8.1, -5.8, 
    -3.5, -1.3, 0.0, -14.8, -12.6, -10.3, -8.1, -5.8, -3.5, -1.3, -17.1, -14.8, -12.6, -10.3, -8.1, -5.8, -3.5, -1.3, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.8, 3.5, 1.3, 0.0, 0.0, 
    0.0, 10.3, 8.1, 5.8, 3.5, 1.3, 0.0, 0.0, 12.6, 10.3, 8.1, 5.8, 3.5, 1.3, 0.0, 14.8, 12.6, 10.3, 
    8.1, 5.8, 3.5, 1.3, 17.1, 14.8, 12.6, 10.3, 8.1, 5.8, 3.5, 1.3, 0.0, 19.7, 19.7, 19.7, 19.7, 19.7, 
    19.7, 0.0, 17.4, 17.4, 17.4, 17.4, 17.4, 17.4, 17.4, 17.4, 15.2, 15.2, 15.2, 15.2, 15.2, 15.2, 15.2, 15.2, 
    12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 10.6, 10.6, 10.6, 10.6, 10.6, 10.6, 10.6, 10.6, 8.4, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 8.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 5.8, 3.5, 1.3, 0.0, 0.0, 0.0, 10.3, 8.1, 5.8, 3.5, 1.3, 0.0, 0.0, 12.6, 10.3, 8.1, 5.8, 
    3.5, 1.3, 0.0, 14.8, 12.6, 10.3, 8.1, 5.8, 3.5, 1.3, 17.1, 14.8, 12.6, 10.3, 8.1, 5.8, 3.5, 1.3, 
    0.0, 0.0, 0.0, 0.0, 0.0, 5.8, 3.6, 1.3, 0.0, 0.0, 0.0, 10.3, 8.1, 5.8, 3.6, 1.3, 0.0, 0.0, 
    12.6, 10.3, 8.1, 5.8, 3.6, 1.3, 0.0, 14.9, 12.6, 10.3, 8.1, 5.8, 3.6, 1.3, 17.1, 14.9, 12.6, 10.3, 
    8.1, 5.8, 3.6, 1.3, 17.1, 14.9, 12.6, 10.3, 8.1, 5.8, 3.6, 1.3, 0.0, 0.0, 17.8, 15.5, 13.3, 11.0, 
    8.8, 6.5, 0.0, 20.1, 17.8, 15.5, 13.3, 11.0, 8.8, 0.0, 0.0, 20.1, 17.8, 15.5, 13.3, 11.0, 8.8, 0.0, 
    0.0, 20.1, 17.8, 15.5, 13.3, 11.0, 8.8, 0.0, 0.0, 20.1, 17.8, 15.5, 13.3, 11.0, 8.8, 0.0, 0.0, 0.0, 
    17.8, 15.5, 13.3, 11.0, 8.8, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0, 5.8, 3.6, 1.3, 0.0, 0.0, 0.0, 10.3, 
    8.1, 5.8, 3.6, 1.3, 0.0, 0.0, 12.6, 10.3, 8.1, 5.8, 3.6, 1.3, 0.0, 14.9, 12.6, 10.3, 8.1, 5.8, 
    3.6, 1.3, 17.1, 14.9, 12.6, 10.3, 8.1, 5.8, 3.6, 1.3, 17.1, 14.9, 12.6, 10.3, 8.1, 5.8, 3.6, 1.3, 
    0.0, 0.0, 0.0, 0.0, 0.0, -5.8, -3.6, -1.3, 0.0, 0.0, 0.0, -10.3, -8.1, -5.8, -3.6, -1.3, 0.0, 0.0, 
    -12.6, -10.3, -8.1, -5.8, -3.6, -1.3, 0.0, -14.9, -12.6, -10.3, -8.1, -5.8, -3.6, -1.3, -17.1, -14.9, -12.6, -10.3, 
    -8.1, -5.8, -3.6, -1.3, -17.1, -14.9, -12.6, -10.3, -8.1, -5.8, -3.6, -1.3, 0.0, 0.0, -17.8, -15.5, -13.3, -11.0, 
    -8.8, -6.5, 0.0, -20.1, -17.8, -15.5, -13.3, -11.0, -8.8, 0.0, 0.0, -20.1, -17.8, -15.5, -13.3, -11.0, -8.8, 0.0, 
    0.0, -20.1, -17.8, -15.5, -13.3, -11.0, -8.8, 0.0, 0.0, -20.1, -17.8, -15.5, -13.3, -11.0, -8.8, 0.0, 0.0, 0.0, 
    -17.8, -15.5, -13.3, -11.0, -8.8, -6.5, 0.0, 0.0, 0.0, 0.0, 0.0, -5.8, -3.6, -1.3, 0.0, 0.0, 0.0, -10.3, 
    -8.1, -5.8, -3.6, -1.3, 0.0, 0.0, -12.6, -10.3, -8.1, -5.8, -3.6, -1.3, 0.0, -14.9, -12.6, -10.3, -8.1, -5.8, 
    -3.6, -1.3, -17.1, -14.9, -12.6, -10.3, -8.1, -5.8, -3.6, -1.3, -17.1, -14.9, -12.6, -10.3, -8.1, -5.8, -3.6, -1.3};
  for(int i=0; i!=576; ++i) fPbSc_RX[i] = mpcx[i];
  float  mpcy[576] = { 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 19.5, 19.5, 19.5, 0.0, 0.0, 
    0.0, 17.3, 17.3, 17.3, 17.3, 17.3, 0.0, 0.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 0.0, 12.7, 12.7, 12.7, 
    12.7, 12.7, 12.7, 12.7, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 0.0, 5.7, 3.4, 1.1, -1.1, -3.4, 
    -5.7, 0.0, 7.9, 5.7, 3.4, 1.1, -1.1, -3.4, -5.7, -7.9, 7.9, 5.7, 3.4, 1.1, -1.1, -3.4, -5.7, -7.9, 
    7.9, 5.7, 3.4, 1.1, -1.1, -3.4, -5.7, -7.9, 7.9, 5.7, 3.4, 1.1, -1.1, -3.4, -5.7, -7.9, 7.9, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, -7.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, -19.5, -19.5, -19.5, 0.0, 0.0, 0.0, -17.3, -17.3, -17.3, -17.3, -17.3, 0.0, 0.0, -15.0, -15.0, -15.0, -15.0, 
    -15.0, -15.0, 0.0, -12.7, -12.7, -12.7, -12.7, -12.7, -12.7, -12.7, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -19.5, -19.5, -19.5, 0.0, 0.0, 
    0.0, -17.3, -17.3, -17.3, -17.3, -17.3, 0.0, 0.0, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, 0.0, -12.7, -12.7, -12.7, 
    -12.7, -12.7, -12.7, -12.7, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, 0.0, -5.7, -3.4, -1.1, 1.1, 3.4, 
    5.7, 0.0, -7.9, -5.7, -3.4, -1.1, 1.1, 3.4, 5.7, 7.9, -7.9, -5.7, -3.4, -1.1, 1.1, 3.4, 5.7, 7.9, 
    -7.9, -5.7, -3.4, -1.1, 1.1, 3.4, 5.7, 7.9, -7.9, -5.7, -3.4, -1.1, 1.1, 3.4, 5.7, 7.9, -7.9, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 7.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 19.5, 19.5, 19.5, 0.0, 0.0, 0.0, 17.3, 17.3, 17.3, 17.3, 17.3, 0.0, 0.0, 15.0, 15.0, 15.0, 15.0, 
    15.0, 15.0, 0.0, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 
    0.0, 0.0, 0.0, 0.0, 0.0, 19.6, 19.6, 19.6, 0.0, 0.0, 0.0, 17.3, 17.3, 17.3, 17.3, 17.3, 0.0, 0.0, 
    15.1, 15.1, 15.1, 15.1, 15.1, 15.1, 0.0, 12.8, 12.8, 12.8, 12.8, 12.8, 12.8, 12.8, 10.5, 10.5, 10.5, 10.5, 
    10.5, 10.5, 10.5, 10.5, 8.3, 8.3, 8.3, 8.3, 8.3, 8.3, 8.3, 8.3, 0.0, 0.0, 5.7, 5.7, 5.7, 5.7, 
    5.7, 5.7, 0.0, 3.4, 3.4, 3.4, 3.4, 3.4, 3.4, 0.0, 0.0, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 0.0, 
    0.0, -1.1, -1.1, -1.1, -1.1, -1.1, -1.1, 0.0, 0.0, -3.4, -3.4, -3.4, -3.4, -3.4, -3.4, 0.0, 0.0, 0.0, 
    -5.6, -5.6, -5.6, -5.6, -5.6, -5.6, 0.0, 0.0, 0.0, 0.0, 0.0, -19.6, -19.6, -19.6, 0.0, 0.0, 0.0, -17.3, 
    -17.3, -17.3, -17.3, -17.3, 0.0, 0.0, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, 0.0, -12.8, -12.8, -12.8, -12.8, -12.8, 
    -12.8, -12.8, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -10.5, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, 
    0.0, 0.0, 0.0, 0.0, 0.0, -19.6, -19.6, -19.6, 0.0, 0.0, 0.0, -17.3, -17.3, -17.3, -17.3, -17.3, 0.0, 0.0, 
    -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, 0.0, -12.8, -12.8, -12.8, -12.8, -12.8, -12.8, -12.8, -10.5, -10.5, -10.5, -10.5, 
    -10.5, -10.5, -10.5, -10.5, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, 0.0, 0.0, 5.7, 5.7, 5.7, 5.7, 
    5.7, 5.7, 0.0, 3.4, 3.4, 3.4, 3.4, 3.4, 3.4, 0.0, 0.0, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 0.0, 
    0.0, -1.1, -1.1, -1.1, -1.1, -1.1, -1.1, 0.0, 0.0, -3.4, -3.4, -3.4, -3.4, -3.4, -3.4, 0.0, 0.0, 0.0, 
    -5.6, -5.6, -5.6, -5.6, -5.6, -5.6, 0.0, 0.0, 0.0, 0.0, 0.0, 19.6, 19.6, 19.6, 0.0, 0.0, 0.0, 17.3, 
    17.3, 17.3, 17.3, 17.3, 0.0, 0.0, 15.1, 15.1, 15.1, 15.1, 15.1, 15.1, 0.0, 12.8, 12.8, 12.8, 12.8, 12.8, 
    12.8, 12.8, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 10.5, 8.3, 8.3, 8.3, 8.3, 8.3, 8.3, 8.3, 8.3};
  for(int i=0; i!=576; ++i) fPbSc_RY[i] = mpcy[i];
  float mpcz[576] = { 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -220.9, -220.9, -220.9, 0.0, 0.0, 
    0.0, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, -220.9, -220.9, -220.9, 
    -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, 
    -220.9, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 
    -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, -220.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, -220.9, -220.9, -220.9, 0.0, 0.0, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, 0.0, -220.9, -220.9, -220.9, -220.9, 
    -220.9, -220.9, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -220.9, -220.9, -220.9, 0.0, 0.0, 
    0.0, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, -220.9, -220.9, -220.9, 
    -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, 
    -220.9, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 
    -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, -220.9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, -220.9, -220.9, -220.9, 0.0, 0.0, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, 0.0, 0.0, -220.9, -220.9, -220.9, -220.9, 
    -220.9, -220.9, 0.0, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, -220.9, 
    0.0, 0.0, 0.0, 0.0, 0.0, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 
    220.9, 220.9, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 
    0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 0.0, 0.0, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 220.9, 
    220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 
    0.0, 0.0, 0.0, 0.0, 0.0, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 
    220.9, 220.9, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 
    0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 0.0, 0.0, 220.9, 220.9, 220.9, 0.0, 0.0, 0.0, 220.9, 
    220.9, 220.9, 220.9, 220.9, 0.0, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 0.0, 220.9, 220.9, 220.9, 220.9, 220.9, 
    220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9, 220.9};
  for(int i=0; i!=576; ++i) fPbSc_RZ[i] = mpcz[i];

}
//========
float mxGeometry::Reference(int k, int r) {
  if(k!=fLastIdx) UpdateVars(k);
  if(r==0) return fX;
  if(r==1) return fY;
  if(r==2) return fZ;
  if(r==3) return fLyrIdx;
  return -1;
}
//========
void mxGeometry::UpdateVars(int k) {
  // first 49152 correspond to key for SiW
  // next NNNN correspond to chn for PbSc
  if(k<49152) UpdateSiW(k);
  else UpdatePbSc(k-49152);
}
//========
float mxGeometry::W_X(int sidx) {
  int sen = sidx%24;
  return fW_RX[sen];
}
//========
float mxGeometry::W_Y(int sidx) {
  int sen = sidx%24;
  return fW_RY[sen];
}
//========
float mxGeometry::W_Z(int sidx) {
  int lyr = sidx/24;
  float dz = 0.1 + fW_a2;
  float z = fSi_RZ[lyr] + dz*(lyr<8?+1:-1);
  return z;
}
//========
void mxGeometry::UpdateSiW(int k) {
  int arm = k/24576;
  int pkt = (k/3072)%8;
  int sen = (k%3072)/128;
  int lyr = fSi_LyrNbr[ 2*pkt + (sen/12) ];
  int senlyr = sen%12;
  int sgnTB = 1;
  int sgnAR = 1;
  if(pkt>3) {
    senlyr += 12;
    sgnTB = -1;
  }
  if(arm>0) sgnAR = -1;
  int typ = lyr % 2;
  int ref = arm*48 + typ*24 + senlyr;
  float x = fSi_RX[ref];
  float y = fSi_RY[ref];
  float nx = fSi_RX[ref]/ (typ==0?fSi_a0:fSi_a1);
  float ny = fSi_RY[ref]/ (typ==0?fSi_a1:fSi_a0);
  if(typ==0) {
    x -= sgnAR*sgnTB*((k%128)/4)*(fSi_a0+0.008);
    y += sgnTB*fSi_DLY[k%4]*(fSi_a1+0.008);
    nx -= sgnAR*sgnTB*((k%128)/4);
    ny += sgnTB*fSi_DLY[k%4];
  } else {
    y -= sgnTB*((k%128)/4)*(fSi_a0+0.008);
    x -= sgnAR*sgnTB*fSi_DLY[k%4]*(fSi_a1+0.008);
    ny -= sgnTB*((k%128)/4);
    nx -= sgnAR*sgnTB*fSi_DLY[k%4];
  }
  fX = x;
  fY = y;
  fZ = fSi_RZ[arm*8+lyr];
  fLyrIdx = arm*9+lyr;
  fLastIdx = k;
}
//========
void mxGeometry::UpdatePbSc(int k) {
  fX = fPbSc_RX[k];
  fY = fPbSc_RY[k];
  fZ = fPbSc_RZ[k];
  int arm = k/288;
  fLyrIdx = arm*9+8;
  fLastIdx = k;
}
