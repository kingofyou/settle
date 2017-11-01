#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.scoos.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = g++
DLINK = ld
SLINK = ar
DLFLG = -belf -G
SLFLG = ruc
CCFLG = -Kpic -belf 
CXXFLG= -Wall
EXFLG =
DBFLG = -g
OPFLG = -O3
DLFIX = .so
SLFIX = .a
UXLIBS = -lm -lc
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl_s
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lgthread
XLIBS   = -lX11 -lXext
XLIBP   = /usr/X11R6/lib
OSDEF = -DSCO_UNIX
OSTYPE = scoos
