/* VFBWC.h - Video Frame Buffer to Write-Combining for P6K7MTRR.sys, ver.0.08e
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

/* queries main memory page count */
extern ULONG QueryMainMemoryPages(VOID);

/* checks if PM is active */
extern BOOL IsPMActive(VOID);

/* queries main memory page count */
extern BOOL QueryVFBAPages(PULONG pulPageBase,
                           PULONG pulPageCount);

/* queries main memory page count (GRADD) */
extern BOOL QueryVFBAPagesGRADD(PULONG pulPageBase,
                                PULONG pulPageCount);

/* "fetches 1 variable MTRR" structure */
typedef struct _F1VMTRR
{
    ULONG ulPageBase;
    ULONG ulPageCount;
    ULONG ulMtrrBase;
    ULONG ulMtrrMask;
} F1VMTRR, * PF1VMTRR;

/* fetches 1 variable MTRR */
extern VOID Fetch1VariableMTRR(PF1VMTRR pxInfo);

/*  */
extern APIRET OpenP6K7MTRR(PHFILE phfP6K7MTRR);

/*  */
extern APIRET QueryMTRRCaps(HFILE hfP6K7MTRR,
                            PULONG pulVariableMtrrCount,
                            PBOOL pbFixedMtrrAvail,
                            PBOOL pbWriteCombineAvail);

/*  */
typedef struct _VARIABLEMTRR
{
    ULONG ulBase;
    ULONG ulMask;
    ULONG ulType;
    BOOL bValid;
} VARIABLEMTRR, * PVARIABLEMTRR;

/*  */
extern APIRET SetVariableMTRR(HFILE hfP6K7MTRR,
                              ULONG ulIndex,
                              PVARIABLEMTRR pxVariableMtrr);

/*  */
extern APIRET SetVGARegionMTRR(HFILE hfP6K7MTRR,
                               ULONG ulMemoryType);

/*  */
extern APIRET OpenAPM(PHFILE phfAPM);

/*  */
extern APIRET CreateAPMEventSem(HFILE hfAPM,
                                PHEV phevAPM,
                                ULONG ulEventMask);

/*  */
extern struct Settings_t
{
    ULONG ulOptions;
    HFILE hfP6K7MTRR;
    ULONG ulVariableMtrrCount;
    PVARIABLEMTRR pxVariableMtrrs;
} xSettings;
#define OPTION_ROUNDUP1MB  0x00000001
#define OPTION_APPLYMTTR   0x00000002
#define OPTION_BEEPONAPPLY 0x00000004
#define OPTION_DAEMON      0x00000008
#define OPTION_INVALID     0x00000010
#define OPTION_VGAREGION   0x00000020
#define OPTION_USAGE       0x40000000
#define OPTION_DEBUG       0x80000000

