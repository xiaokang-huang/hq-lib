#include <math/hq_math.h>
#include <hq_os.h>
#include <hq_type.h>

// set function
void hq_setvertex(hq_vertex2* parg_out, REAL x, REAL y) {
	parg_out->x = x;
	parg_out->y = y;
}

void hq_setvertex(hq_vertex3* parg_out, REAL x, REAL y, REAL z) {
	parg_out->x = x;
	parg_out->y = y;
	parg_out->z = z;
}

void hq_setvertex(hq_vertex4* parg_out, REAL x, REAL y, REAL z, REAL w) {
	parg_out->x = x;
	parg_out->y = y;
	parg_out->z = z;
	parg_out->w = w;
}

////////////////////////////////////////////////////////////////////////////////
// one operand
void hq_minus(hq_vertex2* parg_out) {
	parg_out->x = - parg_out->x;
	parg_out->y = - parg_out->y;
}

void hq_minus(hq_vertex3* parg_out) {
	parg_out->x = - parg_out->x;
	parg_out->y = - parg_out->y;
	parg_out->z = - parg_out->z;
}

void hq_minus(hq_vertex4* parg_out) {
	parg_out->x = - parg_out->x;
	parg_out->y = - parg_out->y;
	parg_out->z = - parg_out->z;
}

void hq_abs(hq_vertex2* parg_out) {
	parg_out->x = abs(parg_out->x);
	parg_out->y = abs(parg_out->y);
}

void hq_abs(hq_vertex3* parg_out) {
	parg_out->x = abs(parg_out->x);
	parg_out->y = abs(parg_out->y);
	parg_out->z = abs(parg_out->z);
}

void hq_abs(hq_vertex4* parg_out) {
	parg_out->x = abs(parg_out->x);
	parg_out->y = abs(parg_out->y);
	parg_out->z = abs(parg_out->z);
}

void hq_normalize(hq_vertex2* parg_out) {
	REAL inv_sqrt;
	hq_length(&inv_sqrt, *parg_out);
	inv_sqrt = 1 / inv_sqrt;
	parg_out->x *= inv_sqrt;
	parg_out->y *= inv_sqrt;
}

void hq_normalize(hq_vertex3* parg_out) {
	REAL inv_sqrt;
	hq_length(&inv_sqrt, *parg_out);
	inv_sqrt = 1 / inv_sqrt;
	parg_out->x *= inv_sqrt;
	parg_out->y *= inv_sqrt;
	parg_out->z *= inv_sqrt;
}

void hq_normalize(hq_vertex4* parg_out) {
	REAL inv_sqrt;
	hq_length(&inv_sqrt, *parg_out);
	inv_sqrt = 1 / inv_sqrt;
	parg_out->x *= inv_sqrt;
	parg_out->y *= inv_sqrt;
	parg_out->z *= inv_sqrt;
}

////////////////////////////////////////////////////////////////////////////////
// two operands
void hq_minus(hq_vertex2* parg_out, const hq_vertex2& arg_1) {
	parg_out->x = - arg_1.x;
	parg_out->y = - arg_1.y;
}

void hq_minus(hq_vertex3* parg_out, const hq_vertex3& arg_1) {
	parg_out->x = - arg_1.x;
	parg_out->y = - arg_1.y;
	parg_out->z = - arg_1.z;
}

void hq_minus(hq_vertex4* parg_out, const hq_vertex4& arg_1) {
	parg_out->x = - arg_1.x;
	parg_out->y = - arg_1.y;
	parg_out->z = - arg_1.z;
}

void hq_length(REAL* parg_out, const hq_vertex2& arg_1) {
	*parg_out = sqrt(arg_1.x * arg_1.x + arg_1.y * arg_1.y);
}

void hq_length(REAL* parg_out, const hq_vertex3& arg_1) {
	*parg_out = sqrt(arg_1.x * arg_1.x + arg_1.y * arg_1.y + arg_1.z * arg_1.z);
}

