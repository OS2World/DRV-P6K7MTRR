/* GRADD.c - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
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
#include <stdio.h>
#define INCL_BASE
#define INCL_PM
#define INCL_ERRORS
#include <os2.h>
#include <gradd.h>
#include "SMPUTIL.h"
#include "VFBWC.h"

/* queries main memory page count (GRADD) */
BOOL QueryVFBAPagesGRADD(PULONG pulPageBase,
                         PULONG pulPageCount)
{
    BOOL bResult = (BOOL)FALSE;
    HFILE hfGRADD;
    ULONG ulIndex;
    HMODULE hmodVMAN;
    ULONG (* EXPENTRY pfnVMIEntry)(GID,
                                   ULONG,
                                   PVOID,
                                   PVOID);
    VMIQCI xQueryChainInfo;
    PGDDMODEINFO pxModeInfo;

    if(DosOpen((PSZ)"\\DEV\\GRADD$",  /* GRADD installation check (GRADD.SYS must be installed) */
               &hfGRADD,
               &ulIndex,
               (ULONG)0,
               FILE_NORMAL,
               OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_FLAGS_FAIL_ON_ERROR | OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,
               (PEAOP2)NULL) == NO_ERROR)
    {
        (VOID)DosClose(hfGRADD);
        if(IsPMActive() != (BOOL)FALSE &&
           DosLoadModule((PSZ)NULL,
                         (ULONG)0,
                         (PSZ)"VMAN",
                         &hmodVMAN) == NO_ERROR)
        {
            (VOID)DosQueryProcAddr(hmodVMAN,
                                   (ULONG)0,
                                   (PSZ)"VMIEntry",
                                   (PFN*)&pfnVMIEntry);
            if(pfnVMIEntry(GID_DONTCARE,
                           VMI_CMD_QUERYCHAININFO,
                           (PVOID)NULL,
                           (PVOID)&xQueryChainInfo) == RC_SUCCESS &&
               xQueryChainInfo.pciHead != (PCHAININFO)NULL &&
               xQueryChainInfo.pciHead->pGraddList != (PGRADDINFO)NULL)
            {
                if((xSettings.ulOptions & OPTION_DEBUG) != (ULONG)0)
                    (void)printf("[DEBUG] GRADD subsystem is detected and activated.\n");
                for(ulIndex = (ULONG)0, pxModeInfo = xQueryChainInfo.pciHead->pGraddList->pModeInfo;
                    ulIndex < xQueryChainInfo.pciHead->pGraddList->cModes;
                    ulIndex++, pxModeInfo = (PGDDMODEINFO)&((PUCHAR)pxModeInfo)[pxModeInfo->ulLength])
                    if(pxModeInfo->ulBpp >= (ULONG)8 &&
                       (ULONG)pxModeInfo->pbVRAMPhys > (ULONG)0x100000)
                    {
                        *pulPageBase = (ULONG)pxModeInfo->pbVRAMPhys >> 12;
                        *pulPageCount = (((pxModeInfo->ulLength >= (ULONG)44 && 
                                           pxModeInfo->ulTotalVRAMSize < pxModeInfo->ulApertureSize) ? pxModeInfo->ulTotalVRAMSize
                                                                                                     : pxModeInfo->ulApertureSize) + (ULONG)4095) >> 12;
                        bResult = (BOOL)TRUE;
                        if((xSettings.ulOptions & OPTION_DEBUG) != (ULONG)0)
                            (void)printf("[DEBUG] GRADD subsystem is used.\n");
                        break;
                    }
            }
            (VOID)DosFreeModule(hmodVMAN);
        }
    }

    return bResult;
}

