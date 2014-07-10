/* VFBWC.c - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
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
#include <stdlib.h>
#include <stdio.h>
#define INCL_BASE
#define INCL_ERRORS
#include <os2.h>
#include "P6K7MTRR.h"
#include "SMPUTIL.h"
#include "VFBWC.h"

/*  */
struct Settings_t xSettings;

/*  */
static VOID APIENTRY SetMTRRsSub(ULONG ulIndex,
                                 ULONG ulParameter)
{
    ULONG ulVariableMtrrIndex;
    for(ulVariableMtrrIndex = (ULONG)0;
        ulVariableMtrrIndex < xSettings.ulVariableMtrrCount;
        ulVariableMtrrIndex++)
        (VOID)SetVariableMTRR(xSettings.hfP6K7MTRR,
                              ulVariableMtrrIndex,
                              &xSettings.pxVariableMtrrs[ulVariableMtrrIndex]);
    if((xSettings.ulOptions & OPTION_VGAREGION) != (ULONG)0)
        (VOID)SetVGARegionMTRR(xSettings.hfP6K7MTRR,
                               (xSettings.ulOptions & OPTION_INVALID) == (ULONG)0 ? P6K7MTRR_MEMTYPE_WRITECOMBINING
                                                                                  : P6K7MTRR_MEMTYPE_UNCACHEABLE);
}

/*  */
static VOID SetMTRRs(VOID)
{
    (VOID)InvokeEachProcessor(SetMTRRsSub,
                              (ULONG)0);
    if((xSettings.ulOptions & OPTION_BEEPONAPPLY) != (ULONG)0)
        (VOID)DosBeep((ULONG)2200,
                      (ULONG)63);
}

