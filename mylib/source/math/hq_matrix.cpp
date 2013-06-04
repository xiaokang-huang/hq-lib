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
		parg_out->row0[i] = arg_1.row0[i] + arg_2.row0[i];
		parg_out->row1[i] = arg_1.row1[i] + arg_2.row1[i];
		parg_out->row2[i] = arg_1.row2[i] + arg_2.row2[i];
		parg_out->row3[i] = arg_1.row3[i] + arg_2.row3[i];
	}
}

void hq_sub(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2) {
	for (int i = 0; i < 4; ++i) {
		parg_out->row0[i] = arg_1.row0[i] - arg_2.row0[i];
		parg_out->row1[i] = arg_1.row1[i] - arg_2.row1[i];
		parg_out->row2[i] = arg_1.row2[i] - arg_2.row2[i];
		parg_out->row3[i] = arg_1.row3[i] - arg_2.row3[i];
	}
}

void hq_mul(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2) {
	for (int row = 0; row < 4; ++ row) {
		for (int col = 0; col < 4; ++ col) {
			parg_out->data[row][col] = arg_1.data[row][0] * arg_2.data[0][col]
			                         + arg_1.data[row][1] * arg_2.data[1][col]
			                         + arg_1.data[row][2] * arg_2.data[2][col]
			                         + arg_1.data[row][3] * arg_2.data[3][col];
		}
	}
}

