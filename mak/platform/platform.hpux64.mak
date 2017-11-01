#|----------------------------------------------------------------------------|
#|                           TOPMake 0.0.0                                    |
#| Copyright (c) 2000-2003 Shanghai Huateng Software Systems Co., Ltd.        |
#|    All Rights Reserved                                                     |
#|----------------------------------------------------------------------------|
#| FILE NAME     : platform.hpux64.mak                                        |
#|----------------------------------------------------------------------------|

SHELL = /bin/sh

CC    = cc
CXX   = aCC
DLINK = ld
SLINK = ar
DLFLG = -b +s
SLFLG = ruc
CCFLG = +z -Ae +DA2.0W +u1 -D_REENTRANT
CXXFLG=
EXFLG = +z -Ae +DA2.0W +u1
DBFLG = -g
OPFLG = -O
DLFIX = .sl
SLFIX = .a
UXLIBS = -lm -lc
DLLIBS = -ldl
NETLIBS = -lnsl
CURLIBS = -lcurses
CRYLIBS =
THREADLIBS = -lpthread
XLIBS = -lXext -lX11
XLIBP = /usr/lib/X11R6
OSDEF = -DHP_UNIX
BITDEF = -DBIT64
OSTYPE = hpux
JAVAMD = hp-ux
