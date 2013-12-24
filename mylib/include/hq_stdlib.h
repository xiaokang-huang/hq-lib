#ifndef HQSTDLIB_H_
#define HQSTDLIB_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//char
#define hq_toupper_unsafe(c)	((c)&~0x20)
#define hq_tolower_unsafe(c)	((c)|0x20)
#define hq_isalnum(c)			isalnum(c)
#define hq_isalpha(c)			isalpha(c)
#define hq_iscntrl(c)			iscntrl(c)
#define hq_isdigit(c)			isdigit(c)
#define hq_islower(c)			islower(c)
#define hq_isascii(c)			isascii(c)
#define hq_isgraph(c)			isgraph(c)
#define hq_isprint(c)			isprint(c)
#define hq_ispunct(c)			ispunct(c)
#define hq_isspace(c)			isspace(c)
#define hq_isupper(c)			isupper(c)
#define hq_isxdigit(c)			isxdigit(c)
#define hq_toascii(c)			toascii(c)
#define hq_tolower(c)			tolower(c)
#define hq_toupper(c)			toupper(c)

//math
#define hq_abs(x)			abs(x)
#define hq_acos(x)			acos(x)
#define hq_asin(x)			asin(x)
#define hq_atan(x)			atan(x)
#define hq_atan2(y, x)		atan2(y, x)
#define hq_ceil(x)			ceil(x)
#define hq_cos(x)			cos(x)
#define hq_cosh(x)			cosh(x)
#define hq_exp(x)			exp(x)
#define hq_fabs(x)			fabs(x)
#define hq_floor(x)			floor(x)
#define hq_fmod(x, y)		fmod(x, y)
#define hq_frexp(x, exp)	frexp(x, exp)
#define hq_hypot(x, y)		hypot(x, y)
#define hq_ldexp(x, exp)	ldexp(x, exp)
#define hq_log(x)			log(x)
#define hq_log10(x)			log10(x)
#define hq_modf(num, i)		modf(num, i)
#define hq_pow(x, y)		pow(x, y)
#define hq_pow10(x)			pow10(x)
#define hq_sin(x)			sin(x)
#define hq_sinh(x)			sinh(x)
#define hq_sqrt(x)			sqrt(x)
#define hq_tan(x)			tan(x)
#define hq_tanh(x)			tanh(x)

const int MultiplyDeBruijnBitPosition32[32] = {
  0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9
};
const int MultiplyDeBruijnBitPosition64[64] = {
	0, 1, 48,2, 57,49,28,3, 61,58,50,42,38,29,17,4,
	62,55,59,36,53,51,43,22,45,39,33,30,24,18,12,5,
	63,47,56,27,60,41,37,16,54,35,52,21,44,32,23,11,
	46,26,40,15,34,20,31,10,25,14,19,9, 13,8, 7, 6
};
#define hq_is2pow(x)		(((x)&(-x)) == (x))
#define hq_isodd(x)			((x)&0x1)
#define hq_iseven(x)		(!((x)&0x1))
#define hq_fastlog2_32(x)	(MultiplyDeBruijnBitPosition32[((unsigned long)(((x) & (-x)) * 0x077CB531U)) >> 27])
#define hq_fastlog2_64(x)	(MultiplyDeBruijnBitPosition64[((unsigned long)(((x) & (-x)) * 0x3f79d71b4cb0a89UL)) >> 27])

INLINE int hq_fastabs(int x)		{	int y = x >> 31;	return (x ^ y) - y;	}
INLINE int hq_count_hbit(int x)		{	int	count = 0;	while(x) {	++count;	x = x&(x-1);	}	return count;}
INLINE UINT32 hq_uptopow2(UINT32 x)	{	x--;x|=x>>1;x|=x>>2;x|=x>>4;x|=x>>8;x|=x>>16;x++;return x;	}
INLINE UINT64 hq_uptopow2(UINT64 x)	{	x--;x|=x>>1;x|=x>>2;x|=x>>4;x|=x>>8;x|=x>>16;x|=x>>32;x++;return x;	}

//memory
#define hq_memcpy(dest, src, count)		memcpy(dest, src, count)
#define hq_memset(buf, c, count)		memset(buf, c, count)
#define hq_memmove(dest, src, count)	memmove(dest, src, count)

//string
#define hq_strcpy(dest, src)	strcpy(dest, src)
#define hq_strcat(dest, src)	strcat(dest, src)
#define hq_strchr(s, c)			strchr(s, c)
#define hq_strcmp(s1, s2)		strcmp(s1, s2)
#define hq_stricmp(s1, s2)		stricmp(s1, s2)
#define hq_strlen(s)			strlen(s)

//sleep
#define hq_usleep(t)	usleep(t)
#define hq_msleep(t)	usleep(t * 1000)
#define hq_sleep(t)		sleep(t)


//file access
const UINT32	HQOPEN_FLAG_R = (1 << 0);	// read flag
const UINT32	HQOPEN_FLAG_W = (1 << 1);	// write flag
const UINT32	HQOPEN_FLAG_A = (1 << 2);	// append flag
const UINT32	HQOPEN_FLAG_C = (1 << 3); // create flag
const UINT32	HQSEEK_FLAG_SET = 0;
const UINT32	HQSEEK_FLAG_CUR = 1;
const UINT32	HQSEEK_FLAG_END = 2;

INT32		hq_open(const char* path, UINT32 flag);
void		hq_close(INT32 handle);
HQOFFSET	hq_seek(INT32 handle, HQOFFSET offset, UINT32 where);
_SIZE		hq_read(INT32 handle, void* buf, _SIZE count);
_SIZE		hq_write(INT32 handle, const void* buf, _SIZE count);

#endif//HQSTDLIB_H_
