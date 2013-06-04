#include <hq_memory_mgr.h>
#include <hq_util.h>
#include <hq_thread.h>

#define BITS_TRACERS 5
#define TRACERS (1<<BITS_TRACERS)
#define TRACER_MASK (0xFF >> (8 - BITS_TRACERS))

class HQMemoryTracer {
PUBLIC:
	void	AllocCallBack(_SIZE count, void* ptr);
	void	FreeCallBack(_SIZE count, void* ptr);
	INT32	GetValue();
PRIVATE:
	HQAtomic m_atom;
};

void HQMemoryTracer::AllocCallBack(_SIZE count, void* ptr) {
	m_atom.Add((INT32)count);
}

void HQMemoryTracer::FreeCallBack(_SIZE count, void* ptr) {
	m_atom.Sub((INT32)count);
}

INT32 HQMemoryTracer::GetValue() {
	return m_atom.Get();
}

struct MemAllocatorManager {
	HQMemoryTracer m_atracers[TRACERS];

	void* MemAlloc(_SIZE size, UINT32 tracer_idx) {
		ASSERT(tracer_idx < TRACERS);
		_SIZE blocksize = size + sizeof(_SIZE);
		MULTIPOINTER multiptr;
		multiptr.ptr_void = malloc(blocksize);
		_SIZE magic = (blocksize << BITS_TRACERS) | (tracer_idx);
		*(multiptr.ptr_size) = magic;

		m_atracers[tracer_idx].AllocCallBack(blocksize, multiptr.ptr_void);
		++ multiptr.ptr_size;
		return multiptr.ptr_void;
	}

	void MemFree(void* ptr) {
		if (ptr == NULL)	return;
		MULTIPOINTER multiptr;
		multiptr.ptr_void = ptr;
		-- multiptr.ptr_size;
		_SIZE magic = *(multiptr.ptr_size);
		_SIZE blocksize = (magic >> BITS_TRACERS);
		_SIZE tracer_idx = magic & TRACER_MASK;
		free(multiptr.ptr_void);
		m_atracers[tracer_idx].FreeCallBack(blocksize, multiptr.ptr_void);
	}
};

static MemAllocatorManager sAllocatorManager;

void*	MemAlloc(UINT32 nTracerIdx, _SIZE size) {
	return sAllocatorManager.MemAlloc(size, nTracerIdx);
}
void	MemFree(void* ptr) {
	sAllocatorManager.MemFree(ptr);
}
void*	MemoryAllocatorBase::operator new		(_SIZE size, UINT32 tracer_idx) {
	return sAllocatorManager.MemAlloc(size, tracer_idx);
}
void*	MemoryAllocatorBase::operator new[]		(_SIZE size, UINT32 tracer_idx) {
	return sAllocatorManager.MemAlloc(size, tracer_idx);
}
void	MemoryAllocatorBase::operator delete		(void* ptr) {
	sAllocatorManager.MemFree(ptr);
}
void	MemoryAllocatorBase::operator delete[]	(void* ptr) {
	sAllocatorManager.MemFree(ptr);
}
INT32	MemoryAllocatorBase::GetSizeAllocated(UINT32 tracer_idx) {
	ASSERT(tracer_idx < TRACERS);
	return sAllocatorManager.m_atracers[tracer_idx].GetValue();
}
