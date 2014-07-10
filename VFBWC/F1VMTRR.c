/* F1VMTRR.c - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
     Copyright 1999-2002 Takayuki 'January June' Suwa

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA. */

#pragma strings(readonly)
#define INCL_BASE
#define INCL_ERRORS
#include <os2.h>
#include "SMPUTIL.h"
#include "VFBWC.h"

/* fetches 1 variable MTRR */
VOID Fetch1VariableMTRR(PF1VMTRR pxInfo)
{
    ULONG ulPages;
    for(ulPages = (ULONG)0x1000000;
        (pxInfo->ulPageBase & (ulPages - (ULONG)1)) != (ULONG)0 ||
          ulPages > pxInfo->ulPageCount;
        ulPages >>= 1)
        ;
    pxInfo->ulMtrrBase = pxInfo->ulPageBase;
    pxInfo->ulMtrrMask = -ulPages & (ULONG)0xffffff;
    pxInfo->ulPageBase += ulPages;
    pxInfo->ulPageCount -= ulPages;
}

