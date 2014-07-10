/* P6K7MTRR.h - definitions for DosDevIOCtl() using P6K7MTRR.sys
     Copyright (c) 1999-2002 Takayuki 'January June' Suwa

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

#if !defined(__P6K7MTRR)
#define __P6K7MTRR

#if defined(__cplusplus)
extern "C" {
#endif

/* device name */
#define DEVNAME_P6K7MTRR "\\DEV\\P6MTRR$"

/* IOCtl category & functions */
#define IOCTL_P6K7MTRR                0xc4
#define P6K7MTRR_QRYMTRRCAP           0x60  /* Query MTRR capabilities */
#define P6K7MTRR_QRYMTRRDEFTYPE       0x61  /* Query MTRR default type */
#define P6K7MTRR_QRYMTRRFIXEDRANGE    0x62  /* Query MTRR fixed ranges */
#define P6K7MTRR_QRYMTRRVARIABLERANGE 0x63  /* Query MTRR variable range */
#define P6K7MTRR_QRYMSRVALUE          0x64  /* Query MSR value (v0.08a+) */
#define P6K7MTRR_SETMTRR2DEFAULT      0x40  /* Set MTRRs to default setting (v0.05+) */
#define P6K7MTRR_SETMTRRDEFTYPE       0x41  /* Set MTRR default type */
#define P6K7MTRR_SETMTRRFIXEDRANGE    0x42  /* Set MTRR fixed ranges */
#define P6K7MTRR_SETMTRRVARIABLERANGE 0x43  /* Set MTRR variable range */
#define P6K7MTRR_SETMSRVALUE          0x44  /* Set MSR value (v0.08a+) */

/* Memory types */
#define P6K7MTRR_MEMTYPE_UNCACHEABLE    0  /* Uncacheable, strong write ordered */
#define P6K7MTRR_MEMTYPE_WRITECOMBINING 1  /* Write-combining, weak write ordered */
#define P6K7MTRR_MEMTYPE_WRITETHROUGH   4  /* Write-through, strong write ordered */
#define P6K7MTRR_MEMTYPE_WRITEPROTECTED 5  /* Write-protected, no write operations */
#define P6K7MTRR_MEMTYPE_WRITEBACK      6  /* Write-back, no write ordered */

/* P6/K7 MTRR capabilities
     for P6K7MTRR_QRYMTRRCAP */
typedef struct
{
    UINT VCNT:8;  /* # of MTRR variable ranges */
    UINT FIX:1;   /* MTRR fixed range support flag, 1 if supported */
    UINT :1;
    UINT WC:1;    /* Write-combining support flag, 1 if supported */
    UINT :21;
    UINT :32;
} P6K7MTRRCAPDATA, * PP6K7MTRRCAPDATA;

/* P6/K7 MTRR default type
     for P6K7MTRR_QRYMTRRDEFTYPE / P6K7MTRR_SETMTRRDEFTYPE */
typedef struct
{
    UINT Type:8;  /* Default memory type, must be one of P6K7MTRR_MEMTYPE_* */
    UINT :2;
    UINT FE:1;    /* MTRR fixed range enabled flag, 1 if enabled */
    UINT E:1;     /* MTRR enabled flag, 1 if enabled (but cannot be 0 because of restriction of P6K7MTRR.sys) */
    UINT :20;
    UINT :32;     /* Other bits should be 0 when set */
} P6K7MTRRDEFTYPEDATA, * PP6K7MTRRDEFTYPEDATA;

/* P6/K7 MTRR fixed ranges
     for P6K7MTRR_QRYMTRRFIXEDRANGE / P6K7MTRR_SETMTRRFIXEDRANGE */
typedef struct
{
    UCHAR Range_00000_64K[8];  /* Memory types, 00000-7FFFF, 64k each */
    UCHAR Range_80000_16K[8];  /* Memory types, 80000-9FFFF, 16k each */
    UCHAR Range_A0000_16K[8];  /* Memory types, A0000-BFFFF, 16k each */
    UCHAR Range_C0000_4K[8];   /* Memory types, C0000-C7FFF, 4k each */
    UCHAR Range_C8000_4K[8];   /* Memory types, C8000-CFFFF, 4k each */
    UCHAR Range_D0000_4K[8];   /* Memory types, D0000-D7FFF, 4k each */
    UCHAR Range_D8000_4K[8];   /* Memory types, D8000-DFFFF, 4k each */
    UCHAR Range_E0000_4K[8];   /* Memory types, E0000-E7FFF, 4k each */
    UCHAR Range_E8000_4K[8];   /* Memory types, E8000-EFFFF, 4k each */
    UCHAR Range_F0000_4K[8];   /* Memory types, F0000-F7FFF, 4k each */
    UCHAR Range_F8000_4K[8];   /* Memory types, F8000-FFFFF, 4k each
                                  Each of memory types must be one of P6K7MTRR_MEMTYPE_* */
} P6K7MTRRFIXEDRANGEDATA, * PP6K7MTRRFIXEDRANGEDATA;

/* P6/K7 MTRR variable range
     for P6K7MTRR_QRYMTRRVARIABLERANGE / P6K7MTRR_SETMTRRVARIABLERANGE */
typedef struct
{
    UCHAR Index;  /* Index of MTRR variable range, must be between 0 to P6K7MTRRCAPDATA.VCNT-1 */
} P6K7MTRRVARIABLERANGEPARM, * PP6K7MTRRVARIABLERANGEPARM;
typedef struct
{
    UINT Type:8;       /* Memory type, must be one of P6K7MTRR_MEMTYPE_* */
    UINT :4;
    UINT PhysBase:24;  /* Physical Memory base, most 24bits of 36bits */
    UINT :28;
    UINT :11;
    UINT V:1;          /* Valid flag, 0(invalid) or 1(valid) */
    UINT PhysMask:24;  /* Physical memory mask, most 24bits of 36bits */
    UINT :28;          /* Other bits should be 0 when set */
} P6K7MTRRVARIABLERANGEDATA, * PP6K7MTRRVARIABLERANGEDATA;

/* P6/K7 MSR index/value
     for P6K7MTRR_QRYMSRVALUE / P6K7MTRR_SETMSRVALUE */
typedef struct
{
    ULONG Index;  /* Index of MSR, Be careful because any access to undefined MSR causes kernel-level crash (GPF) */
} P6K7MTRRQRYMSRVALUEPARM, * PP6K7MTRRQRYMSRVALUEPARM;
typedef struct
{
    QWORD value;  /* Value of MSR */
} P6K7MTRRQRYMSRVALUEDATA, * PP6K7MTRRQRYMSRVALUEDATA;

#if defined(__cplusplus)
}
#endif

#endif

