#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ppu_thread.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/vm.h>
#include <sys/paths.h>
#include <cell/sysmodule.h>

#include <np.h>

// Set priority and stack size for the primary PPU thread.
// Priority : 1000
// Stack    : 1MB
SYS_PROCESS_PARAM(1000, 0x100000)

typedef uintptr_t uptr;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t s32;

#define NP_POOL_SIZE (128*1024)
#define SCE_NP_COOKIE_MAX_SIZE (1024)
#define TICKET_SERVICE_ID		"IV0002-NPXS00001_00"

int main() {

	int result;
	u8 np_pool[NP_POOL_SIZE]; 
	
	printf("Sample started.\n");
	
	result = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP);
	if (result < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP) failed (0x%x)\n", result);
		return 0;
	}
	
	result = cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP2);
	if (result < 0) {
		printf("cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_NP2) failed (0x%x)\n", result);
		return 0;
	}
	
	printf("sceNp2Init()\n");
	result = sceNp2Init(NP_POOL_SIZE, np_pool);
	if (result < 0)
	{
		printf("ERROR: sceNp2Init() returned 0x%x\n", result);
		return 0;
	}
	
	SceNpId _npId;
	printf("sceNpManagerGetNpId()\n");
	result = sceNpManagerGetNpId(&_npId);
	if (result < 0)
	{
		printf("ERROR: sceNpManagerGetNpId() returned 0x%x\n", result);
	}
	printf("_npId.handle.data=%s\n", _npId.handle.data);
	
	SceNpTicketVersion ticketVersion;
	ticketVersion.major = 3;
	ticketVersion.minor = 0;
	printf("sceNpManagerRequestTicket2()\n");
	result = sceNpManagerRequestTicket2(&_npId, &ticketVersion, TICKET_SERVICE_ID, NULL, 0, NULL, 0);
	if (result < 0)
	{
		printf("ERROR: sceNpManagerRequestTicket2() returned 0x%x\n", result);
	}
	
    printf("Sample finished.\n");

    return 0;
}
