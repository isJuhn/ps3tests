#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ppu_thread.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/vm.h>
#include <sysutil/sysutil_gamecontent.h>
#include <sys/paths.h>
#include <cell/sysmodule.h>

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

#define GAMEDATADIRNAME		"UTGD00000"
#define SAMPLE_CREATE_PARAMSFO_TITLE				"SAMPLE CREATE GAME"
#define SAMPLE_CREATE_PARAMSFO_TITLEID				"UTGD00000"
#define SAMPLE_CREATE_PARAMSFO_VERSION				"01.00"

int main() {

	int result;
	
	printf("Sample started.\n");
	
	result = cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_GAME );
	if ( result < 0 ) {
		printf( "ERROR : cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_GAME ) = 0x%x\n", result );
		return 0;
	}
	
	char* dirName = (char*)GAMEDATADIRNAME;
	int type = CELL_GAME_GAMETYPE_GAMEDATA;
	CellGameContentSize size;
	memset(&size, 0x00, sizeof(CellGameContentSize));
	printf("cellGameDataCheck()\n");
	result = cellGameDataCheck(type, dirName, &size);
	if (result < 0)
	{
		printf("ERROR: cellGameDataCheck returned 0x%x\n", result);
		return 0;
	}
	
	char tmp_contentInfoPath[CELL_GAME_PATH_MAX];
	char tmp_usrdirPath[CELL_GAME_PATH_MAX];
	CellGameSetInitParams init;
	memset(&init, 0x00, sizeof(init));
	strncpy(init.title,   SAMPLE_CREATE_PARAMSFO_TITLE, CELL_GAME_SYSP_TITLE_SIZE - 1);
	strncpy(init.titleId, SAMPLE_CREATE_PARAMSFO_TITLEID, CELL_GAME_SYSP_TITLEID_SIZE - 1);
	strncpy(init.version, SAMPLE_CREATE_PARAMSFO_VERSION, CELL_GAME_SYSP_VERSION_SIZE - 1);
	printf("cellGameCreateGameData()\n");
	result = cellGameCreateGameData(&init, tmp_contentInfoPath, tmp_usrdirPath);
	if (result < 0)
	{
		printf("ERROR: cellGameCreateGameData returned 0x%x\n", result);
		return 0;
	}
	printf("tmp_contentInfoPath: %s\n", tmp_contentInfoPath);
	printf("tmp_usrdirPath: %s\n", tmp_usrdirPath);
	
    printf("Sample finished.\n");

    return 0;
}
