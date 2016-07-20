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
  calibration/mxCalibDAu16.h \
  calibration/mxCalibMaster.h \
  calibration/mxCalibBase.h \
  calibration/mxChipStatus.h \
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
  calibration/mxCalibDAu16.cxx \
  calibration/mxCalibMaster.cxx \
  calibration/mxCalibBase.cxx \
  calibration/mxChipStatus.cxx \
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
  mxHit.h \
  mxParty.h \
  mxCoalition.h \
  mxGeometry.h \
  mxReconstruction.h \
  mxQAReconstruction.h \
  \
  calibration/mxCalibDAu16.h \
  calibration/mxCalibMaster.h \
  calibration/mxCalibBase.h \
  calibration/mxChipStatus.h \
  mxHit.h \
  mxParty.h \
  mxCoalition.h \
  mxUnion.h \
  PHMXData.h \
  mSubsysReco.h \
  MXphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

