#ifndef _HQVERTEX_H_
#define _HQVERTEX_H_

#include <hq_type.h>

#pragma pack(16)
struct hq_vertex4 {
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
