#ifndef _HQQUADNODE_H_
#define _HQQUADNODE_H_

#include <container/hq_node.h>

struct HQQuadNode : PUBLIC HQTreeNode {
PUBLIC:
	HQQuadNode();

	BOOLEAN		QuadAddChild(HQQuadNode* pChild, UINT32 childnum);
	HQQuadNode*	QuadRemoveChild(UINT32 childnum);

	HQQuadNode*	QuadGetChild(UINT32 childnum);
PRIVATE:
	HQQuadNode* m_pQuadChilds[4];
};

#endif//_HQQUADNODE_H_
