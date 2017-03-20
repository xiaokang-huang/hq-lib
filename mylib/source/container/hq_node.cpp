#include <container/hq_node.h>
#include <hq_os.h>

HQTreeNode* HQTreeNode::GetParent() const {
	return m_pParent;
}

HQTreeNode* HQTreeNode::GetFirstChild() const {
	if (m_pNext && m_pNext->m_pParent == this)
		return m_pNext;
	return NULL;
}

HQTreeNode* HQTreeNode::GetLastChild() const {
	HQTreeNode* pFirstChild = GetFirstChild();
	return (pFirstChild)? pFirstChild->m_pPrevSibling : NULL;
}

HQTreeNode* HQTreeNode::GetPrevSibling() const {
	if (m_pParent == NULL)	return NULL;
	if (m_pParent->m_pNext == this)	return NULL;
	return m_pPrevSibling;
}

HQTreeNode* HQTreeNode::GetNextSibling() const {
	HQTreeNode* psib = m_pLastDecendent->m_pNext;
	if (psib && psib->m_pParent == m_pParent)	return psib;
	return NULL;
}

HQTreeNode* HQTreeNode::GetNext() const {
	return m_pNext;
}

HQTreeNode* HQTreeNode::GetPrev() const {
	if (m_pParent && m_pParent->m_pNext == this)
		return m_pParent;
	if (m_pPrevSibling->m_pLastDecendent->m_pNext == this)
		return m_pPrevSibling->m_pLastDecendent;
	return NULL;
}

BOOLEAN	HQTreeNode::IsRoot() const {
	return (m_pParent == NULL)? TRUE : FALSE;
}
BOOLEAN	HQTreeNode::IsLeaf() const {
	return (m_pLastDecendent == this)? TRUE : FALSE;
}

BOOLEAN HQTreeNode::Attach(HQTreeNode* pParent) {
	if (pParent == NULL || m_pParent != NULL)
		return FALSE;
	HQTreeNode* pfirstchild = pParent->GetFirstChild();
	// update this node or this tree
	m_pParent = pParent;
	m_pPrevSibling = (pParent->GetLastChild())? pParent->GetLastChild() : this;
	m_pLastDecendent->m_pNext = pParent->m_pLastDecendent->m_pNext;
	// update parent tree
	pParent->m_pLastDecendent->m_pNext = this;
	pParent->m_pLastDecendent = m_pLastDecendent;
	if (pfirstchild) {
		pfirstchild->m_pPrevSibling = this;
	}
	return TRUE;
}

BOOLEAN HQTreeNode::Attach(HQTreeNode* pParent, HQTreeNode* pNextSibling) {
	if (pParent == NULL || m_pParent != NULL || pNextSibling->m_pParent != pParent)
		return FALSE;
	HQTreeNode* pPrevSibling = pNextSibling->GetPrevSibling();
	// update this node or this tree
	m_pParent = pParent;
	m_pPrevSibling = (pPrevSibling)? pPrevSibling : pParent->GetLastChild();
	m_pLastDecendent->m_pNext = pNextSibling;
	// update parent tree
	if (pPrevSibling) {
		pPrevSibling->m_pLastDecendent->m_pNext = this;
	} else {
		m_pParent->m_pNext = this;
	}
	pNextSibling->m_pPrevSibling = this;
	return TRUE;
}

BOOLEAN HQTreeNode::Detach() {
	if (m_pParent == NULL)
		return FALSE;
	HQTreeNode* pPrev = GetPrev();
	HQTreeNode* pNextSibling = (GetNextSibling())? GetNextSibling() : m_pParent->GetFirstChild();
	HQTreeNode* pParent = m_pParent;
	HQTreeNode* pLastDecendent = (pParent->m_pLastDecendent == m_pLastDecendent)? pParent : pParent->m_pLastDecendent;
	// update parent tree
	while (pParent && pParent->m_pLastDecendent == m_pLastDecendent) {
		pParent->m_pLastDecendent = pLastDecendent;
		pParent = pParent->m_pParent;
	}
	pPrev->m_pNext = m_pLastDecendent->m_pNext;
	if (pNextSibling != this) {
		pNextSibling->m_pPrevSibling = m_pPrevSibling;
	}
	// update this node or this tree
	m_pParent = NULL;
	m_pPrevSibling = this;
	m_pLastDecendent->m_pNext = NULL;

	return TRUE;
}
