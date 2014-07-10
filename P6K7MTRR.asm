; P6K7MTRR.asm - Intel P6 / AMD K7 MTRRs management IOCtl PDD, ver.0.08a
;   Copyright (c) 1999-2002 Takayuki 'January June' Suwa
;
; This library is free software; you can redistribute it and/or
; modify it under the terms of the GNU Library General Public
; License as published by the Free Software Foundation; either
; version 2 of the License, or (at your option) any later version.
;
; This library is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; Library General Public License for more details.
;
; You should have received a copy of the GNU Library General Public
; License along with this library; if not, write to the
; Free Software Foundation, Inc., 59 Temple Place - Suite 330,
; Boston, MA 02111-1307, USA.


  .686p


; PDD segments organization

DDHEADER  segment  public byte use16 'DDHEADER'
DDHEADER  ends
_DATA  segment  public byte use16 'DATA'
_DATA  ends
CONST  segment  public byte use16 'CONST'
CONST  ends
_BSS  segment  public byte use16 'BSS'
_BSS  ends
Code  segment  public byte use16 'CODE'
Code  ends
_TEXT  segment  public byte use16 'CODE'
_TEXT  ends
RMCode  segment  public byte use16 'CODE'
RMCode  ends
DGROUP  group  DDHEADER, _DATA, CONST, _BSS
CGROUP  group  Code, _TEXT, RMCode


; DD packet header
PacketHeader     struc
PacketLength     db  ?
BlockDeviceUnit  db  ?
CommandCode      db  ?
PacketStatus     dw  ?
                 dd  ?
QueueLinkage     dd  ?
PacketHeader     ends


; PDD INIT command
HeaderINITin  struc
Header        PacketHeader  <>
              db  ?
DevHlpEntry   dd  ?
ArgumentPtr   dd  ?
FirstUnit     db  ?
HeaderINITin  ends
HeaderINITout  struc
Header         PacketHeader  <>
UnitNumber     db  ?
EndResCode     dw  ?
EndResData     dw  ?
BPBArray       dd  ?
               db  ?
HeaderINITout  ends


; Device Helpers
DevHlp_Save_Message  equ  61
DevHlp_DynamicAPI    equ  108
MsgTable  struc
MsgID     dw  ?
NumItem   dw  ?
PtrItem   dd  ?
MsgTable  ends
MSG_REPLACEMENT_STRING  equ  1178


; PDD IOCTL command
HeaderIOCTL      struc
Header           PacketHeader  <>
Category         db  ?
Function         db  ?
ParameterPtr     dd  ?
DataPtr          dd  ?
SystemFileNum    dw  ?
ParameterLength  dw  ?
DataLength       dw  ?
HeaderIOCTL      ends


; IOCtl category & functions
IOCTL_P6K7MTRR                 equ  0c4h
P6K7MTRR_QRYMTRRCAP            equ  60h
data_MTRRCAP  struc
cap           dq  ?
data_MTRRCAP  ends
P6K7MTRR_SETMTRR2DEFAULT       equ  40h
P6K7MTRR_QRYMTRRDEFTYPE        equ  61h
P6K7MTRR_SETMTRRDEFTYPE        equ  41h
data_MTRRDEFTYPE  struc
deftype           dq  ?
data_MTRRDEFTYPE  ends
P6K7MTRR_QRYMTRRFIXEDRANGE     equ  62h
P6K7MTRR_SETMTRRFIXEDRANGE     equ  42h
data_MTRRFIXEDRANGE  struc
r_00000_64k          dq  ?
r_80000_16k          dq  ?
r_a0000_16k          dq  ?
r_c0000_4k           dq  ?
r_c8000_4k           dq  ?
r_d0000_4k           dq  ?
r_d8000_4k           dq  ?
r_e0000_4k           dq  ?
r_e8000_4k           dq  ?
r_f0000_4k           dq  ?
r_f8000_4k           dq  ?
data_MTRRFIXEDRANGE  ends
P6K7MTRR_QRYMTRRVARIABLERANGE  equ  63h
P6K7MTRR_SETMTRRVARIABLERANGE  equ  43h
parm_MTRRVARIABLERANGE  struc
idx                     db  ?
parm_MTRRVARIABLERANGE  ends
data_MTRRVARIABLERANGE  struc
r_base                  dq  ?
r_mask                  dq  ?
data_MTRRVARIABLERANGE  ends
P6K7MTRR_QRYMSRVALUE  equ  64h
P6K7MTRR_SETMSRVALUE  equ  44h
parm_MSRVALUE  struc
idx            dd  ?
parm_MSRVALUE  ends
data_MSRVALUE  struc
value          dq  ?
data_MSRVALUE  ends


