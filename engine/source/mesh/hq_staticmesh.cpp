#include <mesh/hq_staticmesh.h>
#include <hq_errorcode.h>
#include <math/hq_math.h>

void HQStaticMesh::finalize() {
	delete[] m_pvertexbuffer;	m_pvertexbuffer = NULL;
	delete[] m_pcolorbuffer;	m_pcolorbuffer  = NULL;
	delete[] m_pnormalbuffer;	m_pnormalbuffer = NULL;
	for (int i = 0; i < MAXTEXTUREPLAN; ++i) {
		delete[] m_ptexcordbuffer[i];
		m_ptexcordbuffer[i] = NULL;
	}
	MemFree(m_pindexbuffer);	m_pindexbuffer = NULL;
	m_nvertexnum = 0;
	m_nindexnum = 0;
}

HQStaticMesh::HQStaticMesh(const Info* info) : MemoryManagedBase(info->nTracerIdx),
	m_pvertexbuffer(NULL),
	m_pcolorbuffer(NULL),
	m_pnormalbuffer(NULL),
	m_pindexbuffer(NULL),
	m_nvertexnum(0),
	m_nindexnum(0)
{
	for (int i = 0; i < MAXTEXTUREPLAN; ++i)
		m_ptexcordbuffer[i] = NULL;
}

HQStaticMesh::~HQStaticMesh() {
	finalize();
}

HQMeshBase* HQMeshFactory::CreateStaticMeshCube(HQMeshFactory::Info* info) {
	HQStaticMesh* pdata = NULL;
	int idx;
	HQStaticMesh::Info meshinfo = {	info->nTracerIdx	};
	pdata = new(info->nTracerIdx) HQStaticMesh(&meshinfo);
	CHECK_RETURN(pdata == NULL, NULL);
	pdata->m_pvertexbuffer	= new(info->nTracerIdx) hq_vertex4[4 * 6];
	pdata->m_ptexcordbuffer[0]	= new(info->nTracerIdx) hq_vertex2[4 * 6];
	pdata->m_pnormalbuffer = new(info->nTracerIdx) hq_vertex3[4 * 6];
	pdata->m_pindexbuffer	= (UINT32*)MemAlloc(info->nTracerIdx, sizeof(UINT32) * 3 * 2 * 6);
	pdata->m_nindexnum	= 3 * 2 * 6;
	pdata->m_nvertexnum	= 4 * 6;
	//set vertex buffer
	idx = 0;
	{
#define SET_VERTEX_TEXTURE(x, y, z, u, v) do {\
	hq_setvertex(pdata->m_pvertexbuffer + (idx), x, y, z, 1.0f);\
	hq_setvertex(pdata->m_ptexcordbuffer[0] + (idx), u, v);\
	++ idx;\
} while(0)
		// Bottom Face
		SET_VERTEX_TEXTURE(-1.0f, -1.0f, -1.0f, 0, 0);
		SET_VERTEX_TEXTURE(1.0f,  -1.0f, -1.0f, 0, 1);
		SET_VERTEX_TEXTURE(1.0f,  -1.0f,  1.0f, 1, 1);
		SET_VERTEX_TEXTURE(-1.0f, -1.0f,  1.0f, 1, 0);
		// Front Face
		SET_VERTEX_TEXTURE(-1.0f, -1.0f,  1.0f, 0, 0);
		SET_VERTEX_TEXTURE(1.0f,  -1.0f,  1.0f, 0, 1);
		SET_VERTEX_TEXTURE(1.0f,  1.0f,  1.0f, 1, 1);
		SET_VERTEX_TEXTURE(-1.0f, 1.0f,  1.0f, 1, 0);
		// Back Face
		SET_VERTEX_TEXTURE(-1.0f, -1.0f, -1.0f, 0, 0);
		SET_VERTEX_TEXTURE(-1.0f, 1.0f, -1.0f, 0, 1);
		SET_VERTEX_TEXTURE(1.0f,  1.0f, -1.0f, 1, 1);
		SET_VERTEX_TEXTURE(1.0f,  -1.0f, -1.0f, 1, 0);
		// Right face
		SET_VERTEX_TEXTURE(1.0f, -1.0f, -1.0f, 0, 0);
		SET_VERTEX_TEXTURE(1.0f,  1.0f, -1.0f, 0, 1);
		SET_VERTEX_TEXTURE(1.0f,  1.0f,  1.0f, 1, 1);
		SET_VERTEX_TEXTURE(1.0f, -1.0f,  1.0f, 1, 0);
		// Left Face
		SET_VERTEX_TEXTURE(-1.0f, -1.0f, -1.0f, 0, 0);
		SET_VERTEX_TEXTURE(-1.0f, -1.0f,  1.0f, 0, 1);
		SET_VERTEX_TEXTURE(-1.0f,  1.0f,  1.0f, 1, 1);
		SET_VERTEX_TEXTURE(-1.0f,  1.0f, -1.0f, 1, 0);
#undef SET_VERTEX_TEXTURE
	}
	idx = 0;
	return pdata;
}