/*  */
int main(int argc,
         char* argv[])
{
    ULONG ulParmCount;
    F1VMTRR xFrameBuffer;
    ULONG ulIndex;
    BOOL bFixedMtrrAvail;
    BOOL bWriteCombineAvail;
    F1VMTRR xMainMemory;
    HFILE hfAPM;
    HEV hevAPM;

    (VOID)DosError(FERR_DISABLEHARDERR);
    (void)printf("\n"
                 "VFBWC.exe - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e\n"
                 "Copyright 1999-2002 Takayuki 'January June' Suwa.\n");

    ulParmCount = (ULONG)0;
    xFrameBuffer.ulPageBase = (ULONG)0;
    xFrameBuffer.ulPageCount = (ULONG)0;
    for(ulIndex = (ULONG)1;
        ulIndex < (ULONG)argc;
        ulIndex++)
        if(argv[ulIndex][0] == '-' ||
           argv[ulIndex][0] == '/')
            switch(argv[ulIndex][1])
            {
                case '1':
                    xSettings.ulOptions |= OPTION_ROUNDUP1MB;
                    break;
                case 'A':
                case 'a':
                    xSettings.ulOptions |= OPTION_APPLYMTTR;
                    break;
                case 'B':
                case 'b':
                    xSettings.ulOptions |= OPTION_BEEPONAPPLY;
                    break;
                case 'D':
                case 'd':
                    xSettings.ulOptions |= OPTION_DAEMON;
                    break;
                case 'I':
                case 'i':
                    xSettings.ulOptions |= OPTION_INVALID;
                    break;
                case 'V':
                case 'v':
                    xSettings.ulOptions |= OPTION_VGAREGION;
                    break;
                case '?':
                    xSettings.ulOptions |= OPTION_USAGE;
                    break;
                case '!':
                    xSettings.ulOptions |= OPTION_DEBUG;
                    break;
                default:
                    (void)printf("  warning : invalid switch '%s', ignored.\n",
                                 (const char*)argv[ulIndex]);
                    break;
            }
        else
        {
            switch(ulParmCount)
            {
                case (ULONG)0:
                    xFrameBuffer.ulPageBase = (ULONG)strtoul((const char*)&argv[ulIndex][0],
                                                             (char**)NULL,
                                                             16) >> 12;
                    break;
                case (ULONG)1:
                    xFrameBuffer.ulPageCount = (ULONG)strtoul((const char*)&argv[ulIndex][0],
                                                              (char**)NULL,
                                                              10);
                    break;
            }
            ulParmCount++;
        }

    if((xSettings.ulOptions & OPTION_USAGE) != (ULONG)0)
        (void)printf("\n"
                     "usage:\n"
                     "  VFBWC.exe  [-options]  [{PhysAddr in hexadecimal}  {Pages in decimal}]\n"
                     "    PhysAddr/Pages is not specified, VFBWC will try to detect physical\n"
                     "    address/pages of video frame buffer aperture.\n"
                     "    (requires Presentation Manager activity)\n"
                     "options:\n"
                     "  -1  rounds Pages up to 1MB granule\n"
                     "  -a  applies settings to MTTRs actually\n"
                     "  -b  rings beep when setting is applied\n"
                     "  -d  goes into daemon mode, waits until APM suspend resuming and re-apply\n"
                     "  -i  turns to invalid settings to video frame buffer aperture /\n"
                     "        uncacheable to VGA region\n"
                     "  -v  also applies write-combining to VGA region\n"
                     "  -?  displays this message\n");
    else
        if(OpenP6K7MTRR(&xSettings.hfP6K7MTRR) != NO_ERROR)
            (void)printf("\n"
                         "cannot open P6K7MTRR.sys(" DEVNAME_P6K7MTRR ").\n"
                         "check whether P6K7MTRR.sys is installed.\n");
        else
        {
            (VOID)QueryMTRRCaps(xSettings.hfP6K7MTRR,
                                &xSettings.ulVariableMtrrCount,
                                &bFixedMtrrAvail,
                                &bWriteCombineAvail);
            if(bFixedMtrrAvail == (BOOL)FALSE ||
               bWriteCombineAvail == (BOOL)FALSE)
                (void)printf("\n"
                             "not support fixed-range MTTR / write-combining.\n");
            else
                if((xFrameBuffer.ulPageBase == (ULONG)0 ||
                    xFrameBuffer.ulPageCount == (ULONG)0) &&
                   QueryVFBAPagesGRADD(&xFrameBuffer.ulPageBase,
                                       &xFrameBuffer.ulPageCount) == (BOOL)FALSE &&
                   QueryVFBAPages(&xFrameBuffer.ulPageBase,
                                  &xFrameBuffer.ulPageCount) == (BOOL)FALSE)
                    (void)printf("\n"
                                 "cannot detect physical address/size of video frame buffer aperture.\n");
                else
                {
                    if((xSettings.ulOptions & OPTION_ROUNDUP1MB) != (ULONG)0)
                        xFrameBuffer.ulPageCount = (xFrameBuffer.ulPageCount + (ULONG)255) & (ULONG)~255;
                    xMainMemory.ulPageBase = (ULONG)0x00000;
                    xMainMemory.ulPageCount = QueryMainMemoryPages();
                    (void)printf("\n"
                                 "main memory :\n"
                                 "  %u pages @ 0x%05x000, %ukB -> write-back, valid\n"
                                 "video frame buffer aperture :\n"
                                 "  %u pages @ 0x%05x000, %ukB -> write-combining, %s\n"
                                 "VGA region (32 pages @ 0x000a0000, 128kB) : %s\n",
                                 (unsigned int)xMainMemory.ulPageCount,
                                 (unsigned int)xMainMemory.ulPageBase,
                                 (unsigned int)xMainMemory.ulPageCount * 4,
                                 (unsigned int)xFrameBuffer.ulPageCount,
                                 (unsigned int)xFrameBuffer.ulPageBase,
                                 (unsigned int)xFrameBuffer.ulPageCount * 4,
                                 (xSettings.ulOptions & OPTION_INVALID) == (ULONG)0 ? "valid"
                                                                                    : "invalid",
                                 (xSettings.ulOptions & OPTION_VGAREGION) == (ULONG)0 ? "as-is"
                                                                                      : (xSettings.ulOptions & OPTION_INVALID) == (ULONG)0 ? "write-combining"
                                                                                                                                           : "uncacheable");
                    xSettings.pxVariableMtrrs = (PVARIABLEMTRR)malloc(sizeof(VARIABLEMTRR) * (size_t)xSettings.ulVariableMtrrCount);
                    ulIndex = (ULONG)0;
                    while(xMainMemory.ulPageCount != (ULONG)0)
                    {
                        Fetch1VariableMTRR(&xMainMemory);
                        if(ulIndex < xSettings.ulVariableMtrrCount)
                        {
                            xSettings.pxVariableMtrrs[ulIndex].ulBase = xMainMemory.ulMtrrBase;
                            xSettings.pxVariableMtrrs[ulIndex].ulMask = xMainMemory.ulMtrrMask;
                            xSettings.pxVariableMtrrs[ulIndex].ulType = P6K7MTRR_MEMTYPE_WRITEBACK;
                            xSettings.pxVariableMtrrs[ulIndex].bValid = (BOOL)TRUE;
                            ulIndex++;
                        }
                    }
                    while(xFrameBuffer.ulPageCount != (ULONG)0)
                    {
                        Fetch1VariableMTRR(&xFrameBuffer);
                        if(ulIndex < xSettings.ulVariableMtrrCount)
                        {
                            xSettings.pxVariableMtrrs[ulIndex].ulBase = xFrameBuffer.ulMtrrBase;
                            xSettings.pxVariableMtrrs[ulIndex].ulMask = xFrameBuffer.ulMtrrMask;
                            xSettings.pxVariableMtrrs[ulIndex].ulType = P6K7MTRR_MEMTYPE_WRITECOMBINING;
                            xSettings.pxVariableMtrrs[ulIndex].bValid = (BOOL)((xSettings.ulOptions & OPTION_INVALID) == (ULONG)0);
                            ulIndex++;
                        }
                    }
                    for(;
                        ulIndex < xSettings.ulVariableMtrrCount;
                        ulIndex++)
                    {
                        xSettings.pxVariableMtrrs[ulIndex].ulBase = (ULONG)0x00000;
                        xSettings.pxVariableMtrrs[ulIndex].ulMask = (ULONG)0x00000;
                        xSettings.pxVariableMtrrs[ulIndex].ulType = P6K7MTRR_MEMTYPE_UNCACHEABLE;
                        xSettings.pxVariableMtrrs[ulIndex].bValid = (BOOL)FALSE;
                    }
                    if((xSettings.ulOptions & OPTION_DEBUG) != (ULONG)0)
                    {
                        (void)printf("[DEBUG] variable MTRR setttings :\n");
                        for(ulIndex = (ULONG)0;
                            ulIndex < xSettings.ulVariableMtrrCount;
                            ulIndex++)
                            (void)printf("[DEBUG]   #%-3u : base=0x%06x, mask=0x%06x, type=%u, valid=%u\n",
                                         (unsigned int)ulIndex,
                                         (unsigned int)xSettings.pxVariableMtrrs[ulIndex].ulBase,
                                         (unsigned int)xSettings.pxVariableMtrrs[ulIndex].ulMask,
                                         (unsigned int)xSettings.pxVariableMtrrs[ulIndex].ulType,
                                         (unsigned int)xSettings.pxVariableMtrrs[ulIndex].bValid);
                    }

                    if((xSettings.ulOptions & OPTION_APPLYMTTR) == (ULONG)0)
                        (void)printf("\n"
                                     "in order to apply this setting, rerun with -a option.\n");
                    else
                    {
                        SetMTRRs();
                        (void)printf("\n"
                                     "setting has been applied.\n");
                        if((xSettings.ulOptions & OPTION_DAEMON) != (ULONG)0 &&
                           OpenAPM(&hfAPM) != NO_ERROR)
                        {
                            (void)printf("\n"
                                         "cannot open APM.sys(\\DEV\\APM$).\n"
                                         "check whether APM feature is installed.\n");
                            xSettings.ulOptions &= ~OPTION_DAEMON;
                        }
                        if((xSettings.ulOptions & OPTION_DAEMON) != (ULONG)0 &&
                           CreateAPMEventSem(hfAPM,
                                             &hevAPM,
                                             (ULONG)0x700) == NO_ERROR)
                        {
                            (void)printf("\n"
                                         "go into daemon mode, waiting for APM resuming event...\n");
                            for(;;)
                            {
                                (VOID)DosWaitEventSem(hevAPM,
                                                      SEM_INDEFINITE_WAIT);
                                (VOID)DosResetEventSem(hevAPM,
                                                       &ulIndex);
                                SetMTRRs();
                            }
                        }
                    }

                    free((void*)xSettings.pxVariableMtrrs);
                }
            (VOID)DosClose(xSettings.hfP6K7MTRR);
        }

    return 0;
}

