#ifndef _HQMESH_H_
#define _HQMESH_H_

#include <math/hq_vertex.h>
#include <hq_memory_mgr.h>
#include <hq_type.h>

#define MAXTEXTUREPLAN	8

class HQMeshBase {
PUBLIC:
	virtual ~HQMeshBase() {}
	virtual hq_vertex4* GetVertexBuffer()	= 0;
	virtual hq_vertex4*	GetColorBuffer()	= 0;
	virtual hq_vertex3* GetNormalBuffer()	= 0;
	virtual hq_vertex2* GetTexcordBuffer(UINT32 idx) = 0;
	virtual UINT32*		GetIndexBuffer()	= 0;
	virtual UINT32		GetVertexNum()		= 0;
	virtual UINT32		GetIndexNum()		= 0;
};

class HQMeshFactory {
PUBLIC:
	struct Info {
		UINT32 nTracerIdx;
	};
PUBLIC:
	static HQMeshBase*	CreateStaticMeshCube(Info* info);
	static void DestroyMesh(HQMeshBase* pMesh);
};

#endif//_HQMESH_H_
