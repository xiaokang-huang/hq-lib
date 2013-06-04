#ifndef HQ_MEMORY_MGR_H_
#define HQ_MEMORY_MGR_H_

#include <hq_type.h>
#include <hq_os.h>

void*	MemAlloc(UINT32 nTracerIdx, _SIZE size);
void	MemFree(void* ptr);

class MemoryAllocatorBase {
PUBLIC:
	void*	operator new		(_SIZE size, UINT32 tracer_idx);
	void*	operator new[]		(_SIZE size, UINT32 tracer_idx);
	void	operator delete		(void* ptr);
	void	operator delete[]	(void* ptr);
	static INT32 GetSizeAllocated(UINT32 tracer_idx);
};

class MemoryManagedBase : PUBLIC MemoryAllocatorBase {
PUBLIC:
	explicit MemoryManagedBase(UINT32 tracer_idx) : m_nTracerIdx(tracer_idx) {}
PROTECTED:
	UINT32	m_nTracerIdx;
};

#define Managed_Alloc(size)				MemAlloc(m_nTracerIdx, size)
#define Managed_Free(ptr)				MemFree(ptr)
#define Managed_New(data_type, args)	new(m_nTracerIdx) data_type args
#define Managed_NewN(N, data_type)		new(m_nTracerIdx) data_type[N]
#define Managed_Delete(ptr) 			delete ptr
#define Managed_DeleteN(ptr)			delete[] ptr;
#define Managed_Delete_S(ptr) 			do {	delete ptr;	ptr = NULL;	} while(0)
#define Managed_DeleteN_S(ptr)			do {	delete[] ptr;	ptr = NULL;	} while(0)

#endif /* HQ_MEMORY_MGR_H_ */
