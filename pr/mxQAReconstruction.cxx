//#include <stdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "mxReconstruction.h"
#include "mxQAReconstruction.h"

//========
mxQAReconstruction::mxQAReconstruction() {
  // ctor
  std::cout << "mxQAReconstruction:: ctor" << std::endl;
}
//========
mxQAReconstruction::~mxQAReconstruction() {
  // dtor
}
//========
void mxQAReconstruction::Make(mxReconstruction *r) {
  // filler
  FillEvents(r);
  FillHits(r);
  FillParties(r);
  FillCoalitions(r);
  FillUnions(r);
}
//========
void mxQAReconstruction::FillEvents(mxReconstruction *r) {
  // filler
}
//========
void mxQAReconstruction::FillHits(mxReconstruction *r) {
  // filler
}
//========
void mxQAReconstruction::FillParties(mxReconstruction *r) {
  // filler
}
//========
void mxQAReconstruction::FillCoalitions(mxReconstruction *r) {
  // filler
}
//========
void mxQAReconstruction::FillUnions(mxReconstruction *r) {
  // filler
}
