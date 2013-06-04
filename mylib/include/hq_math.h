#ifndef _HQMATH_H_
#define _HQMATH_H_

/*
 * all the functions in the file is following rules like this:
 * func(aout, a1) -> aout = func a1, for example: minus(aout, a1) -> aout = - a1
 * func(aout, a1, a2) -> aout = a1 func a2, for example: sub(aout, a1, a2) -> aout = a1 - a2
 */

#include <hq_vertex.h>
#include <hq_matrix.h>
////////////////////////////////////////////////////////////////////////////////
// vertex4

//one operand
void hq_minus(hq_vertex4* parg_out);
void hq_abs(hq_vertex4* parg_out);
void hq_normalize(hq_vertex4* parg_out);

//two operands
void hq_minus(hq_vertex4* parg_out, const hq_vertex4& arg_1);
void hq_length(REAL* parg_out, const hq_vertex4& arg_1);
void hq_normalize(hq_vertex4* parg_out, const hq_vertex4& arg_1);

//three operands
void hq_add(hq_vertex4* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2);
void hq_sub(hq_vertex4* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2);
void hq_dot(REAL* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2);
void hq_cross(hq_vertex4* parg_out, const hq_vertex4& arg_1, const hq_vertex4& arg_2);

////////////////////////////////////////////////////////////////////////////////
// matrix4x4

//one operand
void hq_entity(hq_matrix4x4* parg_out);

//two operands

//three operands
void hq_add(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2);
void hq_sub(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2);
void hq_mul(hq_matrix4x4* parg_out, const hq_matrix4x4& arg_1, const hq_matrix4x4& arg_2);

#endif//_HQMATH_H_
