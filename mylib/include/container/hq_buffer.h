#ifndef HQBUFFER_H_
#define HQBUFFER_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_memory_mgr.h>

class HQBuffer : PUBLIC MemoryManagedBase {
PUBLIC:
	HQBuffer(UINT32 tracer_idx) : MemoryManagedBase(tracer_idx), m_size(0), m_ptr(NULL) {}
	HQBuffer(UINT32 size, UINT32 tracer_idx) : MemoryManagedBase(tracer_idx), m_size(size), m_ptr(NULL) {
		m_ptr = Managed_Alloc(size);
	}
	~HQBuffer() {
		Managed_Free(m_ptr);
	}
	HQBuffer* Create(UINT32 size) {
		m_ptr = Managed_Alloc(size);
		m_size = size;
		return this;
	}
	void Destory() {
		Managed_Free(m_ptr);
		m_ptr = NULL;
		m_size = 0;
	}
	UINT32	GetSize()	{	return m_size;	}
	void*	GetBuf()	{	return m_ptr;	}
PROTECTED:
	UINT32	m_size;
	void*	m_ptr;
};

#endif//HQBUFFER_H_
