#ifndef _HQMEMPOOL_H_
#define _HQMEMPOOL_H_

#include <hq_type.h>

class HQMemPool {
PUBLIC:
	HQMemPool();

	void*	MemAlloc(_SIZE size);
	void	MemFree(void* ptr);
PRIVATE:
	void initialize_pool();
PRIVATE:
	struct pool_header {
		UINTSYS	magic_num;
	};
};

#endif//_HQMEMPOOL_H_
