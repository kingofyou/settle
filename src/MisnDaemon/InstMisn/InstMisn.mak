#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.exec.mak                                             |
#| DESCRIPTIONS : Sample makefile for executable binary                       |
#|----------------------------------------------------------------------------|

include $(BATCHHOME)/mak/platform.mak
include $(BATCHHOME)/mak/database.mak
include $(BATCHHOME)/mak/tuxedo.mak

PRGOBJS = \
	$(BATCHHOME)/src/MisnDaemon/InstMisn/InstMisn.o \

PRGTARG = InstMisn
#PRGLIBS = -lBatCommon -lBatDbs -lBatTask -lclntsh -lclient9 $(NETLIBS) $(UXLIBS)
PRGLIBS = \
		-lBPlat -lBCommon -lBDbs -lBConvert $(DBLIBS) $(NETLIBS) $(UXLIBS)
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugexec
release: releaseexec

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(BATCHHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
