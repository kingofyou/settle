#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.rhlux.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = g++
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG = -fpic
CXXFLG= -Wall
EXFLG =
DBFLG = -g
OPFLG = -O3
DLFIX = .so
SLFIX = .a
UXLIBS = -lm -lc
DLLIBS = -ldl
NETLIBS = 
CURLIBS = -lcurses
CRYLIBS = -lcrypt
THREADLIBS = -lpthread
X11LIBS = -lXext -lX11
X11SMLIBS= -lICE -lSM
XLIBS   = $(X11SMLIBS) $(DLLIBS) $(X11LIBS)
XLIBP   = /usr/X11R6/lib
OSDEF = -DREDHAT_LINUX
OSTYPE = rhlux
