#ifndef _HQSCENENODE_H_
#define _HQSCENENODE_H_

#include <scene/hq_boundingbox.h>
#include <container/hq_node.h>
#include <container/hq_doublelist.h>
#include <math/hq_math.h>

class HQSceneNode : PUBLIC MemoryManagedBase {
PUBLIC:
	HQSceneNode(UINT32 nTracerIdx);
PUBLIC:
	void AddData(void* pData);
	void ClearData();

PRIVATE:
	HQTreeNode*		m_pNodeLink;
	HQBoundingBox	m_vBoundingBox[2];
	hq_matrix4x4	m_vTransformMatrix[2];
	HQDoubleNode*	m_pDataList;
};

#endif//_HQSCENENODE_H_
