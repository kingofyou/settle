#|----------------------------------------------------------------------------|
#|                           TOPMake 2.0                                      |
#| Copyright (c) 2000-2004 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.aix32.mak                                       |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = xlC
DLINK = ld
SLINK = ar
#DLFLG = -b64 -G -bE:$(ONLINE_HOME)/mak/exp/lib$(TARGET).exp -bnoentry
DLFLG = -G  
SLFLG = -ruc
CCFLG = -g
CXXFLG= 
EXFLG = 
DBFLG = -g
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lm -lc 
#UXLIBS = -lm -lc -liconv
DLLIBS = -ldl
NETLIBS =
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthreads
XLIBS   = -lXext -lX11
XLIBP   = /usr/X11R6/lib
OSDEF = -DREDHAT_LINUX
BITDEF = -DBIT64
#OSTYPE = aix
#JAVAMD = aix
OSTYPE = rhlux
#JAVAMD = rhlux 
REDDAT64_LIBS = -fPIC
