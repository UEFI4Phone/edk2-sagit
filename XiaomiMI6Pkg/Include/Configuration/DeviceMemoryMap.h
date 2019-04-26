#ifndef _DEVICE_MEMORY_MAP_H_
#define _DEVICE_MEMORY_MAP_H_

#include <Library/ArmLib.h>

#define MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT 64

/* Below flag is used for system memory */
#define SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES                               \
  EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED |        \
      EFI_RESOURCE_ATTRIBUTE_TESTED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |     \
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |                               \
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |                         \
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |                            \
      EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE

typedef enum { NoHob, AddMem, AddDev, MaxMem } DeviceMemoryAddHob;

typedef struct {
  EFI_PHYSICAL_ADDRESS         Address;
  UINT64                       Length;
  EFI_RESOURCE_TYPE            ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttribute;
  ARM_MEMORY_REGION_ATTRIBUTES ArmAttributes;
  DeviceMemoryAddHob           HobOption;
  EFI_MEMORY_TYPE              MemoryType;
} ARM_MEMORY_REGION_DESCRIPTOR_EX, *PARM_MEMORY_REGION_DESCRIPTOR_EX;

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
    /* Address,	  Length,     ResourceType, Resource Attribute, ARM MMU
       Attribute,                  HobOption, EFI Memory Type */
    /*  DDR  */

    /*  Kernel  */
    {0x80000000, 0x05800000, EFI_RESOURCE_SYSTEM_MEMORY,
     SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES, ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK,
     AddMem, EfiConventionalMemory},
    /*  SMEM  */
    {0x86000000, 0x00200000, EFI_RESOURCE_MEMORY_RESERVED,
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE, ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED,
     AddMem, EfiReservedMemoryType},
    /*  DXE Heap  */    
    {0x86000000, 0x00200000, EFI_RESOURCE_SYSTEM_MEMORY,
     SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES, ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK,
     AddMem, EfiConventionalMemory},
    /*  Display Reserved  */
    {0x9D400000, 0x02400000, EFI_RESOURCE_MEMORY_RESERVED,
     SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES, ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK,
     AddMem, EfiReservedMemoryType},
    
    /*  FV Region  */
    {0x9D400000, 0x02400000, EFI_RESOURCE_SYSTEM_MEMORY,
     SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES, ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK,
     AddMem, EfiBootServicesData},
    /*  ABOOT FV  */

    /*  UEFI FD  */    

    /*  SEC Heap  */    
    
    /*  CPU Vectors  */    

    /*  MMU PageTables  */ 
    {0x9FF8D000, 0x00003000, EFI_RESOURCE_SYSTEM_MEMORY,
     SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES, ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK,
     AddMem, EfiBootServicesData},
    /*  UEFI Stack  */ 

    /*  DBI Dump  */ 

    /*  Log Buffer  */ 

    /*  Info Blk  */ 

    


    /* Terminator */
    {}};

#endif
