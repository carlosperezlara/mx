//=========================
// written by Carlos Perez 
//=========================
#include "mxGeometry.h"
#include "mxData.h"
#include "mxCluster.h"
#include "mxClusterer.h"

mxClusterer::mxClusterer() {
  fGeo = mxGeometry::Instance();
  fData = NULL;
  fParty = new mxCluster();
}
mxClusterer::~mxClusterer() {
  delete fParty;
}
