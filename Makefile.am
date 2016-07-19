AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMXphnx.la

noinst_HEADERS = \
  MXphnxLinkDef.h

include_HEADERS = \
  ../pr/mxGeometry.h \
  ../pr/mxHit.h \
  ../pr/mxParty.h \
  ../pr/mxCoalition.h \
  ../pr/mxReconstruction.h \
  \
  ../calibration/mxCalibDAu16.h \
  ../calibration/mxCalibMaster.h \
  ../calibration/mxCalibBase.h \
  ../calibration/mxChipStatus.h \
  mxSubsysReco.h \
  srTest.h

libMXphnx_la_SOURCES = \
  ../pr/mxGeometry.cxx \
  ../pr/mxHit.cxx \
  ../pr/mxParty.cxx \
  ../pr/mxCoalition.cxx \
  ../pr/mxReconstruction.cxx \
  \
  ../calibration/mxCalibDAu16.cxx \
  ../calibration/mxCalibMaster.cxx \
  ../calibration/mxCalibBase.cxx \
  ../calibration/mxChipStatus.cxx \
  mxSubsysReco.cxx \
  srTest.cxx \
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
  ../pr/mxHit.h \
  ../pr/mxParty.h \
  ../pr/mxCoalition.h \
  ../pr/mxGeometry.h \
  ../pr/mxReconstruction.h \
  \
  ../calibration/mxCalibDAu16.h \
  ../calibration/mxCalibMaster.h \
  ../calibration/mxCalibBase.h \
  ../calibration/mxChipStatus.h \
  mxSubsysReco.h \
  srTest.h \
  MXphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

