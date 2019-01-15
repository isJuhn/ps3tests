#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ppu_thread.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/vm.h>
#include <sys/timer.h>
#include <sys/syscall.h>
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

#define int_cast(addr) reinterpret_cast<uptr>(addr)
#define ptr_cast(intnum) reinterpret_cast<void*>(intnum)
#define ptr_caste(intnum, type) reinterpret_cast<type*>(ptr_cast(intnum))

#define GAMEDATADIRNAME		"UTGD00000"
#define SAMPLE_CREATE_PARAMSFO_TITLE				"SAMPLE CREATE GAME"
#define SAMPLE_CREATE_PARAMSFO_TITLEID				"UTGD00000"
#define SAMPLE_CREATE_PARAMSFO_VERSION				"01.00"
#define CREATE_FLAGS CELL_FS_O_RDWR | CELL_FS_O_CREAT
#define CREATE_MODE CELL_FS_S_IRUSR | CELL_FS_S_IWUSR

#define ERROR_CHECK(x...) if (result < 0) {printf(x);}
#define ERROR_CHECK_RET(x...) if (result < 0) {printf(x); return 0;}

register u32 error asm ("3"); 

u32 sys_fs_open(char* path, s32 flags, u32* fd, s32 mode, void* arg, u64 size)
{
	{system_call_6(0x321, int_cast(path), CREATE_FLAGS, int_cast(fd), CREATE_MODE, 0x0, 0x0);}
	return error;
}

u32 sys_fs_close(u32 fd)
{
	{system_call_1(0x324, fd);}
	return error;
}

u32 sys_fs_unlink(char* path)
{
	{system_call_1(0x32E, int_cast(path));}
	return error;
}

u32 sys_fs_stat(char* path, CellFsStat* stat)
{
	{system_call_2(0x328, int_cast(path), int_cast(stat));}
	return error;
}

u32 sys_fs_fstat(u32 fd, CellFsStat* stat)
{
	{system_call_2(0x329, fd, int_cast(stat));}
	return error;
}

u32 sys_fs_rename(char* from, char* to)
{
	{system_call_2(0x32C, int_cast(from), int_cast(to));}
	return error;
}

u32 sys_fs_write(u32 fd, char* buf, u64 nbytes, u64* nwrite)
{
	{system_call_4(0x323, fd, int_cast(buf), nbytes, int_cast(nwrite));}
	return error;
}

u32 sys_fs_read(u32 fd, char* buf, u64 nbytes, u64* nwrite)
{
	{system_call_4(0x322, fd, int_cast(buf), nbytes, int_cast(nwrite));}
	return error;
}