; resource manager structures
DRIVERSTRUCT  struc
DrvrName      dd  ?
DrvrDescript  dd  ?
VendorName    dd  ?
MajorVer      db  ?
MinorVer      db  ?
DateYear      dw  ?
DateMonth     db  ?
DateDay       db  ?
DrvrFlags     dw  ?
DrvrType      dw  ?
DrvrSubType   dw  ?
DrvrCallback  dd  ?
DRIVERSTRUCT  ends


; P6/K7 MSR indices
MSR_MTRRcap           equ  254
MSR_MTRRphysBase      equ  512
MSR_MTRRphysMask      equ  513
MSR_MTRRfix64K_00000  equ  592
MSR_MTRRfix16K_80000  equ  600
MSR_MTRRfix16K_A0000  equ  601
MSR_MTRRfix4K_C0000   equ  616
MSR_MTRRfix4K_C8000   equ  617
MSR_MTRRfix4K_D0000   equ  618
MSR_MTRRfix4K_D8000   equ  619
MSR_MTRRfix4K_E0000   equ  620
MSR_MTRRfix4K_E8000   equ  621
MSR_MTRRfix4K_F0000   equ  622
MSR_MTRRfix4K_F8000   equ  623
MSR_MTRRdefType       equ  767


; CPUID strings
CPUID_Genu  equ  756e6547h
CPUID_ineI  equ  49656e69h
CPUID_ntel  equ  6c65746eh
CPUID_Auth  equ  68747541h
CPUID_enti  equ  69746e65h
CPUID_cAMD  equ  444d4163h


; data segment, resident after initialization
_DATA  segment

; copy of BIOS default P6/K7 MTRR settings
  public  default_MTRRcaps
default_MTRRcaps           data_MTRRCAP  <>
  public  default_MTRRdeftype
default_MTRRdeftype        data_MTRRDEFTYPE  <>
  public  default_MTRRfixedrange
default_MTRRfixedrange     data_MTRRFIXEDRANGE  <>
  public  default_MTRRvariablerange
default_MTRRvariablerange  data_MTRRVARIABLERANGE  256 dup(<>)

; end of DGROUP residence
  public  pbrk_DGROUP
pbrk_DGROUP::

; argument switch flags
; bit 0 : quiet, no messages (include errors)
  public  switch_flags
switch_flags  dw  0

; Ring-0 part of INITIALIZE routine callgate
  public  callgate_ring0_INITIALIZE
callgate_ring0_INITIALIZE  dd  ?

; variables for resource manager
  public  _Device_Help
_Device_Help  dd  ?
  public  _RMFlags
_RMFlags  dd  ?
  public  _RM_Help0
_RM_Help0  dd  ?
  public  _RM_Help3
_RM_Help3  dd  ?

; resource manager driver handle
  public  RM_DriverHandle
RM_DriverHandle  dd  ?

_DATA  ends


; data segment for constants, discarded after initialization
CONST  segment

; message strings
string_Title             db  1bh, "[1m", 0dh
                         db  "P6K7MTRR.sys - Intel P6 / AMD K7 MTRRs management IOCtl PDD, ver.0.08a", 0dh, 0ah
                         db  "Copyright (c) 1999-2002 Takayuki 'January June' Suwa"
                         db  0
string_NotInstalled      db  1bh, "[31m", 0dh
                         db  "Intel P6 / AMD K7 MTRR feature is NOT found. The driver has been ignored.", 0dh
                         db  1bh, "[0m"
                         db  0
