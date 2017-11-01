#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.sunos.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = CC
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG = -Xa -misalign
CXXFLG= -misalign -KPIC
EXFLG = -Xa -misalign
DBFLG = -g
OPFLG = -O
DLFIX = .so
SLFIX = .a
UXLIBS = -lmalloc -lm -lc
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lrt -lpthread
XLIBS   = -lXext -lX11
XLIBP   = /usr/openwin/lib
OSDEF = -DSUNOS_UNIX -D_REENTRANT
OSTYPE = sunos
JAVAMD = solaris
