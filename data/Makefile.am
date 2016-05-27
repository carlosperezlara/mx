AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMXpnhx.la

noinst_HEADERS = \
  libMXphnxLinkDef.h

include_HEADERS = \
  ../mxGeometry.h \
  ../mxHit.h \
  ../mxParty.h \
  ../mxCoalition.h \
  ../mxReconstruction.h \
  \
  ../calibration/mxCalibDAu16.h \
  ../calibration/mxCalibMaster.h \
  ../calibration/mxCalibBase.h \
  ../calibration/mxChipStatus.h \
  mxSubsysReco.h

libMXphnx_la_SOURCES = \
  ../mxGeometry.cxx \
  ../mxHit.cxx \
  ../mxParty.cxx \
  ../mxCoalition.cxx \
  ../mxReconstruction.cxx \
  \
  ../calibration/mxCalibDAu16.cxx \
  ../calibration/mxCalibMaster.cxx \
  ../calibration/mxCalibBase.cxx \
  ../calibration/mxChipStatus.cxx \
  mxSubsysReco.cxx \
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
  ../mxHit.h \
  ../mxParty.h \
  ../mxCoalition.h \
  ../mxGeometry.h \
  ../mxReconstruction.h \
  \
  ../calibration/mxCalibDAu16.h \
  ../calibration/mxCalibMaster.h \
  ../calibration/mxCalibBase.h \
  ../calibration/mxChipStatus.h \
  mxSubsysReco.h \
  MXphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

