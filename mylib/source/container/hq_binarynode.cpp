#include <container/hq_binarynode.h>



HQBinaryNode::HQBinaryNode() : HQTreeNode() {
	for (UINT32 i = 0; i < MAXCHILDNUM; ++i) {
		m_pBinaryChilds[i] = NULL;
	}

}

BOOLEAN HQBinaryNode::BinaryAddChild(HQBinaryNode* pChild, UINT32 childnum) {
	if (childnum >= MAXCHILDNUM || m_pBinaryChilds[childnum] || pChild == NULL)
		return FALSE;
	if (pChild->Attach(this) == FALSE)
		return FALSE;
	m_pBinaryChilds[childnum] = pChild;
	return TRUE;
}

HQBinaryNode* HQBinaryNode::BinaryRemoveChild(UINT32 childnum) {
	if (childnum >= MAXCHILDNUM || m_pBinaryChilds[childnum] == NULL)
		return NULL;
	HQBinaryNode* pchild = m_pBinaryChilds[childnum];
	m_pBinaryChilds[childnum] = NULL;
	pchild->Detach();
	return pchild;
}

HQBinaryNode*	HQBinaryNode::BinaryGetChild(UINT32 childnum) {
	return (childnum >= MAXCHILDNUM)? NULL : m_pBinaryChilds[childnum];
}