string_InitializeFailed  db  1bh, "[31m", 0dh
                         db  "Ring-0 initialization failed. The driver has been ignored.", 0dh
                         db  1bh, "[0m"
                         db  0
string_InstalledP6       db  1bh, "[36m", 0dh
                         db  "Intel P6 MTRR feature is found. The driver has been installed.", 0dh
                         db  1bh, "[0m"
                         db  0
string_InstalledK7       db  1bh, "[36m", 0dh
                         db  "AMD K7 MTRR feature is found. The driver has been installed.", 0dh
                         db  1bh, "[0m"
                         db  0

; message tables
  public  msg_Title
msg_Title             MsgTable  <MSG_REPLACEMENT_STRING, 1, DGROUP:string_Title>
  public  msg_NotInstalled
msg_NotInstalled      MsgTable  <MSG_REPLACEMENT_STRING, 1, DGROUP:string_NotInstalled>
  public  msg_InitializeFailed
msg_InitializeFailed  MsgTable  <MSG_REPLACEMENT_STRING, 1, DGROUP:string_InitializeFailed>
  public  msg_InstalledP6
msg_InstalledP6       MsgTable  <MSG_REPLACEMENT_STRING, 1, DGROUP:string_InstalledP6>
  public  msg_InstalledK7
msg_InstalledK7       MsgTable  <MSG_REPLACEMENT_STRING, 1, DGROUP:string_InstalledK7>

; resource manager strings
string_DriverName         db  "P6K7MTRR.sys", 0
string_DriverDescription  db  "Intel P6 / AMD K7 MTRRs management IOCtl PDD", 0
string_VendorName         db  "Takayuki 'January June' Suwa", 0

; resource manager DRIVERSTRUCT structures
  public  RM_DriverStruct
RM_DriverStruct  DRIVERSTRUCT  <DGROUP:string_DriverName, DGROUP:string_DriverDescription, DGROUP:string_VendorName, 1, 1, 2002, 6, 18, 0, 3, 3, 0>

CONST  ends


; code segment, resident after initialization
_TEXT  segment
  assume  cs:CGROUP, ds:DGROUP, es:nothing, ss:nothing, fs:nothing, gs:nothing

  extrn  _RMCreateDriver:far

;
_add  macro  reg, additive
      if  (additive) eq 1
      inc  reg
      else
      add  reg, additive
      endif
      endm

;  called before MTRR change
pre_MTRRchange  proc  near
  cli                        ; disable maskable interrupts
  mov  eax, cr0              ; disable caching
  or  eax, 1 shl 30
  mov  cr0, eax
  wbinvd                     ; flush all cache contents
  mov  eax, cr3              ; flush all TLBs
  mov  cr3, eax
;  mov  ecx, MSR_MTRRdefType  ; disable MTRR feature
  lea  ecx, ds:[MSR_MTRRdefType]
  rdmsr
;  and  eax, not (1 shl 11)
  and  ax, not (1 shl 11)
  wrmsr
  ret
pre_MTRRchange  endp

;  called after MTRR change
post_MTRRchange  proc  near
  wbinvd                     ; flush all cache contents
  mov  eax, cr3              ; flush all TLBs
  mov  cr3, eax
;  mov  ecx, MSR_MTRRdefType  ; disable MTRR feature
  lea  ecx, ds:[MSR_MTRRdefType]
  rdmsr
;  or  eax, 1 shl 11
  or  ax, 1 shl 11
  wrmsr
  mov  eax, cr0              ; enable caching
  and  eax, not (1 shl 30)
  mov  cr0, eax
  sti                        ; enable maskable interrupts
  ret
post_MTRRchange  endp

; MTRR memory type checker
check_MemType  proc  near
  test  al, al     ; UC
  jz  short check_MemType_0
  cmp  al, 1       ; WC
  je  short check_MemType_0
  cmp  al, 4       ; WT
  je  short check_MemType_0
  cmp  al, 5       ; WP
  je  short check_MemType_0
  cmp  al, 6       ; WB
check_MemType_0:
  ret
check_MemType  endp

