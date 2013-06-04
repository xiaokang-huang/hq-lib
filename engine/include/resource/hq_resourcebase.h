#ifndef _HQRESOURCEBASE_H_
#define _HQRESOURCEBASE_H_

#include <mesh/hq_meshbase.h>
#include <hq_hash.h>
#include <hq_thread.h>
#include <hq_errorcode.h>

template <typename DATATYPE>
class HQResourceBase : PUBLIC MemoryManagedBase {
PUBLIC:
	static const UINT32 HASHPOS_INVALID;
	enum {
		RESULT_SUCCESS,
		RESULT_INVALID_STATUS,
		RESULT_INVALID_VALUE,
		RESULT_FULL,
		RESULT_NOTFULL,
		RESULT_EMPTY,
		RESULT_NOTEMPTY,
		RESULT_DUPLICATE,
		RESULT_STILLUSED,
	};
PUBLIC:
	HQResourceBase(UINT32 nTrackIdx);
	~HQResourceBase();

	RESULT Initialzie(UINT32 nCapacity);
	RESULT Finallize();

	RESULT AddData(const char* pStr, DATATYPE* pData);
	DATATYPE* DeleteData(const char* pStr);

	DATATYPE* Reference(const char* pStr);
	DATATYPE* Dereference(const char* pStr);

PRIVATE:
PRIVATE:
	HQHashTable*	m_phashtable;
	DATATYPE**		m_pdatatable;
	HQAtomic*		m_prefcnttable;
};

template <typename DATATYPE>
const UINT32 HQResourceBase<DATATYPE>::HASHPOS_INVALID = ((UINT32)HQPOSITION_INVALID);

template <typename DATATYPE>
HQResourceBase<DATATYPE>::HQResourceBase(UINT32 nTrackIdx) :
	MemoryManagedBase(nTrackIdx),
	m_phashtable(NULL),
	m_pdatatable(NULL),
	m_prefcnttable(NULL){}

template <typename DATATYPE>
HQResourceBase<DATATYPE>::~HQResourceBase() {
	Finallize();
}

template <typename DATATYPE>
RESULT HQResourceBase<DATATYPE>::Initialzie(UINT32 nCapacity) {
	INT32 ret = Finallize();
	if (ret != HQResourceBase<DATATYPE>::RESULT_SUCCESS)
		return ret;
	m_phashtable	= Managed_New(HQHashTable, (m_nTracerIdx, nCapacity));
	m_pdatatable	= (DATATYPE**)Managed_Alloc(sizeof(DATATYPE*) * m_phashtable->GetCapacity());
	m_prefcnttable	= (HQAtomic*)Managed_Alloc(sizeof(HQAtomic) * m_phashtable->GetCapacity());

	for (UINT32 i = 0; i < m_phashtable->GetCapacity(); ++i) {
		m_pdatatable[i] = NULL;
		m_prefcnttable[i].Set(0);
	}

	return ret;
}

template <typename DATATYPE>
RESULT HQResourceBase<DATATYPE>::Finallize() {
	if (m_prefcnttable) {
		for (UINT32 i = 0; i < m_phashtable->GetCapacity(); ++i) {
			if (m_prefcnttable[i].Get() != 0)
				return HQResourceBase<DATATYPE>::RESULT_STILLUSED;
		}
		Managed_Free(m_prefcnttable);
		m_prefcnttable = NULL;
		Managed_Delete(m_phashtable);
		m_phashtable = NULL;
		Managed_Free(m_pdatatable);
		m_pdatatable = NULL;
	}
	return HQResourceBase<DATATYPE>::RESULT_SUCCESS;
}

template <typename DATATYPE>
RESULT HQResourceBase<DATATYPE>::AddData(const char* pStr, DATATYPE* pData) {
	ASSERT(m_phashtable && m_pdatatable);
	if (pData == NULL)
		return HQResourceBase<DATATYPE>::RESULT_INVALID_VALUE;
	if (m_phashtable->IsFull())
		return HQResourceBase<DATATYPE>::RESULT_FULL;
	UINT32 pos = m_phashtable->InsertValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID)
		return HQResourceBase<DATATYPE>::RESULT_DUPLICATE;
	m_pdatatable[pos] = pData;
	return HQResourceBase<DATATYPE>::RESULT_SUCCESS;
}

template <typename DATATYPE>
DATATYPE* HQResourceBase<DATATYPE>::Reference(const char* pStr) {
	ASSERT(m_phashtable && m_pdatatable);
	UINT32 pos = m_phashtable->GetValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID)
		return NULL;
	m_prefcnttable[pos].Add(1);
	return m_pdatatable[pos];
}

template <typename DATATYPE>
DATATYPE* HQResourceBase<DATATYPE>::Dereference(const char* pStr) {
	ASSERT(m_phashtable && m_pdatatable);
	UINT32 pos = m_phashtable->GetValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID)
		return NULL;
	if (m_prefcnttable[pos].Get() == 0)
		return NULL;
	m_prefcnttable[pos].Sub(1);
	return m_pdatatable[pos];
}

template <typename DATATYPE>
DATATYPE* HQResourceBase<DATATYPE>::DeleteData(const char* pStr) {
	ASSERT(m_phashtable && m_pdatatable);
	UINT32 pos = m_phashtable->DeleteValue(pStr);
	if (pos == HQHashTable::HASHPOS_INVALID) {
		return NULL;
	}
	DATATYPE* pdata = m_pdatatable[pos];
	m_pdatatable[pos] = NULL;
	return pdata;
}

#endif//_HQRESOURCEBASE_H_
