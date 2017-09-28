// origin: S.Karthas (Aug 2016)

#include "mxg4ActionInitialization.h"
#include "mxg4PrimaryGeneratorAction.h"
#include "mxg4RunAction.h"
#include "mxg4EventAction.h"

mxg4ActionInitialization::mxg4ActionInitialization()
 : G4VUserActionInitialization() {
}

mxg4ActionInitialization::~mxg4ActionInitialization() {
}

void mxg4ActionInitialization::BuildForMaster() const {
  SetUserAction(new mxg4RunAction);
}

void mxg4ActionInitialization::Build() const {
  SetUserAction(new mxg4PrimaryGeneratorAction);
  SetUserAction(new mxg4RunAction);
  SetUserAction(new mxg4EventAction);
}  
