#ifndef HQCONCURRENTQUEUE_H_
#define HQCONCURRENTQUEUE_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>
#include <container/hq_buffer.h>
#include <hq_thread.h>

template <typename DATATYPE>
class HQConcurrentRingQueue : PUBLIC HQBuffer {
#define GETPTR() ((DATATYPE*)HQBuffer::m_ptr)
PUBLIC:
	HQConcurrentRingQueue(UINT32 size, UINT32 tracer_idx) : HQBuffer(size * sizeof(DATATYPE), tracer_idx), m_QueueSize(0) {
		m_QueueSize = hq_uptopow2(size);
		m_Head.Set(0);
		m_Tail.Set(1);
	}
	~HQConcurrentRingQueue() {}
	INLINE BOOLEAN IsFull() const {
		return ((UINT32)(m_Tail.Get() - m_Head.Get()) > m_QueueSize);
	}
	INLINE BOOLEAN IsEmpty() const {
		return ((m_Tail.Get() - m_Head.Get()) == 1);
	}
	INLINE UINT32 GetUsed() const {
		return (UINT32)(m_Tail.Get() - m_Head.Get() - 1);
	}
	INLINE UINT32 GetSize() const {
		return m_QueueSize;
	}
	INLINE void CLear() {
		m_Head.Set(0);
		m_Tail.Set(1);
	}
	BOOLEAN PushBack(DATATYPE val) {
		while (TRUE) {
			if (IsFull())
				return FALSE;
			INT32 tail = m_Tail.Get();
			if (m_Tail.CompareAndSwap(tail, tail + 1)) {
				GETPTR()[(tail - 1) & (m_QueueSize - 1)] = val;
				break;
			}
		}
		return TRUE;
	}
	BOOLEAN PopFront(DATATYPE* val) {
		while (TRUE) {
			if (IsEmpty())
				return FALSE;
			INT32 head = m_Head.Get();
			*val = GETPTR()[head & (m_QueueSize - 1)];
			if (m_Head.CompareAndSwap(head, head + 1)) {
				break;
			}
		}
		return TRUE;
	}
PRIVATE:
	HQAtomic	m_Head;
	HQAtomic	m_Tail;
	UINT32		m_QueueSize;
};

#endif//HQCONCURRENTQUEUE_H_
