#ifndef _HQHASH_H_
#define _HQHASH_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>
#include <hq_errorcode.h>

struct HQHash : MemoryAllocatorBase {
	HQHash() : hashA(0), hashB(0)	{}
	UINT32	hashA;
	UINT32	hashB;
};

class HQHashTable : PUBLIC MemoryManagedBase {
PUBLIC:
	static const UINT32 HASHPOS_INVALID;
PUBLIC:
	HQHashTable(UINT32 nTrackIdx, UINT32 nTableLength = 1024);
	~HQHashTable();
PUBLIC:
	UINT32 InsertValue(const char* str);
	UINT32 GetValue(const char* str);
	UINT32 DeleteValue(const char* str);
	void   Clear();

	UINT32 GetCapacity() const;
	UINT32 GetUsed() const;
	BOOLEAN IsFull() const;
	BOOLEAN IsEmpty() const;
PRIVATE:
	HQHash* 	m_ptable;
	UINT32		m_lengthcoef;
	UINT32		m_nused;
};

const UINT32 HQHashTable::HASHPOS_INVALID = ((UINT32)HQPOSITION_INVALID);
#endif//_HQHASH_H_
