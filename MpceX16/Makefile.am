AUTOMAKE_OPTIONS = foreign

AM_CXXFLAGS = -Wall -Werror

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMXCphnx.la \
  libMXRphnx.la

noinst_HEADERS = \
  MXCphnxLinkDef.h \
  MXRphnxLinkDef.h

include_HEADERS = \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibBasePbWO4.h \
  mxGeometry.h \
  mxHit.h \
  mxCluster.h \
  mxCandidate.h \
  mxCandidateCuts.h \
  mxDecayCandidate.h \
  mxData.h \
  mxDisplay.h \
  mxClusterer.h \
  mxClustererMPC.h \
  mxClustererEX.h \
  mMXRinit.h \
  PHMXData.h

libMXCphnx_la_SOURCES = \
  mxCalibMaster.cxx \
  mxCalibBaseSiW.cxx \
  mxCalibBasePbWO4.cxx \
  MXCphnx_Dict.C

libMXRphnx_la_SOURCES = \
  mxGeometry.cxx \
  mxHit.cxx \
  mxCluster.cxx \
  mxCandidate.cxx \
  mxCandidateCuts.cxx \
  mxDecayCandidate.cxx \
  mxData.cxx \
  mxDisplay.cxx \
  mxClusterer.cxx \
  mxClustererMPC.cxx \
  mxClustererEX.cxx \
  mMXRinit.cxx \
  MXRphnx_Dict.C

libMXCphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libMXRphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lmpcex_base \
  -lmpcex_interface \
  -lmpcex_modules \
  -lMpcExReco

# Rule for generating CINT dictionaries from class headers.
MXCphnx_Dict.C: \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibBasePbWO4.h \
  MXCphnxLinkDef.h
        rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

MXRphnx_Dict.C: \
  mxGeometry.h \
  mxHit.h \
  mxCluster.h \
  mxCandidate.h \
  mxCandidateCuts.h \
  mxDecayCandidate.h \
  mxData.h \
  mxDisplay.h \
  mxClusterer.h \
  mxClustererMPC.h \
  mxClustererEX.h \
  mMXRinit.h \
  PHMXData.h \
  MXRphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*


# LocalWords:  AUTOMAKE
