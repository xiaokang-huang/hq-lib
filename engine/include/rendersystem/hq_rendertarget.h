#ifndef _HQRENDERTARGET_H_
#define _HQRENDERTARGET_H_

#include <hq_type.h>

const RESULT HQRESULT_RENDERTARGET_ERROR   = -1;
const RESULT HQRESULT_RENDERTARGET_SUCCESS = 0;

class HQRenderTarget {
PUBLIC:
	typedef enum {
		TYPE_FRAMEBUFFER,
		TYPE_TEXTURE,
	} Type;

	static const UINT32 COLOR_BUFFER_0	= (1 << 0);
	static const UINT32 COLOR_BUFFER_1	= (1 << 1);
	static const UINT32 COLOR_BUFFER_2	= (1 << 2);
	static const UINT32 COLOR_BUFFER_3	= (1 << 3);
	static const UINT32 DEPTH_BUFFER	= (1 << 4);
	static const UINT32 STENCIL_BUFFER	= (1 << 5);
PUBLIC:
	HQRenderTarget() : mWidth(0), mHeight(0), mBufferType(0), mIdFBO(0) {}
	virtual ~HQRenderTarget()	{}
PUBLIC:
	virtual RESULT		Create(UINT32 width, UINT32 height, UINT32 buffertype) = 0;
	virtual RESULT		Destory() = 0;
	virtual Type	GetType() = 0;
PUBLIC:
	RESULT AttachToSystem();
	RESULT DetachFromSystem();
PROTECTED:
	UINT32 mIdFBO;
	UINT32 mWidth;
	UINT32 mHeight;
	UINT32 mBufferType;
};

class HQTextureRenderTarget : PUBLIC HQRenderTarget {
PUBLIC:
	virtual ~HQTextureRenderTarget();
PUBLIC:
	virtual RESULT	Create(UINT32 width, UINT32 height, UINT32 buffertype);
	virtual RESULT	Destory();
	virtual Type	GetType();
};

#endif//_HQRENDERTARGET_H_