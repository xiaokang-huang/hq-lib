#include <container/hq_doublelist.h>
#include <hq_os.h>

HQDoubleNode::HQDoubleNode()
	: m_pPrev(NULL), m_pNext(NULL),
	  m_pVal(NULL) {}

HQDoubleNode* HQDoubleNode::GetPrev() const {
	return m_pPrev;
}

HQDoubleNode* HQDoubleNode::GetNext() const {
	return m_pNext;
}

void HQDoubleNode::SetPrev(HQDoubleNode* pnode) {
	m_pPrev = pnode;
	if (pnode)
		pnode->m_pNext = this;
}

void HQDoubleNode::SetNext(HQDoubleNode* pnode) {
	m_pNext = pnode;
	if (pnode)
		pnode->m_pPrev = this;
}

void HQDoubleNode::InsertAfter(HQDoubleNode* pnode) {
	ASSERT(pnode);
	HQDoubleNode* pnext = pnode->GetNext();
	SetPrev(pnode);
	SetNext(pnext);
}
