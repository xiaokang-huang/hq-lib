#ifndef _HQTEXTURE_H_
#define _HQTEXTURE_H_

#include <hq_type.h>
#include <common/hq_reference_base.h>

class HQTexture : PUBLIC HQReferenceBase {
PUBLIC:
	typedef enum {
		TYPE_TEXTURE_UNKNOWN = 0,
		TYPE_TEXTURE_1D,
		TYPE_TEXTURE_2D,
	} Type;
PUBLIC:
	HQTexture() : mIdTex(0), mType(TYPE_TEXTURE_UNKNOWN), mWidth(0), mHeight(0) {}
	~HQTexture() {}
PUBLIC:
	RESULT Create(UINT32 width);
	RESULT Create(UINT32 width, UINT32 height);
PRIVATE:
	UINT32	mIdTex;
	Type 	mType;
	UINT32	mWidth;
	UINT32	mHeight;
};

#endif//_HQTEXTURE_H_