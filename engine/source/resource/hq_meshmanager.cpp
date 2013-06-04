/*#include <resource/hq_meshmanager.h>

HQMeshManager::HQMeshManager(UINT32 nTrackIdx) :
	MemoryManagedBase(nTrackIdx),
	m_phashtable(NULL),
	m_pmeshtable(NULL),
	m_prefcnttable(NULL){}

HQMeshManager::~HQMeshManager() {
	Finallize();
}

HQMeshManager::RESULT HQMeshManager::Initialzie(UINT32 nCapacity) {
	HQMeshManager::RESULT ret = Finallize();
	if (ret != HQMeshManager::RESULT_SUCCESS)
		return ret;
	m_phashtable	= Managed_New(HQHashTable, (m_nTracerIdx, nCapacity));
	m_pmeshtable	= (HQMeshBase**)Managed_Alloc(sizeof(HQMeshBase*) * m_phashtable->GetCapacity());
	m_prefcnttable	= (HQAtomic*)Managed_Alloc(sizeof(HQAtomic) * m_phashtable->GetCapacity());

	for (UINT32 i = 0; i < m_phashtable->GetCapacity(); ++i) {
		m_pmeshtable[i] = NULL;
		m_prefcnttable[i].Set(0);
	}

	return ret;
}

HQMeshManager::RESULT HQMeshManager::Finallize() {
	if (m_prefcnttable) {
		for (UINT32 i = 0; i < m_phashtable->GetCapacity(); ++i) {
			if (m_prefcnttable[i].Get() != 0)
				return HQMeshManager::RESULT_STILLUSED;
		}
		Managed_Free(m_prefcnttable);
		m_prefcnttable = NULL;
		Managed_Delete(m_phashtable);
		m_phashtable = NULL;
		Managed_Free(m_pmeshtable);
		m_pmeshtable = NULL;
	}
	return HQMeshManager::RESULT_SUCCESS;
}

HQMeshManager::RESULT HQMeshManager::AddMesh (const char* pStr, HQMeshBase* pMesh) {
	ASSERT(m_phashtable && m_pmeshtable);
	if (pMesh == NULL)
		return HQMeshManager::RESULT_INVALID_VALUE;
	if (m_phashtable->IsFull())
		return HQMeshManager::RESULT_FULL;
	UINT32 pos = m_phashtable->InsertValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID)
		return HQMeshManager::RESULT_DUPLICATE;
	m_pmeshtable[pos] = pMesh;
	return HQMeshManager::RESULT_SUCCESS;
}

HQMeshBase* HQMeshManager::Reference(const char* pStr) {
	ASSERT(m_phashtable && m_pmeshtable);
	UINT32 pos = m_phashtable->GetValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID)
		return NULL;
	m_prefcnttable[pos].Add(1);
	return m_pmeshtable[pos];
}

HQMeshBase* HQMeshManager::Dereference(const char* pStr) {
	ASSERT(m_phashtable && m_pmeshtable);
	UINT32 pos = m_phashtable->GetValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID)
		return NULL;
	if (m_prefcnttable[pos].Get() == 0)
		return NULL;
	m_prefcnttable[pos].Sub(1);
	return m_pmeshtable[pos];
}

HQMeshBase* HQMeshManager::DeleteMesh(const char* pStr) {
	ASSERT(m_phashtable && m_pmeshtable);
	UINT32 pos = m_phashtable->DeleteValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID) {
		return NULL;
	}
	HQMeshBase* pmesh = m_pmeshtable[pos];
	m_pmeshtable[pos] = NULL;
	return pmesh;
}
*/
