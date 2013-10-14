#include <math/hq_math.h>
#include <hq_os.h>
#include <hq_type.h>

////////////////////////////////////////////////////////////////////////////////
// one operand
void hq_entity(hq_matrix4x4* parg_out) {
	parg_out->m00 = 1;	parg_out->m01 = 0;	parg_out->m02 = 0;	parg_out->m03 = 0;
	parg_out->m10 = 0;	parg_out->m11 = 1;	parg_out->m12 = 0;	parg_out->m13 = 0;
	parg_out->m20 = 0;	parg_out->m21 = 0;	parg_out->m22 = 1;	parg_out->m23 = 0;
	parg_out->m30 = 0;	parg_out->m31 = 0;	parg_out->m32 = 0;	parg_out->m33 = 1;
}

////////////////////////////////////////////////////////////////////////////////
// two operands

////////////////////////////////////////////////////////////////////////////////
// three operands
void hq_add(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2) {
	for (int i = 0; i < 4; ++i) {
		parg_out->col0[i] = arg_1.col0[i] + arg_2.col0[i];
		parg_out->col1[i] = arg_1.col1[i] + arg_2.col1[i];
		parg_out->col2[i] = arg_1.col2[i] + arg_2.col2[i];
		parg_out->col3[i] = arg_1.col3[i] + arg_2.col3[i];
	}
}

void hq_sub(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2) {
	for (int i = 0; i < 4; ++i) {
		parg_out->col0[i] = arg_1.col0[i] - arg_2.col0[i];
		parg_out->col1[i] = arg_1.col1[i] - arg_2.col1[i];
		parg_out->col2[i] = arg_1.col2[i] - arg_2.col2[i];
		parg_out->col3[i] = arg_1.col3[i] - arg_2.col3[i];
	}
}

void hq_mul(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2) {
	for (int i = 0; i < 4; i++) {
 		parg_out->data[i][0] = arg_1.data[i][0]*arg_2.data[0][0] + arg_1.data[i][1]*arg_2.data[1][0] + arg_1.data[i][2]*arg_2.data[2][0] + arg_1.data[i][3]*arg_2.data[3][0];
		parg_out->data[i][1] = arg_1.data[i][0]*arg_2.data[0][1] + arg_1.data[i][1]*arg_2.data[1][1] + arg_1.data[i][2]*arg_2.data[2][1] + arg_1.data[i][3]*arg_2.data[3][1];
		parg_out->data[i][2] = arg_1.data[i][0]*arg_2.data[0][2] + arg_1.data[i][1]*arg_2.data[1][2] + arg_1.data[i][2]*arg_2.data[2][2] + arg_1.data[i][3]*arg_2.data[3][2];
		parg_out->data[i][3] = arg_1.data[i][0]*arg_2.data[0][3] + arg_1.data[i][1]*arg_2.data[1][3] + arg_1.data[i][2]*arg_2.data[2][3] + arg_1.data[i][3]*arg_2.data[3][3];
	}
}

void hq_mul(hq_vertex4* parg_out, const hq_matrix4x4& arg_1, const hq_vertex4& arg_2) {
	parg_out->x = arg_1.m00 * arg_2.x + arg_1.m01 * arg_2.y + arg_1.m02 * arg_2.z + arg_1.m03 * arg_2.w;
	parg_out->y = arg_1.m10 * arg_2.x + arg_1.m11 * arg_2.y + arg_1.m12 * arg_2.z + arg_1.m13 * arg_2.w;
	parg_out->z = arg_1.m20 * arg_2.x + arg_1.m21 * arg_2.y + arg_1.m22 * arg_2.z + arg_1.m23 * arg_2.w;
	parg_out->w = arg_1.m30 * arg_2.x + arg_1.m31 * arg_2.y + arg_1.m32 * arg_2.z + arg_1.m33 * arg_2.w;
}