int main() {

	int result;
	
	printf("Sample started.\n");
	
	result = cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_GAME );
	ERROR_CHECK_RET("ERROR : cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_GAME ) = 0x%x\n", result)
	
	char* dirName = (char*)GAMEDATADIRNAME;
	int type = CELL_GAME_GAMETYPE_GAMEDATA;
	CellGameContentSize size;
	memset(&size, 0x00, sizeof(CellGameContentSize));
	printf("cellGameDataCheck()\n");
	result = cellGameDataCheck(type, dirName, &size);
	ERROR_CHECK_RET("ERROR: cellGameDataCheck returned 0x%x\n", result)
	
	char tmp_contentInfoPath[CELL_GAME_PATH_MAX];
	char tmp_usrdirPath[CELL_GAME_PATH_MAX];
	CellGameSetInitParams init;
	memset(&init, 0x00, sizeof(init));
	strncpy(init.title,   SAMPLE_CREATE_PARAMSFO_TITLE, CELL_GAME_SYSP_TITLE_SIZE - 1);
	strncpy(init.titleId, SAMPLE_CREATE_PARAMSFO_TITLEID, CELL_GAME_SYSP_TITLEID_SIZE - 1);
	strncpy(init.version, SAMPLE_CREATE_PARAMSFO_VERSION, CELL_GAME_SYSP_VERSION_SIZE - 1);
	
	printf("cellGameCreateGameData()\n");
	result = cellGameCreateGameData(&init, tmp_contentInfoPath, tmp_usrdirPath);
	ERROR_CHECK_RET("ERROR: cellGameCreateGameData returned 0x%x\n", result)
	printf("tmp_contentInfoPath: %s\n", tmp_contentInfoPath);
	printf("tmp_usrdirPath: %s\n", tmp_usrdirPath);
	
	char file_path[CELL_GAME_PATH_MAX];
	size_t len = strlen(tmp_usrdirPath);
	strncpy(file_path, tmp_usrdirPath, len);
	strncpy(file_path + len, "/kot.a\0", 7);
	
	printf("file_path: %s\n", file_path);
	
	u32 fd;
	u32 fd2;
	printf("Creating file\n");
	result = sys_fs_open(file_path, CREATE_FLAGS, &fd, CREATE_MODE, 0x0, 0x0);
	ERROR_CHECK_RET("ERROR: sys_fs_open(\"%s\") returned 0x%x\n", file_path, result)
	
	printf("Opening file again\n");
	result = sys_fs_open(file_path, CREATE_FLAGS, &fd2, CREATE_MODE, 0x0, 0x0);
	ERROR_CHECK_RET("ERROR: sys_fs_open(\"%s\") returned 0x%x\n", file_path, result)
	
	printf("File FDs: 0x%x, 0x%x\n", fd, fd2);
	
	printf("fstat first FD\n");
	CellFsStat stat;
	result = sys_fs_fstat(fd, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd, result)
	
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
	
	u64 nwrite;
	printf("Write to file\n");
	result = sys_fs_write(fd, file_path, len, &nwrite);
	ERROR_CHECK("ERROR: sys_fs_write(0x%x) returned 0x%x\n", fd, result)
	
	printf("Unlinking file\n");
	result = sys_fs_unlink(file_path);
	ERROR_CHECK_RET("ERROR: sys_fs_unlink(\"%s\") returned 0x%x\n", file_path, result)
	
	printf("fstat both FDs\n");
	result = sys_fs_fstat(fd, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd, result)
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
	
	result = sys_fs_fstat(fd2, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd2, result)
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
	
	printf("stat path\n");
	result = sys_fs_stat(file_path, &stat);
	ERROR_CHECK("ERROR: sys_fs_stat(\"%s\") returned 0x%x\n", file_path, result)
	
	printf("Write to file\n");
	result = sys_fs_write(fd, file_path, len, &nwrite);
	ERROR_CHECK("ERROR: sys_fs_write(0x%x) returned 0x%x\n", fd, result)
	printf("nwrite: 0x%x\n", nwrite);
	
	printf("stat path\n");
	result = sys_fs_stat(file_path, &stat);
	ERROR_CHECK("ERROR: sys_fs_stat(\"%s\") returned 0x%x\n", file_path, result)
	
	printf("fstat both FDs\n");
	result = sys_fs_fstat(fd, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd, result)
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
		
	result = sys_fs_fstat(fd2, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd2, result)
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
	
	char buf[len * 2];
	printf("Read FD\n");
	result = sys_fs_read(fd, buf, len * 2, &nwrite);
	ERROR_CHECK("ERROR: sys_fs_read(0x%x) returned 0x%x\n", fd);
	printf("nwrite: 0x%x %s\n", nwrite, buf);
	
	printf("Writing 65k times\n");
	for (s32 i = 0; i < 0x10000; ++i)
	{
		result = sys_fs_fstat(fd, &stat);
		result = sys_fs_write(fd, file_path, len, &nwrite);
		if (result != 0)
		{
			printf("ERROR: sys_fs_write(0x%x) returned 0x%x\n on iteration 0x%x", fd, result, i);
			printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
			break;
		}
	}
	
	printf("Sleeping 3 sec\n");
	sys_timer_sleep(3);
	
	printf("Write to file\n");
	result = sys_fs_write(fd, file_path, len, &nwrite);
	ERROR_CHECK("ERROR: sys_fs_write(0x%x) returned 0x%x\n", fd, result)
	printf("nwrite: 0x%x\n", nwrite);
	
	printf("Close FD\n");
	result = sys_fs_close(fd);
	ERROR_CHECK("ERROR: sys_fs_close(0x%x)", fd);
	
	printf("stat path\n");
	result = sys_fs_stat(file_path, &stat);
	ERROR_CHECK("ERROR: sys_fs_stat(\"%s\") returned 0x%x\n", file_path, result)
	
	printf("fstat both FDs\n");
	result = sys_fs_fstat(fd, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd, result)
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
		
	result = sys_fs_fstat(fd2, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd2, result)
	printf("Stat: mode=0x%x, uid=0x%x, gid=0x%x, atime=0x%x, mtime=0x%x, ctime=0x%x, size=0x%x, blksize=0x%x\n", stat.st_mode, stat.st_uid, stat.st_gid, stat.st_atime, stat.st_mtime, stat.st_ctime, stat.st_size, stat.st_blksize);
	
	printf("Creating file\n");
	result = sys_fs_open(file_path, CREATE_FLAGS, &fd, CREATE_MODE, 0x0, 0x0);
	ERROR_CHECK_RET("ERROR: sys_fs_open(\"%s\") returned 0x%x\n", file_path, result)
	
	char new_path[CELL_GAME_PATH_MAX];
	len = strlen(file_path);
	strncpy(new_path, file_path, len + 1);
	new_path[len - 1] = '2';
	printf("new_path: %s\n", new_path);
	
	printf("Renaming file\n");
	result = sys_fs_rename(file_path, new_path);
	ERROR_CHECK_RET("ERROR: sys_fs_rename(\"%s\", \"%s\") returned 0x%x\n", file_path, new_path, result)
	
	printf("fstat FD\n");
	result = sys_fs_fstat(fd, &stat);
	ERROR_CHECK("ERROR: sys_fs_fstat(0x%x) returned 0x%x\n", fd, result)
	
	printf("stat old and new path\n");
	result = sys_fs_stat(file_path, &stat);
	ERROR_CHECK("ERROR: sys_fs_stat(\"%s\") returned 0x%x\n", file_path, result)
	
	result = sys_fs_stat(new_path, &stat);
	ERROR_CHECK("ERROR: sys_fs_stat(\"%s\") returned 0x%x\n", new_path, result)
	
    printf("Sample finished.\n");

    return 0;
}
