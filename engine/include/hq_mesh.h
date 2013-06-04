#ifndef _HQMESH_H_
#define _HQMESH_H_

#include <hq_vertex.h>
#include <hq_memory_mgr.h>

#include <hq_type.h>

class HQMeshBase : PUBLIC MemoryManagedBase {
PUBLIC:
	virtual ~HQMeshBase() {}
	virtual hq_vertex4* GetVertexBuffer()	= 0;
	virtual UINT32		GetVertexNum()		= 0;
	virtual UINT32*		GetIndexBuffer()	= 0;
	virtual UINT32		GetIndexNum()		= 0;
};

#endif//_HQMESH_H_
