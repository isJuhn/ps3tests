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
sys_addr_t addr2;

int main() {

	int result;
	
	printf("Sample started.\n");
	
	printf("Mapping 32MB of virtual memory with 1MB of physical memory used.\n");
	result = sys_vm_memory_map(0x2000000, 0x100000, 0xffffffff, 0x200, 0x1, &addr);
	if (result != 0)
	{
		printf("ERROR: sys_vm_memory_map(0x2000000,...) returned 0x%x\n", result);
		return 0;
	}
	printf("Address: 0x%x\n", addr);
	
	printf("Mapping 32MB of virtual memory with 1MB of physical memory used.\n");
	result = sys_vm_memory_map(0x2000000, 0x100000, 0xffffffff, 0x200, 0x1, &addr2);
	if (result != 0)
	{
		printf("ERROR: sys_vm_memory_map(0x2000000,...) returned 0x%x\n", result);
		return 0;
	}
	printf("Address: 0x%x\n", addr2);
	
	printf("Unmapping second map.\n");
	result = sys_vm_unmap(addr2);
	if (result != 0)
	{
		printf("ERROR: sys_vm_unmap() returned 0x%x\n", result);
		return 0;
	}
	
	sys_vm_statistics_t stats;
	result = sys_vm_get_statistics(addr, &stats);
	if (result != 0)
	{
		printf("ERROR: sys_vm_get_statistics() returned 0x%x\n", result);
		return 0;
	}
	printf("sys_vm_get_statistics:\n");
	printf("pmem_total: 0x%x\n", stats.pmem_total);
	printf("pmem_used: 0x%x\n", stats.pmem_used);
	
	printf("Appending 512MB in 1MB chunks.\n");
	for (int i = 0; i < 0x200; i++)
	{
		result = sys_vm_append_memory(addr, 0x100000);
		if (result != 0)
		{
			printf("ERROR: sys_vm_append_memory(..., 0x100000) returned 0x%x on iteration %d\n", result, i);
			break;
		}
	}
	
	if (result == 0)
	{
		printf("sys_vm_append_memory(..., 0x100000) never returned an error after 512 iterations\n");
	}
	
	result = sys_vm_get_statistics(addr, &stats);
	if (result != 0)
	{
		printf("ERROR: sys_vm_get_statistics() returned 0x%x\n", result);
		return 0;
	}
	printf("sys_vm_get_statistics:\n");
	printf("pmem_total: 0x%x\n", stats.pmem_total);
	printf("pmem_used: 0x%x\n", stats.pmem_used);
	
	printf("Unmapping.\n");
	result = sys_vm_unmap(addr);
	if (result != 0)
	{
		printf("ERROR: sys_vm_unmap() returned 0x%x\n", result);
		return 0;
	}
	
	printf("Mapping 256MB of virtual memory with 1MB of physical memory used.\n");
	result = sys_vm_memory_map(0x10000000, 0x100000, 0xffffffff, 0x200, 0x1, &addr);
	if (result != 0)
	{
		printf("ERROR: sys_vm_memory_map(0x10000000,...) returned 0x%x\n", result);
		return 0;
	}
	
	result = sys_vm_get_statistics(addr, &stats);
	if (result != 0)
	{
		printf("ERROR: sys_vm_get_statistics() returned 0x%x\n", result);
		return 0;
	}
	printf("sys_vm_get_statistics:\n");
	printf("pmem_total: 0x%x\n", stats.pmem_total);
	printf("pmem_used: 0x%x\n", stats.pmem_used);
	
	printf("Appending 512MB in 1MB chunks.\n");
	for (int i = 0; i < 0x200; i++)
	{
		result = sys_vm_append_memory(addr, 0x100000);
		if (result != 0)
		{
			printf("ERROR: sys_vm_append_memory(..., 0x100000) returned 0x%x on iteration %d\n", result, i);
			break;
		}
	}
	
	if (result == 0)
	{
		printf("sys_vm_append_memory(..., 0x100000) never returned an error after 512 iterations\n");
	}
	
	result = sys_vm_get_statistics(addr, &stats);
	if (result != 0)
	{
		printf("ERROR: sys_vm_get_statistics() returned 0x%x\n", result);
		return 0;
	}
	printf("sys_vm_get_statistics:\n");
	printf("pmem_total: 0x%x\n", stats.pmem_total);
	printf("pmem_used: 0x%x\n", stats.pmem_used);
	
    printf("Sample finished.\n");

    return 0;
}