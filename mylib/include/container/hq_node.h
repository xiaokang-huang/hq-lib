#ifndef _HQNODE_H_
#define _HQNODE_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>

struct HQTreeNode : PUBLIC MemoryAllocatorBase {
PUBLIC:
	HQTreeNode() : m_pContainer(NULL), m_pParent(NULL), m_pPrevSibling(this), m_pNext(NULL), m_pLastDecendent(this) {}

	HQTreeNode* GetParent() const;
	HQTreeNode* GetFirstChild() const;
	HQTreeNode* GetLastChild() const;
	HQTreeNode* GetPrevSibling() const;
	HQTreeNode* GetNextSibling() const;
	HQTreeNode* GetNext() const;
	HQTreeNode* GetPrev() const;

	BOOLEAN		IsRoot() const;
	BOOLEAN		IsLeaf() const;

	BOOLEAN Attach(HQTreeNode* pParent);
    BOOLEAN Attach(HQTreeNode* pParent, HQTreeNode* pNextSibling);
	BOOLEAN Detach();
PUBLIC:
	void* m_pContainer;
PROTECTED:
	HQTreeNode* m_pParent;
	HQTreeNode* m_pPrevSibling;
	HQTreeNode* m_pNext;
	HQTreeNode* m_pLastDecendent;
};

#endif//_HQNODE_H_
