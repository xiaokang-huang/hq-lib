#ifndef _HQGEOMETRY_H_
#define _HQGEOMETRY_H_


#include <math/hq_math.h>
/* --------------------------------------------------
// data define
-------------------------------------------------- */
/*
 * plane is defined as follows:
 * ax + by + cz + d = 0;
 * where a, b, c is the normal
 * in vertex, a = x, b = y, c = z, d = w
 */
typedef hq_vertex4 hq_plane;

/*
 * line is defined by normal a, b, c
 * in vertex, a = x, b = y, c = z
 * w is always 0
 */
typedef hq_vertex4 hq_line;

/*
 * point is defined by normal a, b, c
 * in vertex, a = x, b = y, c = z
 * w is always > 0
 */
typedef hq_vertex4 hq_point;

/* --------------------------------------------------
// function define
-------------------------------------------------- */
void hq_line_line_angle(REAL* pout, const hq_line& line1, const hq_line& line2);

void	hq_plane_normal(hq_vertex3* pout, const hq_plane& plane);
BOOLEAN	hq_plane_intersect(hq_line* pout, const hq_plane& plane1, const hq_plane& plane2);



#endif//_HQGEOMETRY_H_
