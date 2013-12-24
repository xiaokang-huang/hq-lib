#include <image/hq_image.h>
#include <hq_errorcode.h>

const UINT32 FORCC_DDS = FOURCC('D', 'D', 'S', ' ');

/* ====================================================================== */
// DDS loader
/* ====================================================================== */
enum DDSD_FLAGS {
	DDSD_CAPS = 0x00000001l,
	DDSD_HEIGHT = 0x00000002l,
	DDSD_WIDTH = 0x00000004l,
	DDSD_PITCH = 0x00000008l,
	DDSD_BACKBUFFERCOUNT = 0x00000020l,
	DDSD_ZBUFFERBITDEPTH = 0x00000040l,
	DDSD_ALPHABITDEPTH = 0x00000080l,
	DDSD_LPSURFACE = 0x00000800l,
	DDSD_PIXELFORMAT = 0x00001000l,
	DDSD_CKDESTOVERLAY = 0x00002000l,
	DDSD_CKDESTBLT = 0x00004000l,
	DDSD_CKSRCOVERLAY = 0x00008000l,
	DDSD_CKSRCBLT = 0x00010000l,
	DDSD_MIPMAPCOUNT = 0x00020000l,
	DDSD_REFRESHRATE = 0x00040000l,
	DDSD_LINEARSIZE = 0x00080000l,
	DDSD_TEXTURESTAGE = 0x00100000l,
	DDSD_FVF = 0x00200000l,
	DDSD_SRCVBHANDLE = 0x00400000l,
	DDSD_DEPTH = 0x00800000l,
};


struct _ddcaps2 {		// totally 16 BYTES
	UINT32 dwCaps1;
	UINT32 dwCaps2;
	UINT32 dwReserved[2];
};

struct _ddpixelformat	// totally 32 BYTES
{
	UINT32	dwSize;
	UINT32	dwFlags;
	UINT32	dwFourCC;
	UINT32	dwRGBBitCount;
	UINT32	dwRBitMask;
	UINT32	dwGBitMask;
	UINT32	dwBBitMask;
	UINT32	dwRGBAlphaBitMask;
};

struct _dds_surface_header {
	UINT32 dwSize;					//Size of structure. This member must be set to 124.
	UINT32 dwFlags;					//Flags to indicate valid fields. Always include DDSD_CAPS, DDSD_PIXELFORMAT, DDSD_WIDTH, DDSD_HEIGHT.
	UINT32 dwHeight;				//Height of the main image in pixels
	UINT32 dwWidth;					//Width of the main image in pixels
	UINT32 dwPitchOrLinearSize;		//For uncompressed formats, this is the number of bytes per scan line (DWORD> aligned) for the main image. dwFlags should include DDSD_PITCH in this case. For compressed formats, this is the total number of bytes for the main image. dwFlags should be include DDSD_LINEARSIZE in this case.
	UINT32 dwDepth;					//For volume textures, this is the depth of the volume. dwFlags should include DDSD_DEPTH in this case.
	UINT32 dwMipMapCount;			//For items with mipmap levels, this is the total number of levels in the mipmap chain of the main image. dwFlags should include DDSD_MIPMAPCOUNT in this case.
	UINT32 dwReserved1[11];			//Unused	
	_ddpixelformat ddpfPixelFormat;	//32-byte value that specifies the pixel format structure.
	_ddcaps2 ddsCaps;				//16-byte value that specifies the capabilities structure.
	UINT32 dwReserved2;				//Unused
};

struct _dds_file {
	UINT32 				magic_num;
	_dds_surface_header header;
	char				data[0];
};

static void* _load_dds(INT32 fp, UINT32 nTrackId, UINT32* pWidth, UINT32* pHeight) {
	void* pBuf = NULL;
	_SIZE read_size = 0;
	_dds_file dds;

	read_size = hq_read(fp, &dds, (_SIZE)sizeof(_dds_file));
	if (read_size < (_SIZE)sizeof(_dds_file)) {
		return pBuf;
	}
	ASSERT(FORCC_DDS == dds.magic_num);
	ASSERT(dds.header.dwSize == 124);
	ASSERT(dds.header.dwFlags & DDSD_LINEARSIZE);

	*pWidth		= dds.header.dwWidth;
	*pHeight	= dds.header.dwHeight;

	UINT32 data_size = dds.header.dwPitchOrLinearSize;
	pBuf = MemAlloc(nTrackId, data_size);
	read_size = hq_read(fp, pBuf, (_SIZE)(data_size));
	if (read_size < data_size) {
		MemFree(pBuf);
		pBuf = NULL;
		*pWidth = 0;
		*pHeight = 0;
	}

	return pBuf;
}

/* ====================================================================== */
// DDS loader
/* ====================================================================== */
RESULT HQImage::Create(UINT32 width, UINT32 height, CompressType type) {
	Destory();
	mWidth = width;
	mHeight = height;
	mType = type;

	return HQRESULT_SUCCESS;
}

RESULT HQImage::Destory() {
	mWidth = 0;
	mHeight = 0;
	mType = COMPRESS_TYPE_UNKNOWN;
	if (mBuf) {
		Managed_Free(mBuf);
		mBuf = NULL;
	}

	return HQRESULT_SUCCESS;
}
RESULT HQImage::LoadFromFile(const char* path) {
	RESULT ret = HQRESULT_SUCCESS;
	UINT32 magic = 0;
	_SIZE read_size = 0;

	INT32 fp = hq_open(path, HQOPEN_FLAG_R);
	if (fp == 0) {
		ret = HQRESULT_IMAGE_OPENFILE_FAILED;
		goto error;
	}

	read_size = hq_read(fp, &magic, (_SIZE)sizeof(magic));
	if (read_size < (_SIZE)sizeof(magic)) {
		ret = HQRESULT_IMAGE_OPENFILE_FAILED;
		goto error;
	}
	hq_seek(fp, 0, HQSEEK_FLAG_SET);

	switch (magic) {
	case FORCC_DDS:
		mBuf = _load_dds(fp, m_nTracerIdx, &mWidth, &mHeight);
		break;
	}

error:
	if (fp) {
		close(fp);
	}
	return ret;
}
