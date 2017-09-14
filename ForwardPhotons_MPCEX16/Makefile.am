AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMXphnx.la

noinst_HEADERS = \
  MXphnxLinkDef.h

include_HEADERS = \
  mSubsysReco.h \
  mMPCTunner.h \
  mMPCEXCounter.h \
  mMPCEXQ.h

libMXphnx_la_SOURCES = \
  mSubsysReco.cxx \
  mMPCTunner.cxx \
  mMPCEXCounter.cxx \
  mMPCEXQ.cxx \
  MXphnx_Dict.C

libMXphnx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lmpcex_base \
  -lmpcex_interface \
  -lmpcex_modules \
  -lMXCphnx \
  -lMXRphnx \
  -lQCphnx \
  -lMpcExReco

# Rule for generating CINT dictionaries from class headers.
MXphnx_Dict.C: \
  mSubsysReco.h \
  mMPCTunner.h \
  mMPCEXCounter.h \
  mMPCEXQ.h \
  MXphnxLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

