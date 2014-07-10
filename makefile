# makefile - Intel P6 / AMD K7 MTRRs management IOCtl PDD, ver.0.08
#   Copyright (c) 1999-2002 Takayuki 'January June' Suwa
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
# Boston, MA 02111-1307, USA.

P6K7MTRR.sym : P6K7MTRR.map
    mapsym P6K7MTRR.map

P6K7MTRR.sys P6K7MTRR.map : P6K7MTRR.obj P6K7MTRR.def
    link386 /PACKCODE /PACKDATA /FARCALLTRANSLATION /MAP /NOLOGO P6K7MTRR.obj, P6K7MTRR.sys, P6K7MTRR.map, fromIBM\rmcalls.lib, P6K7MTRR.def
    eautil P6K7MTRR.sys nul /s
    lxlite P6K7MTRR.sys /b- /c+ /i- /l- /mr3 /ml1 /mf3 /s+ /t: /v- /yur /ydd /yxs /ynd /zs:1024

P6K7MTRR.obj : P6K7MTRR.asm
    alp -Mb +Fl P6K7MTRR.asm

