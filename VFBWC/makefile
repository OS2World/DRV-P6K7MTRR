# makefile - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08d
#   Copyright 1999-2002 Takayuki 'January June' Suwa
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA. */

..\VFBWC.exe : VFBWC.obj VFBWC.def SMPUTIL.lib
    ilink /NOFREE /OPTFUNC /PACKCODE /PACKDATA /NOLOGO VFBWC.obj, ..\VFBWC.exe, nul, SMPUTIL.lib, VFBWC.def
    eautil ..\VFBWC.exe nul /s
    lxlite ..\VFBWC.exe /b- /c+ /i- /l- /mr3 /ml1 /mf3 /s+ /t: /v- /yur /ydd /yxs /ynd /zs:1024

VFBWC.obj : APM.w F1VMTRR.w GRADD.w ISPMACTV.w MTRR.w QMAINPG.w QVFBAPG.w VFBWC.w
    icc /C+ /G3 /Gs+ /Gi+ /Gf+ /Gu+ /O+ /Oc+ /Oi- /Ol+ /Rn /Q+ /FoVFBWC.obj APM.w F1VMTRR.w GRADD.w ISPMACTV.w MTRR.w QMAINPG.w QVFBAPG.w VFBWC.w

APM.w : APM.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ APM.c

F1VMTRR.w : F1VMTRR.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ F1VMTRR.c

GRADD.w : GRADD.c *.h ..\*.h
    icc /IfromIBM /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ /Ss+ GRADD.c

ISPMACTV.w : ISPMACTV.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ ISPMACTV.c

MTRR.w : MTRR.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ MTRR.c

QMAINPG.w : QMAINPG.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ QMAINPG.c

QVFBAPG.w : QVFBAPG.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ QVFBAPG.c

VFBWC.w : VFBWC.c *.h ..\*.h
    icc /I.. /Fi+ /Si+ /Fw+ /G3 /Gs+ /Gi+ /Gf+ /O+ /Oc+ /Oi- /Rn /Q+ VFBWC.c

