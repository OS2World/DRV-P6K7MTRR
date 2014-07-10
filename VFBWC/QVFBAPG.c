/* QVFBAPG.c - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
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
#define INCL_ORDINALS
#include <os2.h>
#include "SMPUTIL.h"
#include "VFBWC.h"

/* DevEscape(GETAPERTURE) */
#define DEVESC_GETAPERTURE 33000L

/* video frame buffer aperture infomation structure */
typedef struct _APERTURE
{
    ULONG ulPhysAddr;
    ULONG ulApertureSize;
    ULONG ulScanLineSize;
    RECTL rctlScreen;
} APERTURE, *PAPERTURE;

/* queries main memory page count */
BOOL QueryVFBAPages(PULONG pulPageBase,
                    PULONG pulPageCount)
{
    BOOL bResult = (BOOL)FALSE;
    HMODULE hmodPMWIN, hmodPMGPI;
    HAB (*APIENTRY pfnWinInitialize)(ULONG flOptions);
    BOOL (*APIENTRY pfnWinTerminate)(HAB hab);
    HDC (*APIENTRY pfnDevOpenDC)(HAB hab,
                                 LONG lType,
                                 PSZ pszToken,
                                 LONG lCount,
                                 PDEVOPENDATA pdopData,
                                 HDC hdcComp);
    HMF (*APIENTRY pfnDevCloseDC)(HDC hdc);
    LONG (*APIENTRY pfnDevEscape)(HDC hdc,
                                  LONG lCode,
                                  LONG lInCount,
                                  PBYTE pbInData,
                                  PLONG plOutCount,
                                  PBYTE pbOutData);
    HAB hAB;
    HDC hDC;
    static const DEVOPENSTRUC xDeviceOpenStructure = {(PSZ)NULL,
                                                      (PSZ)"DISPLAY",
                                                      (PDRIVDATA)NULL,
                                                      (PSZ)NULL,
                                                      (PSZ)NULL,
                                                      (PSZ)NULL,
                                                      (PSZ)NULL,
                                                      (PSZ)NULL,
                                                      (PSZ)NULL};
    LONG lOutCount;
    APERTURE xAperture;
    ULONG ulSize;
    if(IsPMActive() != (BOOL)FALSE &&
       DosLoadModule((PSZ)NULL,
                     (ULONG)0,
                     (PSZ)"PMWIN",
                     &hmodPMWIN) == NO_ERROR)
    {
        if(DosLoadModule((PSZ)NULL,
                         (ULONG)0,
                         (PSZ)"PMGPI",
                         &hmodPMGPI) == NO_ERROR)
        {
            (VOID)DosQueryProcAddr(hmodPMWIN,
                                   ORD_WIN32INITIALIZE,
                                   (PSZ)NULL,
                                   (PFN*)&pfnWinInitialize);
            (VOID)DosQueryProcAddr(hmodPMWIN,
                                   ORD_WIN32TERMINATE,
                                   (PSZ)NULL,
                                   (PFN*)&pfnWinTerminate);
            (VOID)DosQueryProcAddr(hmodPMGPI,
                                   ORD_DEV32OPENDC,
                                   (PSZ)NULL,
                                   (PFN*)&pfnDevOpenDC);
            (VOID)DosQueryProcAddr(hmodPMGPI,
                                   ORD_DEV32CLOSEDC,
                                   (PSZ)NULL,
                                   (PFN*)&pfnDevCloseDC);
            (VOID)DosQueryProcAddr(hmodPMGPI,
                                   ORD_DEV32ESCAPE,
                                   (PSZ)NULL,
                                   (PFN*)&pfnDevEscape);
            if((hAB = pfnWinInitialize((ULONG)0)) != (HAB)NULLHANDLE)
            {
                if((hDC = pfnDevOpenDC(hAB,
                                       OD_MEMORY,
                                       (PSZ)"*",
                                       (LONG)5,
                                       (PDEVOPENDATA)&xDeviceOpenStructure,
                                       (HDC)NULLHANDLE)) != DEV_ERROR)
                {
                    lOutCount = (LONG)sizeof(xAperture);
                    if(pfnDevEscape(hDC,
                                    DEVESC_GETAPERTURE,
                                    (LONG)0,
                                    (PBYTE)NULL,
                                    &lOutCount,
                                    (PBYTE)&xAperture) == DEV_OK)
                    {
                        *pulPageBase = xAperture.ulPhysAddr >> 12;
                        ulSize = xAperture.ulScanLineSize * ((ULONG)xAperture.rctlScreen.yBottom - (ULONG)xAperture.rctlScreen.yTop + (ULONG)1);
                        if((LONG)ulSize < (LONG)0)
                            ulSize = (ULONG)-(LONG)ulSize;
                        *pulPageCount = (ulSize + (ULONG)4095) >> 12;
                        bResult = (BOOL)TRUE;
                        if((xSettings.ulOptions & OPTION_DEBUG) != (ULONG)0)
                            (void)printf("[DEBUG] Presentation Manager subsystem is activated and used.\n");
                    }
                    (VOID)pfnDevCloseDC(hDC);
                }
                (VOID)pfnWinTerminate(hAB);
            }
            (VOID)DosFreeModule(hmodPMGPI);
        }
        (VOID)DosFreeModule(hmodPMWIN);
    }
    return bResult;
}

