#include <math/hq_math.h>

void hq_line_line_angle(REAL* pout, const hq_line& line1, const hq_line& line2) {
	REAL dot, length_square1, length_square2;
	hq_dot(&dot, line1, line2);
	hq_length_square(&length_square1, line1);
	hq_length_square(&length_square2, line2);
	REAL cos_line1_line2 = dot / hq_sqrt(length_square1 * length_square2);
	*pout = hq_acos(cos_line1_line2);
}

void hq_plane_normal(hq_vertex3* pout, const hq_plane& plane) {
	pout->x = plane.x;
	pout->y = plane.y;
	pout->z = plane.z; 
}

BOOLEAN hq_plane_intersect(hq_line* pout, const hq_plane& plane1, const hq_plane& plane2) {
	hq_vertex3 normal1, normal2;
	hq_plane_normal(&normal1, plane1);
	hq_plane_normal(&normal2, plane2);
	return TRUE;
}