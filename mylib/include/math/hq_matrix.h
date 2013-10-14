#ifndef _HQMATRIX_H_
#define _HQMATRIX_H_

#include <hq_type.h>

/*
// note: the matrix is stored in column-major
*/

#pragma pack(16)
struct hq_matrix4x4 {
	union {
		REAL raw[16];
		REAL data[4][4];
		struct {
			REAL m00;	REAL m10;	REAL m20;	REAL m30;
			REAL m01;	REAL m11;	REAL m21;	REAL m31;
			REAL m02;	REAL m12;	REAL m22;	REAL m32;
			REAL m03;	REAL m13;	REAL m23;	REAL m33;
		};
		struct {
			REAL col0[4];
			REAL col1[4];
			REAL col2[4];
			REAL col3[4];
		};
	};
};
#pragma pack()

#endif//_HQMATRIX_H_