; query MTRR fixed range
query_MTRRFixedRange  proc  near
;  mov  ecx, MSR_MTRRfix64K_00000
  lea  ecx, ds:[MSR_MTRRfix64K_00000]
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix16K_80000 - MSR_MTRRfix64K_00000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix16K_A0000 - MSR_MTRRfix16K_80000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_C0000 - MSR_MTRRfix16K_A0000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_C8000 - MSR_MTRRfix4K_C0000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_D0000 - MSR_MTRRfix4K_C8000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_D8000 - MSR_MTRRfix4K_D0000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_E0000 - MSR_MTRRfix4K_D8000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_E8000 - MSR_MTRRfix4K_E0000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_F0000 - MSR_MTRRfix4K_E8000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRfix4K_F8000 - MSR_MTRRfix4K_F0000>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  ret
query_MTRRFixedRange  endp

; set MTRR fixed range
set_MTRRFixedRange  proc  near
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_00000_64k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_00000_64k[4]
;  mov  ecx, MSR_MTRRfix64K_00000
  lea  ecx, ds:[MSR_MTRRfix64K_00000]
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_80000_16k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_80000_16k[4]
  _add  cx, <MSR_MTRRfix16K_80000 - MSR_MTRRfix64K_00000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_a0000_16k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_a0000_16k[4]
  _add  cx, <MSR_MTRRfix16K_A0000 - MSR_MTRRfix16K_80000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_c0000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_c0000_4k[4]
  _add  cx, <MSR_MTRRfix4K_C0000 - MSR_MTRRfix16K_A0000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_c8000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_c8000_4k[4]
  _add  cx, <MSR_MTRRfix4K_C8000 - MSR_MTRRfix4K_C0000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_d0000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_d0000_4k[4]
  _add  cx, <MSR_MTRRfix4K_D0000 - MSR_MTRRfix4K_C8000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_d8000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_d8000_4k[4]
  _add  cx, <MSR_MTRRfix4K_D8000 - MSR_MTRRfix4K_D0000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_e0000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_e0000_4k[4]
  _add  cx, <MSR_MTRRfix4K_E0000 - MSR_MTRRfix4K_D8000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_e8000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_e8000_4k[4]
  _add  cx, <MSR_MTRRfix4K_E8000 - MSR_MTRRfix4K_E0000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_f0000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_f0000_4k[4]
  _add  cx, <MSR_MTRRfix4K_F0000 - MSR_MTRRfix4K_E8000>
  wrmsr
  mov  eax, dword ptr data_MTRRFIXEDRANGE[si].r_f8000_4k[0]
  mov  edx, dword ptr data_MTRRFIXEDRANGE[si].r_f8000_4k[4]
  _add  cx, <MSR_MTRRfix4K_F8000 - MSR_MTRRfix4K_F0000>
  wrmsr
  ret
set_MTRRFixedRange  endp

; strategy entry
Strategy  proc  far
  ; need not preserve any registers :)
  push  bx  ; move bx value to bp in order to reserve for general purpose
  pop  bp
  push  es  ; move es value to gs in order to reserve for string operations
  pop  gs
  mov  al, gs:PacketHeader[bp].CommandCode
  test  al, al           ; INITIALIZE
  je  near ptr INITIALIZE
  cmp  al, 16            ; IOCTL
  je  short IOCTL
  cmp  al, 28            ; SHUTDOWN
  je  short exit_Done
;
  public  exit_UnknownCommand
exit_UnknownCommand::
  mov  ax, 8103h  ; unknown command
  jmp  short exit_Error
;
  public  exit_Invalid
exit_Invalid::
  mov  ax, 8113h  ; invalid parameter
;
  public  exit_Error
exit_Error::
  stc
  jmp  short exit
;
  public  exit_Done
exit_Done::
  mov  ax, 100h
  clc
;
  public  exit
exit::
  mov  gs:PacketHeader[bp].PacketStatus, ax
  ret
Strategy  endp

