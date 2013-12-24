#include <hq_type.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

UINT32 GetProcessNum () {
	return (UINT32)sysconf(_SC_NPROCESSORS_CONF);
}

INT32 hq_open(const char* path, UINT32 flag) {
	int internal_flag = 0;
	if (flag & (HQOPEN_FLAG_R | HQOPEN_FLAG_W) == (HQOPEN_FLAG_R | HQOPEN_FLAG_W)) {
		internal_flag = O_RDWR;
	} else if (flag & HQOPEN_FLAG_R) {
		internal_flag = O_RDONLY;
	} else if (flag & HQOPEN_FLAG_W) {
		internal_flag = O_WRONLY;
	}
	if (flag & HQOPEN_FLAG_A) {
		internal_flag |= O_APPEND;
	}
	if (flag & HQOPEN_FLAG_C) {
		internal_flag |= O_CREAT;
	}

	return (INT32)open(path, internal_flag);
}

void hq_close(INT32 handle) {
	close((int)handle);
}

HQOFFSET hq_seek(INT32 handle, HQOFFSET offset, UINT32 where) {
	const static int tab[] = {
		SEEK_SET,	SEEK_CUR, SEEK_END
	};
	return lseek(handle, offset, tab[where]);
}

_SIZE hq_read(INT32 handle, void* buf, _SIZE count) {
	return read(handle, buf, count);
}

_SIZE hq_write(INT32 handle, const void* buf, _SIZE count) {
	return write(handle, buf, count);
}
