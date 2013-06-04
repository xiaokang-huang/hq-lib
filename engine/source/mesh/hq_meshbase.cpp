#include <mesh/hq_meshbase.h>

void HQMeshFactory::DestroyMesh(HQMeshBase* pMesh) {
	delete pMesh;
}
