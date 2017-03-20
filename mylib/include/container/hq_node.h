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

struct HQLimitedTreeNodeBase : PUBLIC HQTreeNode {
PUBLIC:
	virtual INT32 GetMaxChildCount() = 0;
	virtual HQTreeNode* GetChild(INT32 nIndex) = 0;
	virtual BOOLEAN Attach(HQLimitedTreeNodeBase* pParent, INT32 nIndex) = 0;
	virtual BOOLEAN RemoveChild(INT32 nIndex) = 0;
PROTECTED:
	virtual void setChild(INT32 nIndex, HQTreeNode* pChild) = 0;
};

template <INT32 MAX_CHILD_NUM>
struct HQLimitedTreeNode : PUBLIC HQLimitedTreeNodeBase {
PUBLIC:
	HQLimitedTreeNode() : HQLimitedTreeNodeBase() {
		hq_memset(m_pChildPtr, 0, sizeof(m_pChildPtr));
	}

	virtual INT32 GetMaxChildCount() {
		return MAX_CHILD_NUM;
	}

	virtual HQTreeNode* GetChild(INT32 nIndex) {
		return m_pChildPtr[nIndex];
	}

	virtual BOOLEAN Attach(HQLimitedTreeNodeBase* pParent, INT32 nIndex) {
		if (nIndex >= pParent->GetMaxChildCount() || pParent->GetChild(nIndex) != NULL)	return FALSE;
		if (Attach(pParent) == FALSE)	return FALSE;
		pParent->setChild(nIndex, this);
		return TRUE;
	}

	virtual BOOLEAN RemoveChild(INT32 nIndex) {
		HQTreeNode* child = GetChild(nIndex);
		if (child == NULL)	return FALSE;
		child->Detach();
		setChild(nIndex, NULL);
	}
PROTECTED:
	virtual void setChild(INT32 nIndex, HQTreeNode* pChild) {
		m_pChildPtr[nIndex] = pChild;
	}
PRIVATE:
	HQLimitedTreeNodeBase* m_pChildPtr[MAX_CHILD_NUM];
};

typedef HQLimitedTreeNode<2> HQBinaryTreeNode;
typedef HQLimitedTreeNode<4> HQQuadTreeNode;
typedef HQLimitedTreeNode<8> HQOCTreeNode;

#endif//_HQNODE_H_
