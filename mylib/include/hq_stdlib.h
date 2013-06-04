#ifndef HQSTDLIB_H_
#define HQSTDLIB_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#endif//HQSTDLIB_H_
