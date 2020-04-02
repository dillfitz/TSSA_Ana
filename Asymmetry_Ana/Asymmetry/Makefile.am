AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

lib_LTLIBRARIES = libppAsymmetry.la

libppAsymmetry_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib 

noinst_HEADERS = \
  AsymmetryLinkDef.h \
  PolarizationLinkDef.h \
  RelativeLuminosityLinkDef.h \
  AcceptanceCorrectionLinkDef.h

include_HEADERS = \
  Constants.h \
  Asymmetry.h \
  Polarization.h \
  RelativeLuminosity.h \
  AcceptanceCorrection.h

libppAsymmetry_la_SOURCES = \
  Asymmetry.C \
  Asymmetry_Dict.C \
  Polarization.C \
  Polarization_Dict.C \
  RelativeLuminosity.C \
  RelativeLuminosity_Dict.C \
  AcceptanceCorrection.C \
  AcceptanceCorrection_Dict.C

# Rule for generating CINT dictionaries from class headers.
%_Dict.C: %.h %LinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

