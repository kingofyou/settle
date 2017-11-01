#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.scouw.mak                                         |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = g++
DLINK = ld
SLINK = ar
DLFLG = -G
SLFLG = ruc
CCFLG =
CXXFLG= -Wall
EXFLG =
DBFLG = -g
OPFLG = -O3
DLFIX = .so
SLFIX = .a
UXLIBS = -lm
DLLIBS = -ldl
NETLIBS = -lsocket -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -Kthread
XLIBS   = -lXext -lX11 -lresolv
XLIBP   = .
OSDEF = -DSCOUW_UNIX
OSTYPE = scouw
JAVAMD = unixware
