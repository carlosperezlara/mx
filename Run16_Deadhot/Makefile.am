AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

SUBDIRS = 

lib_LTLIBRARIES = \
  libMyMpcEx.la 

noinst_HEADERS = \
  MyMpcExLinkDef.h 

include_HEADERS = \
  mMpcExDeadHot.h
 

libMyMpcEx_la_SOURCES = \
  mMpcExDeadHot.C \
  MyMpcEx_Dict.C

libMyMpcEx_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -luspin \
  -lmpcex_base \
  -lmpcex_interface \
  -lmpcex_modules \
  -lmpcex_utils \
  -lMpcExReco

# Rule for generating CINT dictionaries from class headers.
MyMpcEx_Dict.C: \
  mMpcExDeadHot.h \
  MyMpcExLinkDef.h 
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