; IOCTL routine
IOCTL  proc  near
  cmp  gs:HeaderIOCTL[bp].Category, IOCTL_P6K7MTRR
  jne  short exit_Invalid
  mov  al, gs:HeaderIOCTL[bp].Function
  cmp  al, P6K7MTRR_QRYMTRRCAP
  je  short IOCTL_QRYMTTRCAP
  cmp  al, P6K7MTRR_QRYMTRRDEFTYPE
  je  short IOCTL_QRYMTRRDEFTYPE
  cmp  al, P6K7MTRR_QRYMTRRFIXEDRANGE
  je  short IOCTL_QRYMTRRFIXEDRANGE
  cmp  al, P6K7MTRR_QRYMTRRVARIABLERANGE
  je  short IOCTL_QRYMTRRVARIABLERANGE
  cmp  al, P6K7MTRR_QRYMSRVALUE
  je  short IOCTL_QRYMSRVALUE
  cmp  al, P6K7MTRR_SETMTRR2DEFAULT
  je  near ptr IOCTL_SETMTRR2DEFAULT
  cmp  al, P6K7MTRR_SETMTRRDEFTYPE
  je  near ptr IOCTL_SETMTRRDEFTYPE
  cmp  al, P6K7MTRR_SETMTRRFIXEDRANGE
  je  near ptr IOCTL_SETMTRRFIXEDRANGE
  cmp  al, P6K7MTRR_SETMTRRVARIABLERANGE
  je  near ptr IOCTL_SETMTRRVARIABLERANGE
  cmp  al, P6K7MTRR_SETMSRVALUE
  je  near ptr IOCTL_SETMSRVALUE
  jmp  short exit_Invalid
;
  public  IOCTL_QRYMTTRCAP
IOCTL_QRYMTTRCAP::
  mov  si, offset DGROUP:default_MTRRcaps
  les  di, gs:HeaderIOCTL[bp].DataPtr
  movsd
  movsd
  jmp  short exit_Done
;
  public  IOCTL_QRYMTRRDEFTYPE
IOCTL_QRYMTRRDEFTYPE::
  les  di, gs:HeaderIOCTL[bp].DataPtr
;  mov  ecx, MSR_MTRRdefType
  lea  ecx, ds:[MSR_MTRRdefType]
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  jmp  short exit_Done
;
  public  IOCTL_QRYMTRRFIXEDRANGE
IOCTL_QRYMTRRFIXEDRANGE::
  les  di, gs:HeaderIOCTL[bp].DataPtr
  call  query_MTRRFixedRange
  jmp  near ptr exit_Done
;
  public  IOCTL_QRYMTRRVARIABLERANGE
IOCTL_QRYMTRRVARIABLERANGE::
  les  di, gs:HeaderIOCTL[bp].ParameterPtr
  movzx  ecx, es:parm_MTRRVARIABLERANGE[di].idx
  cmp  cl, byte ptr DGROUP:default_MTRRcaps.cap[0]
  jnb  near ptr exit_Invalid
  les  di, gs:HeaderIOCTL[bp].DataPtr
  shl  cx, 1
  add  cx, MSR_MTRRphysBase
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRphysMask - MSR_MTRRphysBase>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  jmp  near ptr exit_Done
;
  public  IOCTL_QRYMSRVALUE
IOCTL_QRYMSRVALUE::
  les  di, gs:HeaderIOCTL[bp].ParameterPtr
  mov  ecx, es:parm_MSRVALUE[di].idx
  les  di, gs:HeaderIOCTL[bp].DataPtr
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  jmp  near ptr exit_Done
;
  public  IOCTL_SETMTRR2DEFAULT
IOCTL_SETMTRR2DEFAULT::
  call  pre_MTRRchange
;  mov  eax, dword ptr DGROUP:default_MTRRdeftype.deftype[0]
;  and  eax, 11111111111111111111011111111111b
  movzx  eax, word ptr DGROUP:default_MTRRdeftype.deftype[0]
  and  ax, 1111011111111111b
  mov  edx, dword ptr DGROUP:default_MTRRdeftype.deftype[4]
;  mov  ecx, MSR_MTRRdefType
  lea  ecx, ds:[MSR_MTRRdefType]
  wrmsr
  mov  si, offset DGROUP:default_MTRRfixedrange
  call  set_MTRRFixedRange
  mov  si, offset DGROUP:default_MTRRvariablerange
  movzx  bx, byte ptr DGROUP:default_MTRRcaps.cap[0]
  mov  cx, MSR_MTRRphysBase
