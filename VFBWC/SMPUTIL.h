/* SMPUTIL.h - OS/2 SMP utility functions dynalink library, release 1.1.2
     Copyright (c) 2001-2002 Takayuki 'January June' Suwa

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

#if !defined(__SMPUTIL)
#define __SMPUTIL

#if defined(__cplusplus)
extern "C" {
#endif

/* QueryProcessorCount() */
extern APIRET APIENTRY QueryProcessorCount(PULONG pulCount);

/* [Get/Set]ProcessorStatus() */
#if !defined(PROC_OFFLINE)
#define PROC_OFFLINE 0
#define PROC_ONLINE  1
#endif
extern APIRET APIENTRY GetProcessorStatus(ULONG ulIndex,  /* 0-based index */
                                          PULONG pulStatus);
extern APIRET APIENTRY SetProcessorStatus(ULONG ulIndex,  /* 0-based index */
                                          ULONG ulStatus);

/* [Query/Set]ThreadAffinity() */
#if !defined(AFNTY_THREAD)
typedef struct _MPAFFINITY
{
    ULONG mask[2];
} MPAFFINITY, * PMPAFFINITY;
#define AFNTY_THREAD 0
#define AFNTY_SYSTEM 1
#endif
extern APIRET APIENTRY QueryThreadAffinity(ULONG ulScope,
                                           PMPAFFINITY pxAffinity);
extern APIRET APIENTRY SetThreadAffinity(PMPAFFINITY pxAffinity);

/* InvokeEachProcessor() */
typedef VOID APIENTRY FNEPINVOKED(ULONG ulIndex,
                                  ULONG ulParameter);
typedef FNEPINVOKED* PFNEPINVOKED;
extern APIRET APIENTRY InvokeEachProcessor(PFNEPINVOKED pfnInvoked,
                                           ULONG ulParameter);

#if defined(__cplusplus)
}
#endif

#endif

