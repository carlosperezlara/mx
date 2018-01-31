#include <iostream>
#include "mxGeometry.h"

mxGeometry *mxGeometry::fGeo = NULL;

mxGeometry::mxGeometry() :
  fLastIdx(-1),
  fLyrIdx(-1),
  fSenIdx(-1),
  fX(0),
  fY(0),
  fZ(0),
  fSi_a0(0.186),
  fSi_a1(1.490),
  fSi_a2(0.050),
  fW_a0(6.400),
  fW_a1(6.400),
  fW_a2(0.204),
  fPbWO4_a0(2.100),
  fPbWO4_a1(2.100),
  fPbWO4_a2(25.00),
  fPbWO4_IdxS(196)
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
  float sy[24] = {+16.500, +16.500, +16.500, +9.900, +9.900, +9.900, +9.900, +9.900, +3.300, +3.300, +3.300, +3.300,
		  -3.300, -3.300, -3.300, -3.300, -9.900, -9.900, -9.900, -9.900, -9.900, -16.500, -16.500, -16.500};
  int si0[24] = { 01, 0, 4, 3, 9, 8,  2, 5, 6, 7,10,11,
		  22,23,19,20,14,15, 21,18,17,16,13,12};
  int si1[24] = { 01, 2, 6, 7,10,11,  0, 5, 4, 3, 9, 8,
		  22,21,17,16,13,12, 23,18,19,20,14,15};
  for(int i=0; i!=24; ++i) {
    fW_RX[0][i] = sx[ si0[i] ];
    fW_RY[0][i] = sy[ si0[i] ];
    fW_RX[1][i] = sx[ si1[i] ];
    fW_RY[1][i] = sy[ si1[i] ];
  }
  // MPC
  float mpcx[416] = {
    -5.8,-3.5,-1.3,-10.3,-8.1,-5.8,-3.5,-1.3,-12.6,-10.3,-8.1,-5.8,-3.5,-1.3,-14.8,-12.6,-10.3,-8.1,-5.8,-3.5,
    -1.3,-17.1,-14.8,-12.6,-10.3,-8.1,-5.8,-3.5,-1.3,-19.7,-19.7,-19.7,-19.7,-19.7,-19.7,-17.4,-17.4,-17.4,-17.4,-17.4,
    -17.4,-17.4,-17.4,-15.2,-15.2,-15.2,-15.2,-15.2,-15.2,-15.2,-15.2,-12.9,-12.9,-12.9,-12.9,-12.9,-12.9,-12.9,-12.9,-10.6,
    -10.6,-10.6,-10.6,-10.6,-10.6,-10.6,-10.6,-8.4,-8.4,-5.8,-3.5,-1.3,-10.3,-8.1,-5.8,-3.5,-1.3,-12.6,-10.3,-8.1,
    -5.8,-3.5,-1.3,-14.8,-12.6,-10.3,-8.1,-5.8,-3.5,-1.3,-17.1,-14.8,-12.6,-10.3,-8.1,-5.8,-3.5,-1.3,5.8,3.5,
    1.3,10.3,8.1,5.8,3.5,1.3,12.6,10.3,8.1,5.8,3.5,1.3,14.8,12.6,10.3,8.1,5.8,3.5,1.3,17.1,
    14.8,12.6,10.3,8.1,5.8,3.5,1.3,19.7,19.7,19.7,19.7,19.7,19.7,17.4,17.4,17.4,17.4,17.4,17.4,17.4,
    17.4,15.2,15.2,15.2,15.2,15.2,15.2,15.2,15.2,12.9,12.9,12.9,12.9,12.9,12.9,12.9,12.9,10.6,10.6,10.6,
    10.6,10.6,10.6,10.6,10.6,8.4,8.4,5.8,3.5,1.3,10.3,8.1,5.8,3.5,1.3,12.6,10.3,8.1,5.8,3.5,
    1.3,14.8,12.6,10.3,8.1,5.8,3.5,1.3,17.1,14.8,12.6,10.3,8.1,5.8,3.5,1.3,5.8,3.6,1.3,10.3,
    8.1,5.8,3.6,1.3,12.6,10.3,8.1,5.8,3.6,1.3,14.9,12.6,10.3,8.1,5.8,3.6,1.3,17.1,14.9,12.6,
    10.3,8.1,5.8,3.6,1.3,17.1,14.9,12.6,10.3,8.1,5.8,3.6,1.3,17.8,15.5,13.3,11.0,8.8,6.5,20.1,
    17.8,15.5,13.3,11.0,8.8,20.1,17.8,15.5,13.3,11.0,8.8,20.1,17.8,15.5,13.3,11.0,8.8,20.1,17.8,15.5,
    13.3,11.0,8.8,17.8,15.5,13.3,11.0,8.8,6.5,5.8,3.6,1.3,10.3,8.1,5.8,3.6,1.3,12.6,10.3,8.1,
    5.8,3.6,1.3,14.9,12.6,10.3,8.1,5.8,3.6,1.3,17.1,14.9,12.6,10.3,8.1,5.8,3.6,1.3,17.1,14.9,
    12.6,10.3,8.1,5.8,3.6,1.3,-5.8,-3.6,-1.3,-10.3,-8.1,-5.8,-3.6,-1.3,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3,
    -14.9,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3,-17.1,-14.9,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3,-17.1,-14.9,-12.6,-10.3,-8.1,
    -5.8,-3.6,-1.3,-17.8,-15.5,-13.3,-11.0,-8.8,-6.5,-20.1,-17.8,-15.5,-13.3,-11.0,-8.8,-20.1,-17.8,-15.5,-13.3,-11.0,
    -8.8,-20.1,-17.8,-15.5,-13.3,-11.0,-8.8,-20.1,-17.8,-15.5,-13.3,-11.0,-8.8,-17.8,-15.5,-13.3,-11.0,-8.8,-6.5,-5.8,
    -3.6,-1.3,-10.3,-8.1,-5.8,-3.6,-1.3,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3,-14.9,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3,
    -17.1,-14.9,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3,-17.1,-14.9,-12.6,-10.3,-8.1,-5.8,-3.6,-1.3};
  for(int i=0; i!=416; ++i) fPbWO4_RX[i] = mpcx[i];
  float  mpcy[416] = {
    19.5,19.5,19.5,17.3,17.3,17.3,17.3,17.3,15.0,15.0,15.0,15.0,15.0,15.0,12.7,12.7,12.7,12.7,12.7,12.7,
    12.7,10.5,10.5,10.5,10.5,10.5,10.5,10.5,10.5,5.7,3.4,1.1,-1.1,-3.4,-5.7,7.9,5.7,3.4,1.1,-1.1,
    -3.4,-5.7,-7.9,7.9,5.7,3.4,1.1,-1.1,-3.4,-5.7,-7.9,7.9,5.7,3.4,1.1,-1.1,-3.4,-5.7,-7.9,7.9,
    5.7,3.4,1.1,-1.1,-3.4,-5.7,-7.9,7.9,-7.9,-19.5,-19.5,-19.5,-17.3,-17.3,-17.3,-17.3,-17.3,-15.0,-15.0,-15.0,
    -15.0,-15.0,-15.0,-12.7,-12.7,-12.7,-12.7,-12.7,-12.7,-12.7,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-19.5,-19.5,
    -19.5,-17.3,-17.3,-17.3,-17.3,-17.3,-15.0,-15.0,-15.0,-15.0,-15.0,-15.0,-12.7,-12.7,-12.7,-12.7,-12.7,-12.7,-12.7,-10.5,
    -10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-5.7,-3.4,-1.1,1.1,3.4,5.7,-7.9,-5.7,-3.4,-1.1,1.1,3.4,5.7,
    7.9,-7.9,-5.7,-3.4,-1.1,1.1,3.4,5.7,7.9,-7.9,-5.7,-3.4,-1.1,1.1,3.4,5.7,7.9,-7.9,-5.7,-3.4,
    -1.1,1.1,3.4,5.7,7.9,-7.9,7.9,19.5,19.5,19.5,17.3,17.3,17.3,17.3,17.3,15.0,15.0,15.0,15.0,15.0,
    15.0,12.7,12.7,12.7,12.7,12.7,12.7,12.7,10.5,10.5,10.5,10.5,10.5,10.5,10.5,10.5,19.6,19.6,19.6,17.3,
    17.3,17.3,17.3,17.3,15.1,15.1,15.1,15.1,15.1,15.1,12.8,12.8,12.8,12.8,12.8,12.8,12.8,10.5,10.5,10.5,
    10.5,10.5,10.5,10.5,10.5,8.3,8.3,8.3,8.3,8.3,8.3,8.3,8.3,5.7,5.7,5.7,5.7,5.7,5.7,3.4,
    3.4,3.4,3.4,3.4,3.4,1.1,1.1,1.1,1.1,1.1,1.1,-1.1,-1.1,-1.1,-1.1,-1.1,-1.1,-3.4,-3.4,-3.4,
    -3.4,-3.4,-3.4,-5.6,-5.6,-5.6,-5.6,-5.6,-5.6,-19.6,-19.6,-19.6,-17.3,-17.3,-17.3,-17.3,-17.3,-15.0,-15.0,-15.0,
    -15.0,-15.0,-15.0,-12.8,-12.8,-12.8,-12.8,-12.8,-12.8,-12.8,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-8.3,-8.3,
    -8.3,-8.3,-8.3,-8.3,-8.3,-8.3,-19.6,-19.6,-19.6,-17.3,-17.3,-17.3,-17.3,-17.3,-15.0,-15.0,-15.0,-15.0,-15.0,-15.0,
    -12.8,-12.8,-12.8,-12.8,-12.8,-12.8,-12.8,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-10.5,-8.3,-8.3,-8.3,-8.3,-8.3,
    -8.3,-8.3,-8.3,5.7,5.7,5.7,5.7,5.7,5.7,3.4,3.4,3.4,3.4,3.4,3.4,1.1,1.1,1.1,1.1,1.1,
    1.1,-1.1,-1.1,-1.1,-1.1,-1.1,-1.1,-3.4,-3.4,-3.4,-3.4,-3.4,-3.4,-5.6,-5.6,-5.6,-5.6,-5.6,-5.6,19.6,
    19.6,19.6,17.3,17.3,17.3,17.3,17.3,15.1,15.1,15.1,15.1,15.1,15.1,12.8,12.8,12.8,12.8,12.8,12.8,12.8,
    10.5,10.5,10.5,10.5,10.5,10.5,10.5,10.5,8.3,8.3,8.3,8.3,8.3,8.3,8.3,8.3};
  for(int i=0; i!=416; ++i) fPbWO4_RY[i] = mpcy[i];
  fPbWO4_RZ[0] = -220.9;
  fPbWO4_RZ[1] = +220.9;
  int mpcneigh[416][4] = {
    {1,5,-1,-1},  {0,2,6,-1},  {1,7,169,-1},  {4,9,-1,-1},  {3,5,10,-1},  {0,4,6,11},  {1,5,7,12},  {2,6,13,174}, 
    {9,15,-1,-1},  {3,8,10,16},  {4,9,11,17},  {5,10,12,18},  {6,11,13,19},  {7,12,20,180},  {15,22,-1,-1},  {8,14,16,23}, 
    {9,15,17,24},  {10,16,18,25},  {11,17,19,26},  {12,18,20,27},  {13,19,28,187},  {22,35,-1,-1},  {14,21,23,43},  {15,22,24,51}, 
    {16,23,25,59},  {17,24,26,67},  {18,25,27,-1},  {19,26,28,-1},  {20,27,195,-1},  {30,36,-1,-1},  {29,31,37,-1},  {30,32,38,-1}, 
    {31,33,39,-1},  {32,34,40,-1},  {33,41,-1,-1},  {21,36,43,-1},  {29,35,37,44},  {30,36,38,45},  {31,37,39,46},  {32,38,40,47}, 
    {33,39,41,48},  {34,40,42,49},  {41,50,90,-1},  {22,35,44,51},  {36,43,45,52},  {37,44,46,53},  {38,45,47,54},  {39,46,48,55}, 
    {40,47,49,56},  {41,48,50,57},  {42,49,58,91},  {23,43,52,59},  {44,51,53,60},  {45,52,54,61},  {46,53,55,62},  {47,54,56,63}, 
    {48,55,57,64},  {49,56,58,65},  {50,57,66,92},  {24,51,60,67},  {52,59,61,-1},  {53,60,62,-1},  {54,61,63,-1},  {55,62,64,-1}, 
    {56,63,65,-1},  {57,64,66,-1},  {58,65,68,93},  {25,59,-1,-1},  {66,94,-1,-1},  {70,74,-1,-1},  {69,71,75,-1},  {70,76,100,-1}, 
    {73,78,-1,-1},  {72,74,79,-1},  {69,73,75,80},  {70,74,76,81},  {71,75,82,105},  {78,84,-1,-1},  {72,77,79,85},  {73,78,80,86}, 
    {74,79,81,87},  {75,80,82,88},  {76,81,89,111},  {84,91,-1,-1},  {77,83,85,92},  {78,84,86,93},  {79,85,87,94},  {80,86,88,95}, 
    {81,87,89,96},  {82,88,97,118},  {42,91,-1,-1},  {50,83,90,92},  {58,84,91,93},  {66,85,92,94},  {68,86,93,95},  {87,94,96,-1}, 
    {88,95,97,-1},  {89,96,126,-1},  {99,103,-1,-1},  {98,100,104,-1},  {71,99,105,-1},  {102,107,-1,-1},  {101,103,108,-1},  {98,102,104,109}, 
    {99,103,105,110},  {76,100,104,111},  {107,113,-1,-1},  {101,106,108,114},  {102,107,109,115},  {103,108,110,116},  {104,109,111,117},  {82,105,110,118}, 
    {113,120,-1,-1},  {106,112,114,121},  {107,113,115,122},  {108,114,116,123},  {109,115,117,124},  {110,116,118,125},  {89,111,117,126},  {120,133,-1,-1}, 
    {112,119,121,141},  {113,120,122,149},  {114,121,123,157},  {115,122,124,165},  {116,123,125,-1},  {117,124,126,-1},  {97,118,125,-1},  {128,134,-1,-1}, 
    {127,129,135,-1},  {128,130,136,-1},  {129,131,137,-1},  {130,132,138,-1},  {131,139,-1,-1},  {119,134,141,-1},  {127,133,135,142},  {128,134,136,143}, 
    {129,135,137,144},  {130,136,138,145},  {131,137,139,146},  {132,138,140,147},  {139,148,188,-1},  {120,133,142,149},  {134,141,143,150},  {135,142,144,151}, 
    {136,143,145,152},  {137,144,146,153},  {138,145,147,154},  {139,146,148,155},  {140,147,156,189},  {121,141,150,157},  {142,149,151,158},  {143,150,152,159}, 
    {144,151,153,160},  {145,152,154,161},  {146,153,155,162},  {147,154,156,163},  {148,155,164,190},  {122,149,158,165},  {150,157,159,-1},  {151,158,160,-1}, 
    {152,159,161,-1},  {153,160,162,-1},  {154,161,163,-1},  {155,162,164,-1},  {156,163,166,191},  {123,157,-1,-1},  {164,192,-1,-1},  {168,172,-1,-1}, 
    {167,169,173,-1},  {2,168,174,-1},  {171,176,-1,-1},  {170,172,177,-1},  {167,171,173,178},  {168,172,174,179},  {7,169,173,180},  {176,182,-1,-1}, 
    {170,175,177,183},  {171,176,178,184},  {172,177,179,185},  {173,178,180,186},  {13,174,179,187},  {182,189,-1,-1},  {175,181,183,190},  {176,182,184,191}, 
    {177,183,185,192},  {178,184,186,193},  {179,185,187,194},  {20,180,186,195},  {140,189,-1,-1},  {148,181,188,190},  {156,182,189,191},  {164,183,190,192}, 
    {166,184,191,193},  {185,192,194,-1},  {186,193,195,-1},  {28,187,194,-1},  {197,201,-1,-1},  {196,198,202,-1},  {197,203,381,-1},  {200,205,-1,-1}, 
    {199,201,206,-1},  {196,200,202,207},  {197,201,203,208},  {198,202,209,386},  {205,211,-1,-1},  {199,204,206,212},  {200,205,207,213},  {201,206,208,214}, 
    {202,207,209,215},  {203,208,216,392},  {211,218,-1,-1},  {204,210,212,219},  {205,211,213,220},  {206,212,214,221},  {207,213,215,222},  {208,214,216,223}, 
    {209,215,224,399},  {218,225,-1,-1},  {210,217,219,226},  {211,218,220,227},  {212,219,221,228},  {213,220,222,229},  {214,221,223,230},  {215,222,224,231}, 
    {216,223,232,407},  {217,226,233,-1},  {218,225,227,234},  {219,226,228,235},  {220,227,229,236},  {221,228,230,237},  {222,229,231,238},  {223,230,232,-1}, 
    {224,231,415,-1},  {225,234,240,-1},  {226,233,235,241},  {227,234,236,242},  {228,235,237,243},  {229,236,238,244},  {230,237,-1,-1},  {240,245,-1,-1}, 
    {233,239,241,246},  {234,240,242,247},  {235,241,243,248},  {236,242,244,249},  {237,243,250,-1},  {239,246,251,-1},  {240,245,247,252},  {241,246,248,253}, 
    {242,247,249,254},  {243,248,250,255},  {244,249,256,-1},  {245,252,257,-1},  {246,251,253,258},  {247,252,254,259},  {248,253,255,260},  {249,254,256,261}, 
    {250,255,262,-1},  {251,258,-1,-1},  {252,257,259,263},  {253,258,260,264},  {254,259,261,265},  {255,260,262,266},  {256,261,267,-1},  {258,264,298,-1}, 
    {259,263,265,299},  {260,264,266,300},  {261,265,267,301},  {262,266,268,302},  {267,303,-1,-1},  {270,274,-1,-1},  {269,271,275,-1},  {270,276,308,-1}, 
    {273,278,-1,-1},  {272,274,279,-1},  {269,273,275,280},  {270,274,276,281},  {271,275,282,313},  {278,284,-1,-1},  {272,277,279,285},  {273,278,280,286}, 
    {274,279,281,287},  {275,280,282,288},  {276,281,289,319},  {284,291,-1,-1},  {277,283,285,292},  {278,284,286,293},  {279,285,287,294},  {280,286,288,295}, 
    {281,287,289,296},  {282,288,297,326},  {291,298,-1,-1},  {283,290,292,299},  {284,291,293,300},  {285,292,294,301},  {286,293,295,302},  {287,294,296,303}, 
    {288,295,297,304},  {289,296,305,334},  {263,290,299,-1},  {264,291,298,300},  {265,292,299,301},  {266,293,300,302},  {267,294,301,303},  {268,295,302,304}, 
    {296,303,305,-1},  {297,304,342,-1},  {307,311,-1,-1},  {306,308,312,-1},  {271,307,313,-1},  {310,315,-1,-1},  {309,311,316,-1},  {306,310,312,317}, 
    {307,311,313,318},  {276,308,312,319},  {315,321,-1,-1},  {309,314,316,322},  {310,315,317,323},  {311,316,318,324},  {312,317,319,325},  {282,313,318,326}, 
    {321,328,-1,-1},  {314,320,322,329},  {315,321,323,330},  {316,322,324,331},  {317,323,325,332},  {318,324,326,333},  {289,319,325,334},  {328,335,-1,-1}, 
    {320,327,329,336},  {321,328,330,337},  {322,329,331,338},  {323,330,332,339},  {324,331,333,340},  {325,332,334,341},  {297,326,333,342},  {327,336,373,-1}, 
    {328,335,337,374},  {329,336,338,375},  {330,337,339,376},  {331,338,340,377},  {332,339,341,378},  {333,340,342,-1},  {305,334,341,-1},  {344,350,408,-1}, 
    {343,345,351,409},  {344,346,352,410},  {345,347,353,411},  {346,348,354,412},  {347,413,-1,-1},  {350,355,-1,-1},  {343,349,351,356},  {344,350,352,357}, 
    {345,351,353,358},  {346,352,354,359},  {347,353,360,-1},  {349,356,361,-1},  {350,355,357,362},  {351,356,358,363},  {352,357,359,364},  {353,358,360,365}, 
    {354,359,366,-1},  {355,362,367,-1},  {356,361,363,368},  {357,362,364,369},  {358,363,365,370},  {359,364,366,371},  {360,365,372,-1},  {361,368,-1,-1}, 
    {362,367,369,373},  {363,368,370,374},  {364,369,371,375},  {365,370,372,376},  {366,371,377,-1},  {335,368,374,-1},  {336,369,373,375},  {337,370,374,376}, 
    {338,371,375,377},  {339,372,376,378},  {340,377,-1,-1},  {380,384,-1,-1},  {379,381,385,-1},  {198,380,386,-1},  {383,388,-1,-1},  {382,384,389,-1}, 
    {379,383,385,390},  {380,384,386,391},  {203,381,385,392},  {388,394,-1,-1},  {382,387,389,395},  {383,388,390,396},  {384,389,391,397},  {385,390,392,398}, 
    {209,386,391,399},  {394,401,-1,-1},  {387,393,395,402},  {388,394,396,403},  {389,395,397,404},  {390,396,398,405},  {391,397,399,406},  {216,392,398,407}, 
    {401,408,-1,-1},  {393,400,402,409},  {394,401,403,410},  {395,402,404,411},  {396,403,405,412},  {397,404,406,413},  {398,405,407,414},  {224,399,406,415}, 
    {343,400,409,-1},  {344,401,408,410},  {345,402,409,411},  {346,403,410,412},  {347,404,411,413},  {348,405,412,414},  {406,413,415,-1},  {232,407,414,-1}
  };
  for(int i=0; i!=416; ++i) for(int j=0; j!=4; ++j) fPbWO4_4N[i][j] = mpcneigh[i][j];
  for(int i=0; i!=24; ++i) for(int n=0; n!=5; ++n) fPS_Sen5N[i][n] = -1;
  fPS_Sen5N[0][0] = 6; fPS_Sen5N[0][1] = 1; fPS_Sen5N[0][2] = 8; fPS_Sen5N[0][3] = 7; fPS_Sen5N[0][4] = 2;
  fPS_Sen5N[1][0] = 0; fPS_Sen5N[1][1] = 7; fPS_Sen5N[1][2] = 2; fPS_Sen5N[1][3] = 3;
  fPS_Sen5N[2][0] = 0; fPS_Sen5N[2][1] = 1; fPS_Sen5N[2][2] = 7; fPS_Sen5N[2][3] = 3; fPS_Sen5N[2][4] = 4;
  fPS_Sen5N[3][0] = 1; fPS_Sen5N[3][1] = 2; fPS_Sen5N[3][2] = 4; fPS_Sen5N[3][3] = 5;
  fPS_Sen5N[4][0] = 2; fPS_Sen5N[4][1] = 3; fPS_Sen5N[4][2] = 5; fPS_Sen5N[4][3] = 22; fPS_Sen5N[4][4] = 23;
  fPS_Sen5N[5][0] = 3; fPS_Sen5N[5][1] = 4; fPS_Sen5N[5][2] = 22; fPS_Sen5N[5][3] = 23;
  fPS_Sen5N[6][0] = 0; fPS_Sen5N[6][1] = 9; fPS_Sen5N[6][2] = 8; fPS_Sen5N[6][3] = 7;
  fPS_Sen5N[7][0] = 6; fPS_Sen5N[7][1] = 0; fPS_Sen5N[7][2] = 1; fPS_Sen5N[7][3] = 8; fPS_Sen5N[7][4] = 2;
  fPS_Sen5N[8][0] = 6; fPS_Sen5N[8][1] = 0; fPS_Sen5N[8][2] = 9; fPS_Sen5N[8][3] = 7; fPS_Sen5N[8][4] = 10;
  fPS_Sen5N[9][0] = 6; fPS_Sen5N[9][1] = 8; fPS_Sen5N[9][2] = 11; fPS_Sen5N[9][3] = 10;
  fPS_Sen5N[10][0] = 9; fPS_Sen5N[10][1] = 8; fPS_Sen5N[10][2] = 11; fPS_Sen5N[10][3] = 17; fPS_Sen5N[10][4] = 16;
  fPS_Sen5N[11][0] = 9; fPS_Sen5N[11][1] = 10; fPS_Sen5N[11][2] = 17; fPS_Sen5N[11][3] = 16;
  fPS_Sen5N[12][0] = 14; fPS_Sen5N[12][1] = 19; fPS_Sen5N[12][2] = 20; fPS_Sen5N[12][3] = 13; fPS_Sen5N[12][4] = 18;
  fPS_Sen5N[13][0] = 15; fPS_Sen5N[13][1] = 14; fPS_Sen5N[13][2] = 19; fPS_Sen5N[13][3] = 12;
  fPS_Sen5N[14][0] = 16; fPS_Sen5N[14][1] = 15; fPS_Sen5N[14][2] = 19; fPS_Sen5N[14][3] = 13; fPS_Sen5N[14][4] = 12;
  fPS_Sen5N[15][0] = 17; fPS_Sen5N[15][1] = 16; fPS_Sen5N[15][2] = 14; fPS_Sen5N[15][3] = 13;
  fPS_Sen5N[16][0] = 11; fPS_Sen5N[16][1] = 10; fPS_Sen5N[16][2] = 17; fPS_Sen5N[16][3] = 15; fPS_Sen5N[16][4] = 14;
  fPS_Sen5N[17][0] = 11; fPS_Sen5N[17][1] = 10; fPS_Sen5N[17][2] = 16; fPS_Sen5N[17][3] = 15;
  fPS_Sen5N[18][0] = 19; fPS_Sen5N[18][1] = 20; fPS_Sen5N[18][2] = 21; fPS_Sen5N[18][3] = 12;
  fPS_Sen5N[19][0] = 14; fPS_Sen5N[19][1] = 20; fPS_Sen5N[19][2] = 13; fPS_Sen5N[19][3] = 12; fPS_Sen5N[19][4] = 18;
  fPS_Sen5N[20][0] = 22; fPS_Sen5N[20][1] = 19; fPS_Sen5N[20][2] = 21; fPS_Sen5N[20][3] = 12; fPS_Sen5N[20][4] = 18;
  fPS_Sen5N[21][0] = 22; fPS_Sen5N[21][1] = 23; fPS_Sen5N[21][2] = 20; fPS_Sen5N[21][3] = 18;
  fPS_Sen5N[22][0] = 4; fPS_Sen5N[22][1] = 5; fPS_Sen5N[22][2] = 23; fPS_Sen5N[22][3] = 20; fPS_Sen5N[22][4] = 21;
  fPS_Sen5N[23][0] = 4; fPS_Sen5N[23][1] = 5; fPS_Sen5N[23][2] = 22; fPS_Sen5N[23][3] = 21;

}
//========
float mxGeometry::Reference(int k, int r) {
  if(k!=fLastIdx) UpdateVars(k);
  if(r==0) return fX;
  if(r==1) return fY;
  if(r==2) return fZ;
  if(r==3) return fLyrIdx;
  if(r==4) return fSenIdx;
  if(r==5) return fSenPktIdx;
  return -1;
}
//========
void mxGeometry::UpdateVars(int k) {
  // first 49152 correspond to key for SiW
  // next 416 correspond to chn for PbWO4
  if(k<49152) UpdateSiW(k%49152);
  else UpdatePbWO4(k-49152);
}
//========
float mxGeometry::W_X(int sidx, int arm) {
  int sen = sidx%24;
  return fW_RX[arm][sen];
}
//========
float mxGeometry::W_Y(int sidx, int arm) {
  int sen = sidx%24;
  return fW_RY[arm][sen];
}
//========
float mxGeometry::W_Z(int lyr) {
  float dz = 0.1 + fW_a2;
  float z = RZ(lyr) + dz*(lyr<8?+1:-1);
  return z;
}
//========
int mxGeometry::RefKey(int lyridx, int tb) {
  if(tb<0||tb>1) return -1;
  if(lyridx<0||lyridx>16||lyridx==8) return -1;
  int lyr = lyridx;
  if(lyr>7) lyr--;
  int arm = lyr/8;
  lyr = lyr%8;
  int pktA[8] = {2,2,3,3,0,0,1,1};
  int keyA = pktA[lyr]*3072 + (lyr%2)*1536;
  int keyB = (pktA[lyr]+4)*3072 + (lyr%2)*1536;
  if(arm==1) {
    keyA += 24576;
    keyB += 24576;
  }
  if(arm==0 && tb==0) return keyB;
  if(arm==0 && tb==1) return keyA;
  if(arm==1 && tb==0) return keyA;
  if(arm==1 && tb==1) return keyB;
  return -1;
}
//========
int mxGeometry::Adjacent_Si_0(int idx) {
  int adj = idx;
  int mpd = idx%128;
  if( mpd>3 ) {
    adj -= 4;
  } else {
    adj *= -1;
  }
  return adj;
}
//========
int mxGeometry::Adjacent_Si_1(int idx) {
  int adj = idx;
  int mpd = idx%128;
  if( mpd<124 ) {
    adj += 4;
  } else {
    adj *= -1;
  }
  return adj;
}
//========
void mxGeometry::UpdateSiW(int key) {
  int k = key;
  int pkt = (k/3072)%8;

  //====================
  // fix for fem7 south
  if((key/3072)==7) {
    int sensor = (key%3072)/128;
    if(sensor<6) {
      k = key + 128*18;
    } else if(sensor>=18) {
      k = key - 128*18;
    }
  }
  //====================

  int arm = k/24576;
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
  //float nx = fSi_RX[ref]/ (typ==0?fSi_a0:fSi_a1);
  //float ny = fSi_RY[ref]/ (typ==0?fSi_a1:fSi_a0);
  if(typ==0) {
    x -= sgnAR*sgnTB*((k%128)/4)*(fSi_a0+0.008);
    y += sgnTB*fSi_DLY[k%4]*(fSi_a1+0.008);
    //nx -= sgnAR*sgnTB*((k%128)/4);
    //ny += sgnTB*fSi_DLY[k%4];
  } else {
    y -= sgnTB*((k%128)/4)*(fSi_a0+0.008);
    x -= sgnAR*sgnTB*fSi_DLY[k%4]*(fSi_a1+0.008);
    //ny -= sgnTB*((k%128)/4);
    //nx -= sgnAR*sgnTB*fSi_DLY[k%4];
  }
  fX = x;
  fY = y;
  fZ = fSi_RZ[arm*8+lyr];
  fLyrIdx = arm*9+lyr;
  fLastIdx = k;
  fSenIdx = senlyr;
  fSenPktIdx = sen;
}
//========
float mxGeometry::RZ(int lyr) {
  if(lyr==8) {
    return fPbWO4_RZ[0];
  } else if(lyr==17) {
    return fPbWO4_RZ[1];
  } else {
    if(lyr<8) return fSi_RZ[lyr];
    else return fSi_RZ[lyr-1];
  }
}
//========
void mxGeometry::UpdatePbWO4(int k) {
  fX = fPbWO4_RX[k];
  fY = fPbWO4_RY[k];
  if(k<fPbWO4_IdxS) fZ = fPbWO4_RZ[0];
  else fZ = fPbWO4_RZ[1];
  int arm = 0;
  if(k>fPbWO4_IdxS) arm = 1;
  fLyrIdx = arm*9+8;
  fLastIdx = k;
}
//========
int mxGeometry::PbWO4_FindIdx(int arm, float x, float y) {
  int a = 0;
  int b = fPbWO4_IdxS;
  if(arm==1) {
    a=fPbWO4_IdxS;
    b=416;
  }
  float dx = fPbWO4_a0/2;
  float dy = fPbWO4_a1/2;
  int ret = -1;
  int cn =0;
  bool foundinx = false;
  for(int i=a; i!=b; ++i) {
    if( fPbWO4_RX[i]-dx < x ) continue;
    if( fPbWO4_RX[i]+dx > x ) continue;
    foundinx = true;
    if( fPbWO4_RY[i]-dy < y ) continue;
    if( fPbWO4_RY[i]+dy > y ) continue;
    ret = i;
    cn++;
  }
  if(cn>0) std::cout << "mxGeometry unexpected " << cn << std::endl;
  if(ret>-1) return ret+49152;
  if(foundinx) return -2;
  else return -1;
}
//========
int mxGeometry::PbWO4_FindClosestIdx(int arm, float x0, float y0) {
  int ret = PbWO4_FindIdx(arm,x0,y0);
  float dx = fPbWO4_a0/2;
  float dy = fPbWO4_a1/2;
  if(ret>0) return ret;
  if(ret==-2) {
    ret = PbWO4_FindIdx(arm, x0, y0-dy);
    if(ret>0) return ret;
    ret = PbWO4_FindIdx(arm, x0, y0+dy);
    if(ret>0) return ret;
  } else {
    ret = PbWO4_FindIdx(arm, x0-dx, y0-dy);
    if(ret>0) return ret;
    ret = PbWO4_FindIdx(arm, x0+dx, y0-dy);
    if(ret>0) return ret;
    ret = PbWO4_FindIdx(arm, x0-dx, y0+dy);
    if(ret>0) return ret;
    ret = PbWO4_FindIdx(arm, x0+dx, y0+dy);
    if(ret>0) return ret;
  }
  return -3;
}
//========
void mxGeometry::PbWO4_GetNeighbours(int idx, int en[4]) {
  en[0] = fPbWO4_4N[idx-49152][0];
  en[1] = fPbWO4_4N[idx-49152][1];
  en[2] = fPbWO4_4N[idx-49152][2];
  en[3] = fPbWO4_4N[idx-49152][3];
  for(int i=0; i!=4; ++i)
    if(en[i]!=-1) en[i] += 49152;
}
//========
void mxGeometry::PS_GetSensorNeighbours(int sidx, int nei[5]) {
  nei[0] = fPS_Sen5N[sidx][0];
  nei[1] = fPS_Sen5N[sidx][1];
  nei[2] = fPS_Sen5N[sidx][2];
  nei[3] = fPS_Sen5N[sidx][3];
  nei[4] = fPS_Sen5N[sidx][4];
}
//========
int mxGeometry::PS_SenRing(int sidx) {
  if(sidx==2) return 0;
  if(sidx==4) return 0;
  if(sidx==7) return 0;
  if(sidx==8) return 0;
  if(sidx==10) return 0;
  if(sidx==14) return 0;
  if(sidx==16) return 0;
  if(sidx==19) return 0;
  if(sidx==20) return 0;
  if(sidx==22) return 0;
  return 1;
}
