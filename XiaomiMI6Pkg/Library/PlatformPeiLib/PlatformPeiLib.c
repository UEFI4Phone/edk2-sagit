/** @file
*
*  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiPei.h>
#include <Library/ArmPlatformLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

#define INFOBLOCK_GUID { 0x90a49afd, 0x422f, 0x08ae, { 0x96, 0x11, 0xe7, 0x88, 0xd3, 0x80, 0x48, 0x45 } }
#define INFOBLOCK_ADDR (0x9FFFF000)
EFI_STATUS
EFIAPI
PlatformPeim (
  VOID
  )
{
  //See uefiplat.cfg to find this address
  UINT32* InfoBlockAddr = (UINT32 *)INFOBLOCK_ADDR;
  BuildGuidDataHob (&(EFI_GUID)INFOBLOCK_GUID , &InfoBlockAddr, 8);

  BuildFvHob (PcdGet64 (PcdFvBaseAddress), PcdGet32 (PcdFvSize));

  return EFI_SUCCESS;
}
