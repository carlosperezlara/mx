AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMXphnx.la

noinst_HEADERS = \
  MXphnxLinkDef.h

include_HEADERS = \
  mxGeometry.h \
  mxHit.h \
  mxParty.h \
  mxCoalition.h \
  mxCoalitionCuts.h \
  mxUnion.h \
  mxReconstruction.h \
  mxQAReconstruction.h \
  mxEventCuts.h \
  \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibBasePbWO4.h \
  PHMXData.h \
  mSubsysReco.h \
  mMPCTunner.h

libMXphnx_la_SOURCES = \
  mxGeometry.cxx \
  mxHit.cxx \
  mxParty.cxx \
  mxCoalition.cxx \
  mxCoalitionCuts.cxx \
  mxUnion.cxx \
  mxReconstruction.cxx \
  mxQAReconstruction.cxx \
  mxEventCuts.cxx \
  \
  mxCalibMaster.cxx \
  mxCalibBaseSiW.cxx \
  mxCalibBasePbWO4.cxx \
  mSubsysReco.cxx \
  mMPCTunner.cxx \
  MXphnx_Dict.C

libMXphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lmpcex_base \
  -lmpcex_interface \
  -lmpcex_modules \
  -lMpcExReco

# Rule for generating CINT dictionaries from class headers.
MXphnx_Dict.C: \
  mxGeometry.h \
  mxHit.h \
  mxParty.h \
  mxCoalition.h \
  mxCoalitionCuts.h \
  mxUnion.h \
  mxReconstruction.h \
  mxQAReconstruction.h \
  mxEventCuts.h \
  \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibBasePbWO4.h \
  PHMXData.h \
  mSubsysReco.h \
  mMPCTunner.h \
  MXphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

