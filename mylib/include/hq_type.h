#ifndef _HQTYPE_H_
#define _HQTYPE_H_

typedef signed char			INT8;
typedef signed short		INT16;
typedef signed int			INT32;
typedef signed long long	INT64;
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned int		UINT32;
typedef unsigned long long	UINT64;
typedef signed long			INTSYS;
typedef unsigned long		UINTSYS;

typedef UINTSYS HQHANDLE;
typedef INTSYS 	HQOFFSET;

typedef float	REAL;
typedef double	DOUBLE;

typedef INT32	RESULT;
typedef INTSYS	_PTRDIFF;
typedef INT32	BOOLEAN;

#include <hq_os.h>

union MULTIPOINTER {
	INT8*		ptr_int8;
	INT16*		ptr_int16;
	INT32*		ptr_int32;
	INT64*		ptr_int64;
	INTSYS* 	ptr_intsys;
	UINT8*		ptr_uint8;
	UINT16*		ptr_uint16;
	UINT32*		ptr_uint32;
	UINT64*		ptr_uint64;
	UINTSYS*	ptr_uintsys;

	REAL*	ptr_real;
	DOUBLE*	ptr_double;

	_SIZE*	ptr_size;

	void*	ptr_void;
};

#endif//_HQTYPE_H_

