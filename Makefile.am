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
  mxUnion.h \
  mxReconstruction.h \
  mxQAReconstruction.h \
  \
  mxCalibDAu16.h \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  PHMXData.h \
  mSubsysReco.h

libMXphnx_la_SOURCES = \
  mxGeometry.cxx \
  mxHit.cxx \
  mxParty.cxx \
  mxCoalition.cxx \
  mxUnion.cxx \
  mxReconstruction.cxx \
  mxQAReconstruction.cxx \
  \
  mxCalibDAu16.cxx \
  mxCalibMaster.cxx \
  mxCalibBaseSiW.cxx \
  mSubsysReco.cxx \
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
  mxUnion.h \
  mxReconstruction.h \
  mxQAReconstruction.h \
  \
  mxCalibDAu16.h \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  PHMXData.h \
  mSubsysReco.h \
  MXphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

