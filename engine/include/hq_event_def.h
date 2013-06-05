#ifndef _HQEVENTDEF_H_
#define _HQEVENTDEF_H_

#include <hq_type.h>

typedef UINT32 HQEventType;
typedef UINT32 HQEventData;
typedef void(*EventCb)(HQEventType, HQEventData);

/*
 * event type data:
 * | BASETYPE(8Bit) | SUBTYPE(24Bit) |
 */
const UINT32 BASETYPE_SHIFT = 24;
#define GET_BASETYPE(inputtype)			((inputtype) & (0xFF000000))

const HQEventType HQEVENTTYPE_KEY	= (0x01 << (BASETYPE_SHIFT + 0));
const HQEventType HQEVENTTYPE_KEY_ASCII	= (HQEVENTTYPE_KEY | 0x01);
const HQEventType HQEVENTTYPE_KEY_ARROW	= (HQEVENTTYPE_KEY | 0x02);
const HQEventType HQEVENTTYPE_KEY_FUNCTION	= (HQEVENTTYPE_KEY | 0x03);

const HQEventType HQEVENTTYPE_POINT	= (0x01 << (BASETYPE_SHIFT + 1));

const HQEventType HQEVENTTYPE_SYSTEM	= (0x01 << (BASETYPE_SHIFT + 2));
const HQEventType HQEVENTTYPE_SYSTEM_EXIT	= (HQEVENTTYPE_SYSTEM | 0x01);

/*
 * ascii key event data:
 * | Reserved(16Bit) | MODIFIER_FLAG(8Bit) | ASCII(8Bit) |
 */
const UINT32 MODIFIER_SHIFT = 8;
const HQEventData HQEVENTDATA_KEY_MODIFIER_MASK_CONTROL	= (0x01 << (MODIFIER_SHIFT + 0));
const HQEventData HQEVENTDATA_KEY_MODIFIER_MASK_ALT		= (0x01 << (MODIFIER_SHIFT + 1));
const HQEventData HQEVENTDATA_KEY_MODIFIER_MASK_SHIFT	= (0x01 << (MODIFIER_SHIFT + 2));
const HQEventData HQEVENTDATA_KEY_ASCII_MASK			= (0xFF);

/*
 * arrow key event data:
 * | Reserved(24Bit) | ARROW(8Bit) |
 */
const HQEventData HQEVENTDATA_KEY_ARROW_LEFT	= (0x01 << 0);
const HQEventData HQEVENTDATA_KEY_ARROW_UP		= (0x01 << 1);
const HQEventData HQEVENTDATA_KEY_ARROW_RIGHT	= (0x01 << 2);
const HQEventData HQEVENTDATA_KEY_ARROW_DOWN	= (0x01 << 3);

/*
 * function key event data:
 * | Reserved(24Bit) | FUNCTION(8Bit) |
 */



#endif//_HQEVENTDEF_H_
