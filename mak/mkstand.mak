#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME    : mksstand.mak                                                |
#| DESCRIPTIONS : Makefile for all kind of platform, for c and pc file,       |
#|                for lib(dynamic & static) and execuatable object            |
#|----------------------------------------------------------------------------|

include $(BATCHHOME)/mak/platform.mak
include $(BATCHHOME)/mak/database.mak
include $(BATCHHOME)/mak/tuxedo.mak

# environment variables must be define outside of makefile
#
# $BATCHHOME : home directory of application 
# $CCFLG  : compiler options for c source code
# $CXXFLG : compiler options for c++ source code
# $DLFLG  : link options for dynamic library
# $SLFLG  : link options for static library
# $EXFLG  : link options for excecuteable object
# $DBFLG  : debug mode options
# $OPFLG  : optimize mode options
# $DLFIX  : suffix of dynamic library
# $SLFIX  : suffix of static library

# $OBJS   : c object files list
# $LIBS   : libs required for executable object
# $DEFS   : define flag list
# $TARGET : target objetc name

# $TUXSVCS: tuxedo services name
# $TUXCLTTARG: tuxedo client name
# $TUXRSMN: tuxedo resource manager name

# $DBNM   : db name (for db2)

# commands
 
PC		=	proc
EC		=	esql
DB2		=	db2

JAVAC   =   javac
JAVAH   =   javah
RMIC    =   rmic

RM		=	rm -f
CP		=	cp
MV		=   mv
CD		=   cd
ECHO	=	@echo

# paths and flags

SYSINCP	=	/usr/include
USRINCP	=	/usr/include/libxml2
SYSLIBP	=	/usr/lib	
JINCP	=	$(JAVA_HOME)/include
JMDINCP	=	$(JAVA_HOME)/include/$(JAVAMD)

SRCP	=	$(BATCHHOME)/src
BINP	= 	$(BATCHHOME)/bin
#TUXBINP =	$(BATCHHOME)/online/bin
#CLIBINP =	$(BATCHHOME)/online/bin
LIBP	=	$(BATCHHOME)/lib
INCP	=	$(BATCHHOME)/include
CLSP    =   $(BINP)/classes
JLIBP   =   $(JAVA_HOME)/lib
BNDDIR  =   $(HOME)/bnd

INCPS	=   -I$(SYSINCP) -I$(USRINCP) -I$(INCP) -I$(DBINCP) -I$(TUXINCP) -I$(JINCP) -I$(JMDINCP)
LIBPS	=   -L$(SYSLIBP) -L$(XLIBP) -L$(LIBP) $(DBLIBP) -L$(TUXLIBP)

CCFLGS	=   $(RLFLG) $(CCFLG) $(DEFS) $(TUXDEF) $(DBDEF) $(OSDEF) $(BITDEF) $(INCPS) $(REDDAT64_LIBS)
CXXFLGS =   $(RLFLG) $(CXXFLG) $(DEFS) $(TUXDEF) $(DBDEF) $(OSDEF) $(BITDEF) $(INCPS)
DLFLGS	=   $(DLFLG) $(LIBPS)
EXFLGS	=   $(EXFLG) $(LIBPS)
 
DLTARG	= $(LIBP)/lib$(TARGET)$(DLFIX)
SLTARG	= $(LIBP)/lib$(TARGET)$(SLFIX)
EXTARG	= $(BINP)/$(TARGET)
#EXTARG	= $(TARGET)
TUXTARG	= $(TUXBINP)/$(TARGET)
#TUXCLTTARG	= $(CLIBINP)/$(TARGET)

debugdynamic: setdebug $(DLTARG)
releasedynamic: setrelease $(DLTARG)
debugstatic: setdebug $(SLTARG)
releasestatic: setrelease $(SLTARG)
debugexec: setdebug $(EXTARG)
releaseexec : setrelease $(EXTARG)
debugtuxsvrexec: setdebug $(TUXTARG)
releasetuxsvrexec : setrelease $(TUXTARG)
debugtuxcltexec: setdebug $(TUXCLTTARG)
releasetuxcltexec : setrelease $(TUXCLTTARG)

javaclasses : $(OBJS)
	$(ECHO) "Finish java package [$(TARGET)]"
rmi :
	$(ECHO) "rmic [$(TARGET)] ..."
	@$(CD) `dirname $(TARGET)`; \
	$(RMIC) `basename $(TARGET)`

setdebug:
	$(ECHO) $(DBFLG) > $(BATCHHOME)/mak/.mak.tmp

setrelease:
	$(ECHO) $(OPFLG) > $(BATCHHOME)/mak/.mak.tmp

$(SLTARG): $(OBJS) 
	$(ECHO) "Linking   [$(SLTARG)] ..."
	@$(RM) $@
	@$(SLINK) $(SLFLG) $@ $(OBJS)
	@$(RM) $(BATCHHOME)/mak/.mak.tmp

$(DLTARG): $(OBJS) 
	$(ECHO) "Linking   [$(DLTARG)] ..."
	@if [ $(OSTYPE) = aix ]; \
	then \
		$(DLINK) -o $@ $(DLFLGS) $(OBJS) $(LIBS) $(UXLIBS); \
	else \
		$(DLINK) -o `basename $@` $(DLFLGS) $(OBJS) $(LIBS); \
		$(MV) `basename $@` `dirname $@`; \
	fi
	@$(RM) $(BATCHHOME)/mak/.mak.tmp

