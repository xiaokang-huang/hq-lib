#ifndef _HQVERTEX_H_
#define _HQVERTEX_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>

struct hq_vertex2 : PUBLIC MemoryAllocatorBase {
	union {
		REAL data[2];
		struct {
			REAL x;
			REAL y;
		};
		struct {
			REAL u;
			REAL v;
		};
	};
};

struct hq_vertex3 : PUBLIC MemoryAllocatorBase {
	union {
		REAL data[3];
		struct {
			REAL x;
			REAL y;
			REAL z;
		};
	};
};

#pragma pack(16)
struct hq_vertex4 : PUBLIC MemoryAllocatorBase {
	union {
		REAL data[4];
		struct {
			REAL x;
			REAL y;
			REAL z;
			REAL w;
		};
	};
};
#pragma pack()

#endif//_HQVERTEX_H_
