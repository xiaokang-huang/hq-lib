#include <scene/hq_scenenode.h>

#define BACK_INDEX 0
#define CURR_INDEX 1


HQSceneNode::HQSceneNode() {
	m_NodeLink.m_pContainer = this;
}

HQTreeNode* HQSceneNode::GetTreeNode() {
    return &m_NodeLink;
}

HQBoundingBox* HQSceneNode::GetBoundingBox(BOOLEAN isCurrent) {
    return &m_vBoundingBox[isCurrent];
}

hq_matrix4x4* HQSceneNode::GetTransformMtx(BOOLEAN isCurrent) {
    return &m_vTransformMatrix[isCurrent];
}

void HQSceneNode::UpdateBackToCurrent() {
    hq_memcpy(&m_vBoundingBox[CURR_INDEX], &m_vBoundingBox[BACK_INDEX], sizeof(HQBoundingBox));
    hq_memcpy(&m_vTransformMatrix[CURR_INDEX], &m_vTransformMatrix[BACK_INDEX], sizeof(hq_matrix4x4));
}
