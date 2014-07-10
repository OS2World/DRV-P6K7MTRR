/* MTRR.c - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
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
#include <string.h>
#define INCL_BASE
#define INCL_ERRORS
#include <os2.h>
#include "P6K7MTRR.h"
#include "SMPUTIL.h"
#include "VFBWC.h"

/*  */
APIRET OpenP6K7MTRR(PHFILE phfP6K7MTRR)
{
    ULONG ulWork;
    return DosOpen(DEVNAME_P6K7MTRR,
                   phfP6K7MTRR,
                   &ulWork,
                   (ULONG)0,
                   FILE_NORMAL,
                   OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                   OPEN_FLAGS_FAIL_ON_ERROR | OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,
                   (PEAOP2)NULL);
}

/*  */
APIRET QueryMTRRCaps(HFILE hfP6K7MTRR,
                     PULONG pulVariableMtrrCount,
                     PBOOL pbFixedMtrrAvail,
                     PBOOL pbWriteCombineAvail)
{
    APIRET rc;
    P6K7MTRRCAPDATA xData;
    ULONG ulDataLen;
    if((rc = DosDevIOCtl(hfP6K7MTRR,
                         IOCTL_P6K7MTRR,
                         P6K7MTRR_QRYMTRRCAP,
                         (PVOID)NULL,
                         (ULONG)0,
                         (PULONG)0,
                         (PVOID)&xData,
                         ulDataLen = (ULONG)sizeof(xData),
                         &ulDataLen)) == NO_ERROR)
    {
        *pulVariableMtrrCount = (ULONG)xData.VCNT;
        *pbFixedMtrrAvail = (BOOL)(xData.FIX != (UINT)0);
        *pbWriteCombineAvail = (BOOL)(xData.WC != (UINT)0);
    }
    return rc;
}

/*  */
APIRET SetVariableMTRR(HFILE hfP6K7MTRR,
                       ULONG ulIndex,
                       PVARIABLEMTRR pxVariableMtrr)
{
    P6K7MTRRVARIABLERANGEPARM xParm;
    ULONG ulParmLen;
    P6K7MTRRVARIABLERANGEDATA xData;
    ULONG ulDataLen;
    xParm.Index = (UCHAR)ulIndex;
    xData.Type = (UINT)pxVariableMtrr->ulType;
    xData.PhysBase = (UINT)pxVariableMtrr->ulBase;
    xData.V = (UINT)pxVariableMtrr->bValid;
    xData.PhysMask = (UINT)pxVariableMtrr->ulMask;
    return DosDevIOCtl(hfP6K7MTRR,
                       IOCTL_P6K7MTRR,
                       P6K7MTRR_SETMTRRVARIABLERANGE,
                       (PVOID)&xParm,
                       ulParmLen = (ULONG)sizeof(xParm),
                       &ulParmLen,
                       (PVOID)&xData,
                       ulDataLen = (ULONG)sizeof(xData),
                       &ulDataLen);
}

/*  */
APIRET SetVGARegionMTRR(HFILE hfP6K7MTRR,
                        ULONG ulMemoryType)
{
    APIRET rc;
    P6K7MTRRFIXEDRANGEDATA xData;
    ULONG ulDataLen;
    rc = DosDevIOCtl(hfP6K7MTRR,
                     IOCTL_P6K7MTRR,
                     P6K7MTRR_QRYMTRRFIXEDRANGE,
                     (PVOID)NULL,
                     (ULONG)0,
                     (PULONG)NULL,
                     (PVOID)&xData,
                     ulDataLen = (ULONG)sizeof(xData),
                     &ulDataLen);
    if(rc == NO_ERROR)
    {
        (void)memset((void*)&xData.Range_A0000_16K[0],
                     (int)ulMemoryType,
                     (size_t)8);
        rc = DosDevIOCtl(hfP6K7MTRR,
                         IOCTL_P6K7MTRR,
                         P6K7MTRR_SETMTRRFIXEDRANGE,
                         (PVOID)NULL,
                         (ULONG)0,
                         (PULONG)NULL,
                         (PVOID)&xData,
                         ulDataLen = (ULONG)sizeof(xData),
                         &ulDataLen);
    }
    return rc;
}

