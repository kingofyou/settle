#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.hpux32.mak                                        |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = aCC
DLINK = ld
SLINK = ar
DLFLG = -b +s
SLFLG = ruc
CCFLG = +z -Ae +DAportable +u1 -D_REENTRANT
CXXFLG= +z -Ae +DAportable +u1
EXFLG = +z -Ae +DAportable +u1
DBFLG = -g
OPFLG = -O
DLFIX = .sl
SLFIX = .a
UXLIBS = -lm -lc
DLLIBS =
NETLIBS =
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthread
XLIBS = -lXext -lX11
XLIBP = /usr/lib/X11R6
OSDEF = -DHP_UNIX
OSTYPE = hpux
JAVAMD = hp-ux
