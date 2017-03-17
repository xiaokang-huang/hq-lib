#ifndef _HQBINARYNODE_H_
#define _HQBINARYNODE_H_

#include <container/hq_node.h>

struct HQBinaryNode : PUBLIC HQTreeNode {
PRIVATE:
	static const UINT32 MAXCHILDNUM = 2;
PUBLIC:
	HQBinaryNode();

	BOOLEAN		BinaryAddChild(HQBinaryNode* pChild, UINT32 childnum);
	HQBinaryNode*	BinaryRemoveChild(UINT32 childnum);

	HQBinaryNode*	BinaryGetChild(UINT32 childnum);
PRIVATE:
	HQBinaryNode* m_pBinaryChilds[MAXCHILDNUM];
};

#endif//_HQBINARYNODE_H_
