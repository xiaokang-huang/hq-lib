#ifndef _HQIMAGE_H_
#define _HQIMAGE_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>
#include <container/hq_buffer.h>

const RESULT HQRESULT_IMAGE_OPENFILE_FAILED = -1;

class HQImage : PUBLIC MemoryManagedBase {
PUBLIC:
	typedef enum {
		COMPRESS_TYPE_UNKNOWN = 0,
		COMPRESS_TYPE_NOCOMPRESS,
		COMPRESS_TYPE_DXT1 = 0x100,
		COMPRESS_TYPE_DXT2,
		COMPRESS_TYPE_DXT3,
		COMPRESS_TYPE_DXT4,
		COMPRESS_TYPE_DXT5,
	} CompressType;
	struct Info {
		UINT32 nTracerIdx;
	};
PUBLIC:
	HQImage(Info* info) : MemoryManagedBase(info->nTracerIdx), mWidth(0), mHeight(0), mDataSize(0), mType(COMPRESS_TYPE_UNKNOWN), mBuf(NULL) {}
	~HQImage();
PUBLIC:
	RESULT Create(UINT32 width, UINT32 height, CompressType type);
	RESULT Destory();
	RESULT LoadFromFile(const char* path);

	UINT32 GetWidth() {	return mWidth;	}
	UINT32 GetHeight() {	return mHeight;	}
	CompressType GetCompressType() {	return mType;	}
PRIVATE:
	UINT32			mWidth;
	UINT32			mHeight;
	UINT32			mDataSize;
	CompressType 	mType;
	void*			mBuf;
};

#endif//_HQIMAGE_H_
