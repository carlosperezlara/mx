#include "../mxGeometry.h"
#include "../mxGeometry.cxx"

void gen_geantgeo() {
  ofstream ofile("mpcex.g4bl");
  mxGeometry *mgeo = new mxGeometry();
  ofile << Form("physics FTFP_BERT\n");
  float dx, dy, dz;
  dx = mgeo->W_a0(); dy = mgeo->W_a1(); dz = mgeo->W_a2();
  ofile << Form("box tungs width=%0.2f height=%0.2f length=%0.2f material=W color=1,1,0\n", dx*10, dy*10, dz*10);
  ofile << Form("virtualdetector vd width=%0.2f height=%0.2f length=0.1 material=W color=1,0,0\n", dx*10, dy*10);
  dx = mgeo->Si_a0(); dy = mgeo->Si_a1(); dz = mgeo->Si_a2();
  ofile << Form("detector MPX width=%0.2f height=%0.2f length=%0.2f material=Si color=0,0,1\n", dx*10, dy*10, dz*10);
  ofile << Form("detector MPY width=%0.2f height=%0.2f length=%0.2f material=Si color=0,0,1\n", dy*10, dx*10, dz*10);
  dx = mgeo->PWO4_a0(); dy = mgeo->PWO4_a1(); dz = mgeo->PWO4_a2();
  ofile << Form("detector MPC width=%0.2f height=%0.2f length=%0.2f material=PbWO4 color=0,0,1\n", dx*10, dy*10, dz*10);
  //for(int idx=0; idx!=384; ++idx) {
  //  for(int idx=0+24; idx!=0+48; ++idx) {
  for(int idx=0; idx!=192+192; ++idx) { // all north
    //for(int idx=192+0; idx!=192+192; ++idx) { // all north
    float senx = mgeo->W_X(idx);
    float seny = mgeo->W_Y(idx);
    float senz = mgeo->W_Z(idx);
    if(idx<192) { // 'front' places object displaced by length in the negative side
      senz -= mgeo->W_a2();
    }
    ofile << Form("place tungs rename=W%03d z=%0.2f x=%0.2f y=%0.2f front=1\n",idx,senz*10,senx*10,seny*10);
    ofile << Form("place vd rename=VD%03d z=%0.2f x=%0.2f y=%0.2f front=1\n",idx,senz*10-0.2,senx*10,seny*10);
  }
  for(int idx=0; idx!=288+288; ++idx) { // all north
    //for(int idx=288+0; idx!=288+288; ++idx) { // all north
    int k = idx+49152;
    float mpcx = mgeo->X(k);
    float mpcy = mgeo->Y(k);
    float mpcz = mgeo->Z(k);
    if(idx<288) { // 'front' places object displaced by length in the negative side
      mpcz -= mgeo->PWO4_a2();
    }
    if(mpcz<50&&mpcz>-50) continue; //skip holes
    ofile << Form("place MPC rename=PWO4%03d z=%0.2f x=%0.2f y=%0.2f front=1\n",k,mpcz*10,mpcx*10,mpcy*10);
  }
  if(0) {
    //for(int k=0; k!=49152; ++k) {
    for(int k=24576; k!=49152; ++k) {
      float x = mgeo->X( k );
      float y = mgeo->Y( k );
      float z = mgeo->Z( k );
      if(k<24576) { // 'front' places object displaced by length in the negative side
	z -= mgeo->Si_a2();
      }
      //if(mgeo->LyrIdx(k)!=9+1) continue;
      if((mgeo->LyrIdx(k)%9)%2==0) {
	ofile << Form("place MPX rename=Si%05d x=%0.2f y=%0.2f z=%0.2f front=1\n", k, x*10, y*10, z*10);
      } else {
	ofile << Form("place MPY rename=Si%05d x=%0.2f y=%0.2f z=%0.2f front=1\n", k, x*10, y*10, z*10);
      }
    }
  }
  ofile << Form("geometry nPoints=0\n"); // ommit geometry test
  ofile << "beam gaussian beamZ=0 meanYp=120./2100. sigmaYp=0.001 meanXp=0 sigmaXp=0.001 meanMomentum=20000 particle=pi0 nEvents=100";
  // 127 / 2100
  ofile.close();
}