IOCTL_SETMTRR2DEFAULT_1:
  mov  eax, dword ptr data_MTRRVARIABLERANGE[si].r_base[0]
  mov  edx, dword ptr data_MTRRVARIABLERANGE[si].r_base[4]
  wrmsr
  _add  cx, <MSR_MTRRphysMask - MSR_MTRRphysBase>
  mov  eax, dword ptr data_MTRRVARIABLERANGE[si].r_mask[0]
  mov  edx, dword ptr data_MTRRVARIABLERANGE[si].r_mask[4]
  wrmsr
  _add  cx, <MSR_MTRRphysMask - MSR_MTRRphysBase>
  add  si, size data_MTRRVARIABLERANGE
  dec  bx
  jnz  short IOCTL_SETMTRR2DEFAULT_1
  call  post_MTRRchange
  jmp  near ptr exit_Done
;
  public  IOCTL_SETMTRRDEFTYPE
IOCTL_SETMTRRDEFTYPE::
  lds  si, gs:HeaderIOCTL[bp].DataPtr
  assume  ds:nothing
  mov  al, byte ptr data_MTRRDEFTYPE[si].deftype[0]
  call  check_MemType
  jnz  near ptr exit_Invalid
  call  pre_MTRRchange
;  mov  eax, dword ptr data_MTRRDEFTYPE[si].deftype[0]
;  and  eax, 00000000000000000000010011111111b
  movzx  eax, word ptr data_MTRRDEFTYPE[si].deftype[0]
  and  ax, 0000010011111111b
;  mov  edx, dword ptr data_MTRRDEFTYPE[si].deftype[4]
  xor  edx, edx
;  mov  ecx, MSR_MTRRdefType
  lea  ecx, ds:[MSR_MTRRdefType]
  wrmsr
  call  post_MTRRchange
  jmp  near ptr exit_Done
  assume  ds:DGROUP
;
  public  IOCTL_SETMTRRFIXEDRANGE
IOCTL_SETMTRRFIXEDRANGE::
  lds  si, gs:HeaderIOCTL[bp].DataPtr
  assume  ds:nothing
  mov  cx, size data_MTRRFIXEDRANGE
IOCTL_SETMTRRDEFTYPE_0:
  lodsb
  call  check_MemType
  jnz  near ptr exit_Invalid
  loop  IOCTL_SETMTRRDEFTYPE_0
  sub  si, size data_MTRRFIXEDRANGE
  call  pre_MTRRchange
  call  set_MTRRFixedRange
  call  post_MTRRchange
  jmp  near ptr exit_Done
  assume  ds:DGROUP
;
  public  IOCTL_SETMTRRVARIABLERANGE
IOCTL_SETMTRRVARIABLERANGE::
  les  di, gs:HeaderIOCTL[bp].ParameterPtr
  movzx  bx, es:parm_MTRRVARIABLERANGE[di].idx
  cmp  bl, byte ptr DGROUP:default_MTRRcaps.cap[0]
  jnb  near ptr exit_Invalid
  lds  si, gs:HeaderIOCTL[bp].DataPtr
  assume  ds:nothing
  mov  al, byte ptr data_MTRRVARIABLERANGE[si].r_base[0]
  call  check_MemType
  jnz  near ptr exit_Invalid
  call  pre_MTRRchange
  mov  eax, dword ptr data_MTRRVARIABLERANGE[si].r_base[0]
;  and  eax, 11111111111111111111000011111111b
  and  ax, 1111000011111111b
;  mov  edx, dword ptr data_MTRRVARIABLERANGE[si].r_base[4]
;  and  edx, 00000000000000000000000000001111b
  movzx  edx, byte ptr data_MTRRVARIABLERANGE[si].r_base[4]
  and  dl, 00001111b
;  lea  ecx, [ebx+ebx+MSR_MTRRphysBase]
  lea  cx, [bx+MSR_MTRRphysBase]  ; high 16bits of ecx are already 0 (see pre_MTRRchange)
  add  cx, bx
  wrmsr
  mov  eax, dword ptr data_MTRRVARIABLERANGE[si].r_mask[0]
