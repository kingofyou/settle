#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.aix64.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = xlC
DLINK = ld
SLINK = ar
DLFLG = -b64 -G -bE:$(BATCHHOME)/mak/exp/lib$(TARGET).exp -bnoentry
SLFLG = -X64 -ruc
CCFLG = -q64 -qchars=signed
CXXFLG= -q64 -qchars=signed
EXFLG = -q64 -brtl -qcpluscmt
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
BITDEF = -DBIT64
OSTYPE = aix
JAVAMD = aix
