#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.aix32.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = xlC
DLINK = ld
SLINK = ar
DLFLG = -G -bE:$(BATCHHOME)/mak/exp/lib$(TARGET).exp -bnoentry
SLFLG = ruc
CCFLG = -qchars=signed
CXXFLG= -qchars=signed
EXFLG = -brtl
DBFLG = -g
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lm -lc
DLLIBS = -ldl
NETLIBS =
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthreads
XLIBS   = -lXext -lX11
XLIBP   = .
OSDEF = -DAIX_UNIX
OSTYPE = aix
JAVAMD = aix
