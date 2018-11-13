#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ppu_thread.h>

#include <sys/process.h>
#include <sys/memory.h>

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

sys_memory_container_t cid;

int main() {

	printf("Sample started.\n");

	int result;

	result = sys_memory_allocate(0x100000, 0x400, 0x0);
	if (result != 0)
	{
		printf("ERROR: sys_memory_allocate(0x100000, 0x400, 0x0) returned 0x%x\n", result);
	}
	
	result = sys_memory_container_create(&cid, 0x100000);
	if (result != 0)
	{
		printf("ERROR: sys_memory_container_create(&cid, 0x100000) returned 0x%x\n", result);
	}
	
	result = sys_memory_allocate_from_container(0x100000, cid, 0x400, 0x0);
	if (result != 0)
	{
		printf("ERROR: sys_memory_allocate_from_container(0x100000, cid, 0x400, 0x0) returned 0x%x\n", result);
	}
	
    printf("Sample finished.\n");

    return 0;
}