;  and  eax, 11111111111111111111100000000000b
  and  ax, 1111100000000000b
;  mov  edx, dword ptr data_MTRRVARIABLERANGE[si].r_mask[4]
;  and  edx, 00000000000000000000000000001111b
  movzx  edx, byte ptr data_MTRRVARIABLERANGE[si].r_mask[4]
  and  dl, 00001111b
  _add  cx, <MSR_MTRRphysMask - MSR_MTRRphysBase>
  wrmsr
  call  post_MTRRchange
  jmp  near ptr exit_Done
  assume  ds:DGROUP
;
  public  IOCTL_SETMSRVALUE
IOCTL_SETMSRVALUE::
  cli                       ; disable maskable interrupts
  mov  eax, cr0             ; disable caching
  or  eax, 1 shl 30
  mov  cr0, eax
  wbinvd                    ; flush all cache contents
  mov  eax, cr3             ; flush all TLBs
  mov  cr3, eax
  les  di, gs:HeaderIOCTL[bp].ParameterPtr
  mov  ecx, es:parm_MSRVALUE[di].idx
  les  di, gs:HeaderIOCTL[bp].DataPtr
  mov  eax, dword ptr es:data_MSRVALUE[di].value[0]
  mov  edx, dword ptr es:data_MSRVALUE[di].value[4]
  wrmsr
  wbinvd                    ; flush all cache contents
  mov  eax, cr3             ; flush all TLBs
  mov  cr3, eax
  mov  eax, cr0             ; enable caching
  and  eax, not (1 shl 30)
  mov  cr0, eax
  sti                       ; enable maskable interrupts
  jmp  near ptr exit_Done
IOCTL  endp

; end of CGROUP residence
  public  pbrk_CGROUP
pbrk_CGROUP::

; INITIALIZE routine
INITIALIZE  proc  far
  les  si, gs:HeaderINITin[bp].ArgumentPtr
INITIALIZE_0:
  lodsb  es:[si]
  test  al, al
  jz  short INITIALIZE_2
  cmp  al, '-'
  je  short INITIALIZE_1
  cmp  al, '/'
  jne  short INITIALIZE_0
INITIALIZE_1:
  lodsb  es:[si]
  test  al, al
  jz  short INITIALIZE_2
  and  al, not ('a' - 'A')
  cmp  al, 'Q'
  jne  short INITIALIZE_0
  or  DGROUP:switch_flags, 1b
  jmp  short INITIALIZE_0
INITIALIZE_2:
  mov  si, offset DGROUP:msg_Title
  call  save_Message
  call  check_P6K7MTRR
  test  ax, ax
  jnz  short INITIALIZE_4
  mov  si, offset DGROUP:msg_NotInstalled
INITIALIZE_3:
  call  save_Message
  xor  ax, ax
  mov  gs:HeaderINITout[bp].EndResCode, ax
  mov  gs:HeaderINITout[bp].EndResData, ax
  mov  ax, 8115h  ; initialization failed
  jmp  near ptr exit_Error
INITIALIZE_4:
  mov  si, offset DGROUP:msg_InstalledP6
  dec  ax
  jz  short INITIALIZE_5
  mov  si, offset DGROUP:msg_InstalledK7
INITIALIZE_5:
  mov  bx, offset CGROUP:ring0_INITIALIZE
  mov  ax, cs
  and  al, 0fch
  xor  cx, cx
  mov  dx, DevHlp_DynamicAPI + 3*256  ; 16bit, 16:16 addressing
  call  gs:HeaderINITin[bp].DevHlpEntry
  jnc  short INITIALIZE_6
  mov  si, offset DGROUP:msg_InitializeFailed
  jmp  short INITIALIZE_3
INITIALIZE_6:
  call  save_Message
  shl  edi, 16
  mov  DGROUP:callgate_ring0_INITIALIZE, edi
  call  DGROUP:callgate_ring0_INITIALIZE
  mov  eax, gs:HeaderINITin[bp].DevHlpEntry
  mov  DGROUP:_Device_Help, eax
  push  ds
  push  offset DGROUP:RM_DriverHandle
  push  ds
  push  offset DGROUP:RM_DriverStruct
