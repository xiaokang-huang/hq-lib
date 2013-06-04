#ifndef _HQDOUBLELIST_H_
#define _HQDOUBLELIST_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>

struct HQDoubleNode : PUBLIC MemoryAllocatorBase {
PUBLIC:
	HQDoubleNode();

	HQDoubleNode*	GetPrev() const;
	HQDoubleNode*	GetNext() const;
	void			SetPrev(HQDoubleNode* pnode);
	void			SetNext(HQDoubleNode* pnode);
	void			InsertAfter(HQDoubleNode* pnode);
PRIVATE:
	HQDoubleNode* m_pPrev;
	HQDoubleNode* m_pNext;
PUBLIC:
	void* m_pVal;
};

#endif//_HQDOUBLELIST_H_
