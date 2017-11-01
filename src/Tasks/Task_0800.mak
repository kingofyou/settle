include $(BATCHHOME)/mak/platform.mak
include $(BATCHHOME)/mak/database.mak
include $(BATCHHOME)/mak/tuxedo.mak

PRGOBJS = \
    Task_0800.o
 
PRGTARG = BTask_0800
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
