#ifndef _HQBOUNDINGBOX_H_
#define _HQBOUNDINGBOX_H_

#include <math/hq_vertex.h>
#include <hq_memory_mgr.h>

struct HQBoundingBox : PUBLIC MemoryAllocatorBase {
	hq_vertex3 m_vMin;
	hq_vertex3 m_vMax;

	HQBoundingBox () {
		m_vMin.x = -1;	m_vMin.y = -1;	m_vMin.z = -1;
		m_vMax.x = 1;	m_vMax.y = 1;	m_vMax.z = 1;
	}
};

#endif//_HQBOUNDINGBOX_H_