void hq_length(REAL* parg_out, const hq_vertex4& arg_1) {
	*parg_out = sqrt(arg_1.x * arg_1.x + arg_1.y * arg_1.y + arg_1.z * arg_1.z);
}

void hq_normalize(hq_vertex2* parg_out, const hq_vertex2& arg_1) {
	REAL inv_sqrt;
	hq_length(&inv_sqrt, arg_1);
	inv_sqrt = 1 / inv_sqrt;
	parg_out->x = arg_1.x * inv_sqrt;
	parg_out->y = arg_1.y * inv_sqrt;
}

void hq_normalize(hq_vertex3* parg_out, const hq_vertex3& arg_1) {
	REAL inv_sqrt;
	hq_length(&inv_sqrt, arg_1);
	inv_sqrt = 1 / inv_sqrt;
	parg_out->x = arg_1.x * inv_sqrt;
	parg_out->y = arg_1.y * inv_sqrt;
	parg_out->z = arg_1.z * inv_sqrt;
}

void hq_normalize(hq_vertex4* parg_out, const hq_vertex4& arg_1) {
	REAL inv_sqrt;
	hq_length(&inv_sqrt, arg_1);
	inv_sqrt = 1 / inv_sqrt;
	parg_out->x = arg_1.x * inv_sqrt;
	parg_out->y = arg_1.y * inv_sqrt;
	parg_out->z = arg_1.z * inv_sqrt;
}

////////////////////////////////////////////////////////////////////////////////
// three operands
void hq_add(hq_vertex2* parg_out, const hq_vertex2& arg_1, const hq_vertex2& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.x + arg_2.x;
	parg_out->y = arg_1.y + arg_2.y;
}

void hq_add(hq_vertex3* parg_out, const hq_vertex3& arg_1, const hq_vertex3& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.x + arg_2.x;
	parg_out->y = arg_1.y + arg_2.y;
	parg_out->z = arg_1.z + arg_2.z;
}

void hq_add(hq_vertex4* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.x + arg_2.x;
	parg_out->y = arg_1.y + arg_2.y;
	parg_out->z = arg_1.z + arg_2.z;
	//parg_out->w = arg_1.w + arg_2.w;
}

void hq_sub(hq_vertex2* parg_out, const hq_vertex2& arg_1, const hq_vertex2& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.x - arg_2.x;
	parg_out->y = arg_1.y - arg_2.y;
}

void hq_sub(hq_vertex3* parg_out, const hq_vertex3& arg_1, const hq_vertex3& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.x - arg_2.x;
	parg_out->y = arg_1.y - arg_2.y;
	parg_out->z = arg_1.z - arg_2.z;
}

void hq_sub(hq_vertex4* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.x - arg_2.x;
	parg_out->y = arg_1.y - arg_2.y;
	parg_out->z = arg_1.z - arg_2.z;
	//parg_out->w = arg_1.w - arg_2.w;
}

void hq_dot(REAL* parg_out, const hq_vertex2& arg_1, const hq_vertex2& arg_2) {
	ASSERT(parg_out != NULL);
	*parg_out = arg_1.x * arg_2.x + arg_1.y * arg_2.y;
}

void hq_dot(REAL* parg_out, const hq_vertex3& arg_1, const hq_vertex3& arg_2) {
	ASSERT(parg_out != NULL);
	*parg_out = arg_1.x * arg_2.x + arg_1.y * arg_2.y + arg_1.z * arg_2.z;
}

void hq_dot(REAL* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2) {
	ASSERT(parg_out != NULL);
	*parg_out = arg_1.x * arg_2.x + arg_1.y * arg_2.y + arg_1.z * arg_2.z;
}

void hq_cross(hq_vertex4* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2) {
	ASSERT(parg_out != NULL);
	parg_out->x = arg_1.y * arg_2.z - arg_1.z * arg_2.y;
	parg_out->y = arg_1.z * arg_2.x - arg_1.x * arg_2.z;
	parg_out->z = arg_1.x * arg_2.y - arg_1.y * arg_2.x;
}