$(EXTARG): $(OBJS)
	$(ECHO) "Linking   [$(EXTARG)] ..."
	$(CC) -o $@ $(EXFLGS) $(OBJS) $(LIBS)
	$(RM) $(BATCHHOME)/mak/.mak.tmp

$(TUXTARG): $(OBJS)
	$(ECHO) "buildserver [$(TUXTARG)] ..."
	TUXDIR="$(TUXDIR)" \
	CFLAGS="$(EXFLGS)" \
	$(TUXBS) -v -t -o $@  -f "$(TUXLIBPS)"  -f "$(OBJS)" -v $(TUXSVCS) -f "$(LIBS)"
#	$(TUXBS) -v -t -o $@  -r ${TUXRM}  -f "$(OBJS)" -v $(TUXSVCS) -f "$(LIBS)"
#	$(TUXBS) -v -o $@   -f $(OBJS) -v $(TUXSVCS)   $(LIBS)
#	$(TUXBS) -v -o $@ -r ${TUXRM} -f "$(OBJS)" -v $(TUXSVCS) -f "$(LIBS)"
#	$(TUXBS) -o $@  -f "$(OBJS)" -v $(TUXSVCS) -f "$(LIBS)"
# -r $(TUXRSMN)
# -f " $(INCPS) "

$(TUXCLTTARG): $(OBJS)
	$(ECHO) "buildclient [$(TUXCLTTARG)] ..."
	$(TUXBC)  -o $@ -f "$(OBJS)" -f "$(EXFLGS)" -f "$(LIBS)
#	$(TUXBC) -w -o $@ -r ${TUXRM} -f "$(OBJS)" -f "$(EXFLGS)" -f "$(LIBS)

# implicit
 
.SUFFIXES:
.SUFFIXES: .java .class .xc .ec .pc .sqc .c .o
 
.xc.o:
	$(ECHO) "Compiling [$@] ..."
	@$(RM) $*$(DBFIX)
	@$(CP) $*.xc $*$(DBFIX)
	@if [ $(DBTYPE) = ora ]; \
	then \
		$(PC) define=$(DBD) include=$(TUXINCP) include=$(INCP) include=$(JINCP) include=$(JMDINCP) code=ANSI_C iname=$*$(DBFIX); \
		$(CC) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CCFLGS) -c $*.c; \
		$(RM) $*.lis; \
	else \
		$(EC) -e -ED$(DBD) $(INCPS) $*$(DBFIX); \
		$(CC) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c; \
		$(RM) `basename $*`.c; \
	fi
	@$(RM) $*$(DBFIX) $*.c

.pc.o:
	$(ECHO) "Compiling [$@] ..."
#	@$(PC) lines=yes define=$(DBD) include=$(TUXINCP) include=$(INCP) include=$(DBINCP) code=ANSI_C iname=$*$(DBFIX)
#	@$(PC) lines=yes define=$(DBD) include=$(INCP) include=$(DBINCP) include=$(TUXINCP) code=ANSI_C iname=$*$(DBFIX)
	@$(PC)  parse=no lines=yes define=$(DBD) include=$(TUXINCP) include=$(INCP) include=/usr/include include=`dirname $@` code=ANSI_C iname=$*$(DBFIX)
	@$(CC) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CCFLGS) -c -fPIC $*.c
	@$(RM) $*.lis $*.c tp*

.ec.o:
	$(ECHO) "Compiling [$@] ..."
	$(EC) -e -ED$(DBD) $(INCPS) $*$(DBFIX); \
	$(CC) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CCFLGS) -c -fPIC `basename $*`.c; \
	$(RM) `basename $*`.c; \

.sqc.o:
	$(ECHO) "Compiling [$@] ..."
	@$(RM) $*.bnd
	@$(DB2) connect to $(DBNM)
#	@$(DB2) prep `basename $<` bindfile
#	@$(DB2) bind `basename $*`.bnd
#	@$(DB2) prep $< bindfile
	-$(DB2) prep $< bindfile USING $(BNDDIR)/$*.bnd VERSION LDCARD
#	$(DB2) bind $(BNDDIR)/$*.bnd DATETIME iso dynamicrules bind SQLWARN no
	$(DB2) bind $(BNDDIR)/$*.bnd DATETIME iso dynamicrules bind 
	@$(DB2) connect reset
	@$(DB2) terminate
	$(CD) `dirname $@`; \
        $(CC) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CCFLGS) -c `basename $*`.c
#	@$(RM) $*.bnd $*.c
#	@$(RM) $*.c
	
.cpp.o:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@` ; \
	$(CXX) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CXXFLGS) -c `basename $<`

.c.o:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@`; \
	$(CC) -o $@ `cat $(BATCHHOME)/mak/.mak.tmp` $(CCFLGS) -c -fPIC `basename $<`
 
.java.class:
	$(ECHO) "Compiling [$@] ..."
	@$(CD) `dirname $@`; \
	$(JAVAC) `basename $<`

# clearing object codes
 
clean: cleanup
	@$(RM) $(SLTARG) $(DLTARG) $(EXTARG) $(TUXTARG)
 
cleanup:
	@$(RM) $(OBJS)
