#ifndef _HQBINARYNODE_H_
#define _HQBINARYNODE_H_

#include <container/hq_node.h>

struct HQBinaryNode : PUBLIC HQTreeNode {
PUBLIC:
	HQBinaryNode();

	BOOLEAN		BinaryAddChild(HQBinaryNode* pChild, UINT32 childnum);
	HQBinaryNode*	BinaryRemoveChild(UINT32 childnum);

	HQBinaryNode*	BinaryGetChild(UINT32 childnum);
PRIVATE:
	HQBinaryNode* m_pQuadChilds[2];
};

#endif//_HQBINARYNODE_H_
