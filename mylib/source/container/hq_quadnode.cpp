#include <container/hq_quadnode.h>

#define MAXCHILDNUM	4

HQQuadNode::HQQuadNode() : HQTreeNode() {
	m_pQuadChilds[0] = m_pQuadChilds[1] = m_pQuadChilds[2] = m_pQuadChilds[3] = NULL;
}

BOOLEAN HQQuadNode::QuadAddChild(HQQuadNode* pChild, UINT32 childnum) {
	if (childnum >= MAXCHILDNUM || m_pQuadChilds[childnum] || pChild == NULL)
		return FALSE;
	if (pChild->Attach(this) == FALSE)
		return FALSE;
	m_pQuadChilds[childnum] = pChild;
	return TRUE;
}

HQQuadNode* HQQuadNode::QuadRemoveChild(UINT32 childnum) {
	if (childnum >= MAXCHILDNUM || m_pQuadChilds[childnum] == NULL)
		return NULL;
	HQQuadNode* pchild = m_pQuadChilds[childnum];
	m_pQuadChilds[childnum] = NULL;
	pchild->Detach();
	return pchild;
}

HQQuadNode*	HQQuadNode::QuadGetChild(UINT32 childnum) {
	return (childnum >= MAXCHILDNUM)? NULL : m_pQuadChilds[childnum];
}
