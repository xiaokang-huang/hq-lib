#include <scene/hq_scenenode.h>

HQSceneNode::HQSceneNode(UINT32 nTracerIdx)
	: MemoryManagedBase(nTracerIdx), m_pNodeLink(NULL), m_pDataList(NULL)
{}

void HQSceneNode::AddData(void* pData) {
	HQDoubleNode* pnode = Managed_New(HQDoubleNode, ());
	pnode->m_pVal = pData;
	pnode->SetNext(m_pDataList);
	m_pDataList = pnode;
}

void HQSceneNode::ClearData() {
	while (m_pDataList) {
		HQDoubleNode* pnode = m_pDataList;
		m_pDataList = pnode->GetNext();
		pnode->SetNext(NULL);
		Managed_Delete(pnode);
	}
}
