#include <cstdlib>

#include "mxHit.h"
#include "mxParty.h"
#include "mxCoalition.h"

int main() {
  mxHit *hit = new mxHit();
  mxParty *pty = new mxParty();
  mxCoalition *coa = new mxCoalition(0);
  return 0;
}
