/* APM.c - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
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
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#include <os2.h>
#include "SMPUTIL.h"
#include "VFBWC.h"

/*  */
APIRET OpenAPM(PHFILE phfAPM)
{
    ULONG ulWork;
    return DosOpen((PSZ)"\\DEV\\APM$",
                   phfAPM,
                   &ulWork,
                   (ULONG)0,
                   FILE_NORMAL,
                   OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                   OPEN_FLAGS_FAIL_ON_ERROR | OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,
                   (PEAOP2)NULL);
}

/*  */
APIRET CreateAPMEventSem(HFILE hfAPM,
                         PHEV phevAPM,
                         ULONG ulEventMask)
{
    APIRET rc;
    HEV hev;
#pragma pack(1)
    struct
    {
        USHORT usFlags;
        ULONG ulSemHandle;
        ULONG ulEventMask;
    } xParm;
    struct
    {
        USHORT usResult;
    } xData;
#pragma pack()
    ULONG ulParmLen, ulDataLen;
    rc = DosCreateEventSem((PSZ)NULL,
                           &hev,
                           DC_SEM_SHARED,
                           (BOOL32)FALSE);
    if(rc == NO_ERROR)
    {
        xParm.usFlags = (USHORT)9;
        xParm.ulSemHandle = (ULONG)hev;
        xParm.ulEventMask = ulEventMask;
        rc = DosDevIOCtl(hfAPM,
                         IOCTL_POWER,
                         POWER_SETPOWEREVENTRES,
                         (PVOID)&xParm,
                         ulParmLen = (ULONG)sizeof(xParm),
                         &ulParmLen,
                         (PVOID)&xData,
                         ulDataLen = (ULONG)sizeof(xData),
                         &ulDataLen);
        if(rc == NO_ERROR)
        {
            if(xData.usResult == (USHORT)0)
            {
                *phevAPM = hev;
                return NO_ERROR;
            }
            rc = (APIRET)-1;
        }
        (VOID)DosCloseEventSem(hev);
    }
    return rc;
}

