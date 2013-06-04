#ifndef _HQEVENTDEF_H_
#define _HQEVENTDEF_H_

#include <hq_type.h>

/*
 * event type data:
 * | BASETYPE(8Bit) | SUBTYPE(24Bit) |
 */

const UINT32 _BASETYPE = 0x01000000;
const UINT32 _KEY = 1;
const UINT32 _POINT = 2;
const UINT32 _SYSTEM = 3;

#define DEFEVENTBASE(eve) EVENTBASE##eve = (eve)*_BASETYPE
#define DEFEVENTDATA(eve) EVENTDATA##eve

#define ISEVENTBASE(c, eve) (((c) & (EVENTBASE##eve)) == (EVENTBASE##eve))
#define ISEVENTDATA(c, eve) ((c) == (EVENTDATA##eve))

typedef enum {
	DEFEVENTBASE(_KEY),		//key event
	DEFEVENTBASE(_POINT),	//point event
	DEFEVENTBASE(_SYSTEM),	//system event
		DEFEVENTDATA(_EXIT),
} HQEventTypes;

#endif//_HQEVENTDEF_H_
