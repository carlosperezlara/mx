AUTOMAKE_OPTIONS = foreign

AM_CXXFLAGS = -Wall -Werror

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

dist_pkgdata_DATA = \
Run16dAuEmcalDeadMap.txt \
dAu62_EmcalDeadmap.txt\
dAu39_EmcalDeadmap.txt\
pAu15_deadmaps.txt \
run_vs_ratiopersector.dat\
dAu62run_vs_ratiopersector.dat\
dAu39run_vs_ratiopersector.dat


SUBDIRS = 

lib_LTLIBRARIES = \
  libMXCphnx.la \
  libMXRphnx.la \
  libQCRphnx.la \
  libCARphnx.la

noinst_HEADERS = \
  MXCphnxLinkDef.h \
  MXRphnxLinkDef.h \
  QCRphnxLinkDef.h \
  CARphnxLinkDef.h

include_HEADERS = \
  EMCC.h \
  CNTE.h \
  CNTDE.h \
  mCArecoPizeroes.h \
  qcQ.h \
  qcData.h \
  PHQCData.h \
  qcCalibBase.h \
  mQCRinit.h \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibStageSiW.h \
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
  mxCalibStageSiW.cxx \
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

libQCRphnx_la_SOURCES = \
  qcCalibBase.cxx \
  qcQ.cxx \
  qcData.cxx \
  mQCRinit.cxx \
  QCRphnx_Dict.C

libCARphnx_la_SOURCES = \
  EMCC.cxx \
  CNTE.cxx \
  CNTDE.cxx \
  mCArecoPizeroes.cxx \
  CARphnx_Dict.C

libMXCphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libMXRphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lmpcex_base \
  -lmpcex_interface \
  -lmpcex_modules \
  -lMpcExReco \
  libMXCphnx.la

libQCRphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  libMXCphnx.la \
  libMXRphnx.la

libCARphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lSubsysReco \
  -luti \
  -lHardpDST \
  -lCNT \
  -lcgl \
  -lpad \
  -lemc \
  -llpc \
  -lTHmul \
  -lboost_thread \
  -lboost_date_time \
  -lTOAD \
  -lFROG \
  -lfun4allfuncs \
  libMXCphnx.la \
  libMXRphnx.la \
  libQCRphnx.la

# Rule for generating CINT dictionaries from class headers.
QCRphnx_Dict.C: \
  qcCalibBase.h \
  qcQ.h \
  qcData.h \
  PHQCData.h \
  mQCRinit.h \
  QCRphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

MXCphnx_Dict.C: \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibStageSiW.h \
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

CARphnx_Dict.C: \
  EMCC.h \
  CNTE.h \
  CNTDE.h \
  mCArecoPizeroes.h \
  CARphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^


clean-local:
	rm -f *Dict*


# LocalWords:  AUTOMAKE
