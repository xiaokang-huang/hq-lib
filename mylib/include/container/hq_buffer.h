#ifndef HQBUFFER_H_
#define HQBUFFER_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_memory_mgr.h>

class HQBuffer : PUBLIC MemoryManagedBase {
PUBLIC:
	HQBuffer(UINT32 size, UINT32 tracer_idx) : MemoryManagedBase(tracer_idx), m_size(size), m_ptr(NULL) {
		m_ptr = Managed_Alloc(size);
	}
	~HQBuffer() {
		Managed_Free(m_ptr);
	}

PROTECTED:
	UINT32	m_size;
	void*	m_ptr;
};

#endif//HQBUFFER_H_
