#ifndef _HQSCENENODE_H_
#define _HQSCENENODE_H_

#include <scene/hq_boundingbox.h>
#include <container/hq_node.h>
#include <container/hq_doublelist.h>
#include <math/hq_math.h>

class HQSceneNode : PUBLIC MemoryAllocatorBase {
PUBLIC:
	HQSceneNode();

	HQTreeNode* GetTreeNode();
    HQBoundingBox* GetBoundingBox(BOOLEAN isCurrent);
    hq_matrix4x4* GetTransformMtx(BOOLEAN isCurrent);
    void UpdateBackToCurrent();
PRIVATE:
	HQTreeNode		m_NodeLink;
	HQBoundingBox   m_vBoundingBox[2];
	hq_matrix4x4    m_vTransformMatrix[2];
};

#endif//_HQSCENENODE_H_
