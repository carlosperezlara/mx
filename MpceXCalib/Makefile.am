AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMXCphnx.la

noinst_HEADERS = \
  MXCphnxLinkDef.h

include_HEADERS = \
  mxSafetyCuts.h \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibBasePbWO4.h

libMXCphnx_la_SOURCES = \
  mxSafetyCuts.cxx \
  mxCalibMaster.cxx \
  mxCalibBaseSiW.cxx \
  mxCalibBasePbWO4.cxx \
  MXCphnx_Dict.C

libMXCphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lmpcex_base \
  -lmpcex_interface \
  -lmpcex_modules \
  -lMpcExReco

# Rule for generating CINT dictionaries from class headers.
MXCphnx_Dict.C: \
  mxSafetyCuts.h \
  mxCalibMaster.h \
  mxCalibBaseSiW.h \
  mxCalibBasePbWO4.h \
  MXCphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