;  call  _RMCreateDriver
  push  cs                        ; same physical segment
  call  near ptr _RMCreateDriver  ;
  add  sp, 8
  mov  gs:HeaderINITout[bp].EndResCode, offset CGROUP:pbrk_CGROUP
  movzx  ax, byte ptr DGROUP:default_MTRRcaps.cap[0]
  shl  ax, 4
  add  ax, offset DGROUP:default_MTRRvariablerange
  mov  gs:HeaderINITout[bp].EndResData, ax
  jmp  near ptr exit_Done
INITIALIZE  endp

; display message
save_Message  proc  near
  test  DGROUP:switch_flags, 1b
  jnz  short save_Message_0
  xor  bx, bx
  mov  dl, DevHlp_Save_Message
  call  gs:HeaderINITin[bp].DevHlpEntry
save_Message_0:
  ret
save_Message  endp

; P6/K7 MTRRs presence checker
check_P6K7MTRR  proc  near
  xor  si, si
  pushfd         ; CPUID availablity check
  pop  edx
  mov  eax, 1 shl 21
  xor  eax, edx
  push  eax
  popfd
  pushfd
  pop  eax
  xor  eax, edx
  jz  short check_P6K7MTRR_2
  xor  eax, eax  ; CPUID std.func. & vender check
  cpuid
  test  eax, eax
  jz  short check_P6K7MTRR_2
  cmp  ebx, CPUID_Genu
  jne  short check_P6K7MTRR_0
  cmp  edx, CPUID_ineI
  jne  short check_P6K7MTRR_0
  cmp  ecx, CPUID_ntel
  je  short check_P6K7MTRR_1
check_P6K7MTRR_0:
  cmp  ebx, CPUID_Auth
  jne  short check_P6K7MTRR_2
  cmp  edx, CPUID_enti
  jne  short check_P6K7MTRR_2
  cmp  ecx, CPUID_cAMD
  jne  short check_P6K7MTRR_2
  inc  si
check_P6K7MTRR_1:
  inc  si
;  mov  eax, 1  ; CPUID MTRR feature check
  lea  eax, ds:[1]
  cpuid
  shr  dx, 12+1
  sbb  ax, ax
  and  si, ax
check_P6K7MTRR_2:
  xchg  ax, si
  ret
check_P6K7MTRR  endp

; Ring-0 part of INITIALIZE routine
ring0_INITIALIZE  proc  far
  assume  ds:nothing
  push  seg DGROUP
  pop  ds
  assume  ds:DGROUP
  mov  di, offset DGROUP:default_MTRRcaps
  push  ds
  pop  es
;  mov  ecx, MSR_MTRRcap
  lea  ecx, ds:[MSR_MTRRcap]
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  mov  cx, MSR_MTRRdefType
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  call  query_MTRRFixedRange
  movzx  bx, byte ptr DGROUP:default_MTRRcaps.cap[0]
  mov  cx, MSR_MTRRphysBase
ring0_INITIALIZE_0:
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRphysMask - MSR_MTRRphysBase>
  rdmsr
  stosd
  xchg  eax, edx
  stosd
  _add  cx, <MSR_MTRRphysMask - MSR_MTRRphysBase>
  dec  bx
  jnz  short ring0_INITIALIZE_0
  ret
ring0_INITIALIZE  endp

_TEXT  ends


; data segment for DD header, resident after initialization
DDHEADER  segment

  dd  -1                        ; far pointer to next device header
  dw  1000000110000000b         ; device attribute
;     CIISO LE    CNSK              CHR, level=3
;     HDBHP  VE   LUCB
;     RCMRN   L   KLRD
  dw  offset CGROUP:Strategy  ; 16bit offset to strategy entry of default code segment
  dw  0                         ; 16bit offset to IDC entry of default code segment
  db  "P6MTRR$ "                ; driver name
  db  8 dup(0)
  dd  00011b                    ; capabilities bit strip
                                ;   DosDevIOCtl2(), > 16MB addressibility

DDHEADER  ends


  end

