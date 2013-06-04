#ifndef _HQALLOCATOR_H_
#define _HQALLOCATOR_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_memory_mgr.h>

template <typename T>
class HQAllocatorBase {
PUBLIC:
	typedef T					value_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef _SIZE				size_type;
	typedef _PTRDIFF			difference_type;
};

template <typename T, typename MemMgrT>
class HQAllocator : PUBLIC HQAllocatorBase<T>  {
PUBLIC:
	pointer		Allocate (size_type count);
	void		DeAllocate (pointer ptr);
	size_type	GetMemAllocated () const;
PRIVATE:
	static MemMgrT memmgr;
};

template <typename T, typename MemMgrT>
static MemMgrT HQAllocator<T, MemMgrT>::memmgr;

#endif//_HQALLOCATOR_H_
