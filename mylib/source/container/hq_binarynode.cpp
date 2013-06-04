#include <container/hq_binarynode.h>

#define MAXCHILDNUM	2

HQBinaryNode::HQBinaryNode() : HQTreeNode() {
	m_pQuadChilds[0] = m_pQuadChilds[1] = m_pQuadChilds[2] = m_pQuadChilds[3] = NULL;
}

BOOLEAN HQBinaryNode::BinaryAddChild(HQBinaryNode* pChild, UINT32 childnum) {
	if (childnum >= MAXCHILDNUM || m_pQuadChilds[childnum] || pChild == NULL)
		return FALSE;
	if (pChild->Attach(this) == FALSE)
		return FALSE;
	m_pQuadChilds[childnum] = pChild;
	return TRUE;
}

HQBinaryNode* HQBinaryNode::BinaryRemoveChild(UINT32 childnum) {
	if (childnum >= MAXCHILDNUM || m_pQuadChilds[childnum] == NULL)
		return NULL;
	HQBinaryNode* pchild = m_pQuadChilds[childnum];
	m_pQuadChilds[childnum] = NULL;
	pchild->Detach();
	return pchild;
}

HQBinaryNode*	HQBinaryNode::BinaryGetChild(UINT32 childnum) {
	return (childnum >= MAXCHILDNUM)? NULL : m_pQuadChilds[childnum];
}
