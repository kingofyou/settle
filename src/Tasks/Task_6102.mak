#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : sample.dynamiclib.mak                                       |
#| DESCRIPTIONS : Sample makefile for dynamic library                         |
#|----------------------------------------------------------------------------|

include $(BATCHHOME)/mak/platform.mak
include $(BATCHHOME)/mak/database.mak
include $(BATCHHOME)/mak/tuxedo.mak

PRGOBJS = \
    Task_6102.o
 
PRGTARG = BTask_6102
#PRGLIBS = -lBCommon -lBDbs -lBConvert $(DBLIBS) $(NETLIBS) $(UXLIBS)
PRGLIBS =-lBPlat -lBCommon -lBDbs -lBConvert $(DBLIBS) $(NETLIBS) $(UXLIBS)
PRGDEFS =

# used for db2 database
PRGDBNM =

debug all: debugdynamic
release: releasedynamic

# DO NOT modify any code below!!!

releasedynamic debugdynamic releasestatic debugstatic releaseexec debugexec clean:
	@make -f $(BATCHHOME)/mak/mkstand.mak $@ TARGET="$(PRGTARG)" OBJS="$(PRGOBJS)" LIBS="$(PRGLIBS)" DEFS="$(PRGDEFS)" DBNM="$(PRGDBNM)"
