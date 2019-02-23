/** @file
*
*  Copyright (c) 2018, Linaro Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS 12

// DDR attributes
#define DDR_ATTRIBUTES_CACHED           ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED         ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

#define SDM845_PERIPH_BASE              0x00000000
#define SDM845_PERIPH_SZ                0x80000000

#define HIKEY960_MEMORY_SIZE               0x0000000100000000


// A compromise between the original HiKey960's reserved memory code and Lumia950XLPkg's massive tables
STATIC struct Pixel3XLReservedMemory {
  EFI_PHYSICAL_ADDRESS         Offset;
  EFI_PHYSICAL_ADDRESS         Size;
  EFI_RESOURCE_ATTRIBUTE_TYPE  Attributes;
  ARM_MEMORY_REGION_ATTRIBUTES MemoryTableAttributes;
} Pixel3XLReservedMemoryBuffer [] = {
/*
  { 0x9d400000, 0x02400000, EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE, ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH},
  { 0xa1a10000, 0x00200000, EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE, ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED},
*/
/*
  { 0x1AC00000, 0x00098000 },    // ARM-TF reserved
  { 0x32000000, 0x00100000 },    // PSTORE/RAMOOPS
  { 0x32100000, 0x00001000 },    // ADB REBOOT "REASON"
  { 0x3E000000, 0x02000000 },    // TEE OS
  { 0x89B80000, 0x00100000 },    // MCU Code reserved
  { 0x89C80000, 0x00040000 }     // MCU reserved
*/ // TODO.
};

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR describing a Physical-to-
                                    Virtual Memory mapping. This array must be ended by a zero-filled
                                    entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES  CacheAttributes;
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  UINTN                         Index = 0, Count, ReservedTop;
  EFI_PEI_HOB_POINTERS          NextHob;
  UINT64                        ResourceLength;
  EFI_PHYSICAL_ADDRESS          ResourceTop;

  ResourceAttributes = (
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
    EFI_RESOURCE_ATTRIBUTE_TESTED
  );

  // Create initial Base Hob for system memory.
  BuildResourceDescriptorHob (
    EFI_RESOURCE_SYSTEM_MEMORY,
    ResourceAttributes,
    PcdGet64 (PcdSystemMemoryBase),
    PcdGet64 (PcdSystemMemorySize)
  );

  NextHob.Raw = GetHobList ();
  Count = sizeof (Pixel3XLReservedMemoryBuffer) / sizeof (struct Pixel3XLReservedMemory);
  while ((NextHob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, NextHob.Raw)) != NULL)
  {
    if (Index >= Count)
      break;
    if ((NextHob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) &&
        (Pixel3XLReservedMemoryBuffer[Index].Offset >= NextHob.ResourceDescriptor->PhysicalStart) &&
        ((Pixel3XLReservedMemoryBuffer[Index].Offset + Pixel3XLReservedMemoryBuffer[Index].Size) <=
         NextHob.ResourceDescriptor->PhysicalStart + NextHob.ResourceDescriptor->ResourceLength))
    {
      ResourceAttributes = NextHob.ResourceDescriptor->ResourceAttribute;
      ResourceLength = NextHob.ResourceDescriptor->ResourceLength;
      ResourceTop = NextHob.ResourceDescriptor->PhysicalStart + ResourceLength;
      ReservedTop = Pixel3XLReservedMemoryBuffer[Index].Offset + Pixel3XLReservedMemoryBuffer[Index].Size;

      // Create the System Memory HOB for the reserved buffer
      BuildResourceDescriptorHob (
        EFI_RESOURCE_MEMORY_RESERVED,
        Pixel3XLReservedMemoryBuffer[Index].Attributes,
        Pixel3XLReservedMemoryBuffer[Index].Offset,
        Pixel3XLReservedMemoryBuffer[Index].Size
      );
      // Update the HOB
      NextHob.ResourceDescriptor->ResourceLength = Pixel3XLReservedMemoryBuffer[Index].Offset -
                                                   NextHob.ResourceDescriptor->PhysicalStart;

      // If there is some memory available on the top of the reserved memory then create a HOB
      if (ReservedTop < ResourceTop)
      {
        BuildResourceDescriptorHob (EFI_RESOURCE_SYSTEM_MEMORY,
                                    ResourceAttributes,
                                    ReservedTop,
                                    ResourceTop - ReservedTop);
      }
      Index++;
    }
    NextHob.Raw = GET_NEXT_HOB (NextHob);
  }

  ASSERT (VirtualMemoryMap != NULL);

  VirtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR*)AllocatePages (
                                                        EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR) * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS)
                                                        );
  if (VirtualMemoryTable == NULL) {
    return;
  }

  CacheAttributes = DDR_ATTRIBUTES_CACHED;

  Index = 0;

  // SDM845 SOC peripherals
  VirtualMemoryTable[Index].PhysicalBase    = SDM845_PERIPH_BASE;
  VirtualMemoryTable[Index].VirtualBase     = SDM845_PERIPH_BASE;
  VirtualMemoryTable[Index].Length          = SDM845_PERIPH_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;

  EFI_PHYSICAL_ADDRESS RamBase = PcdGet64 (PcdSystemMemoryBase); 

  // Honestly, I should probably just copypasta Lumia950XLPkg's table.
  // Oh well.
  for (unsigned int reserveI = 0; reserveI < Count; reserveI++) {
    UINTN lastNonReservedBlockEnd = Pixel3XLReservedMemoryBuffer[reserveI].Offset;
    if (lastNonReservedBlockEnd > RamBase) {
      VirtualMemoryTable[Index++].PhysicalBase = RamBase;
      VirtualMemoryTable[Index].VirtualBase = RamBase;
      VirtualMemoryTable[Index].Length = lastNonReservedBlockEnd - RamBase;
      VirtualMemoryTable[Index].Attributes = CacheAttributes;
    }
    VirtualMemoryTable[Index++].PhysicalBase = lastNonReservedBlockEnd;
    VirtualMemoryTable[Index].VirtualBase = lastNonReservedBlockEnd;
    VirtualMemoryTable[Index].Length = Pixel3XLReservedMemoryBuffer[reserveI].Size;
    VirtualMemoryTable[Index].Attributes = Pixel3XLReservedMemoryBuffer[reserveI].MemoryTableAttributes;
    RamBase = lastNonReservedBlockEnd + Pixel3XLReservedMemoryBuffer[reserveI].Size;
  }

  EFI_PHYSICAL_ADDRESS EndOfMemory = PcdGet64 (PcdSystemMemoryBase) + PcdGet64 (PcdSystemMemorySize);
  if (EndOfMemory > RamBase) {
    VirtualMemoryTable[Index++].PhysicalBase = RamBase;
    VirtualMemoryTable[Index].VirtualBase = RamBase;
    VirtualMemoryTable[Index].Length = EndOfMemory - RamBase;
    VirtualMemoryTable[Index].Attributes = CacheAttributes;
  }

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase  = 0;
  VirtualMemoryTable[Index].VirtualBase     = 0;
  VirtualMemoryTable[Index].Length          = 0;
  VirtualMemoryTable[Index].Attributes      = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT((Index + 1) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

  *VirtualMemoryMap = VirtualMemoryTable;
}
