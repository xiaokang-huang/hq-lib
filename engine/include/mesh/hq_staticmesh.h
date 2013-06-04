#ifndef _HQSTATICMESH_H_
#define _HQSTATICMESH_H_

#include <mesh/hq_meshbase.h>

class HQStaticMesh : PUBLIC HQMeshBase, PUBLIC MemoryManagedBase {
PUBLIC:
	struct Info {
		UINT32 nTracerIdx;
	};
	friend class HQMeshFactory;
PUBLIC:
	HQStaticMesh(const Info* info);
	virtual ~HQStaticMesh();
PUBLIC:
	virtual hq_vertex4* GetVertexBuffer()	{	return m_pvertexbuffer;		}
	virtual hq_vertex4*	GetColorBuffer()	{	return m_pcolorbuffer;		}
	virtual hq_vertex2* GetTexcordBuffer(UINT32 idx)	{	return m_ptexcordbuffer[idx];	}
	virtual hq_vertex3* GetNormalBuffer()	{	return m_pnormalbuffer;		}
	virtual UINT32*		GetIndexBuffer()	{	return m_pindexbuffer;		}
	virtual UINT32		GetVertexNum()		{	return m_nvertexnum;		}
	virtual UINT32		GetIndexNum()		{	return m_nindexnum;			}

PRIVATE:
	void finalize();
PRIVATE:
	hq_vertex4*	m_pvertexbuffer;
	hq_vertex4*	m_pcolorbuffer;
	hq_vertex3* m_pnormalbuffer;
	hq_vertex2* m_ptexcordbuffer[MAXTEXTUREPLAN];
	UINT32*		m_pindexbuffer;
	UINT32		m_nvertexnum;
	UINT32		m_nindexnum;
};

#endif//_HQSTATICMESH_H_
