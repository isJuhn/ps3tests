#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ppu_thread.h>

#include <sys/process.h>
#include <sys/memory.h>
#include <sys/vm.h>

// Set priority and stack size for the primary PPU thread.
// Priority : 1000
// Stack    : 64KB
SYS_PROCESS_PARAM(1000, 0x10000)

typedef uintptr_t uptr;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t s32;

sys_addr_t addr;

int main() {

	int result;

	result = sys_vm_memory_map(0x10000000, 0x100000, 0xffffffff, 0x200, 0x1, &addr);
	if (result != 0)
	{
		printf("ERROR: sys_vm_memory_map(0x10000000,...) returned 0x%x\n", result);
		return 0;
	}
	
	printf("Address: 0x%x\n", addr);
    printf("Sample finished.\n");

    return 0;
}