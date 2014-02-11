#include <rendersystem/hq_texture.h>
#include <hq_errorcode.h>

RESULT HQTexture::Destory() {
	if (mIdTex != 0) {
		glDeleteTextures(1, &mIdTex);
		mIdTex	= 0;
		mType	= TYPE_TEXTURE_UNKNOWN;
		mWidth	= 0;
		mHeight	= 0;
	}
	return HQRESULT_SUCCESS;
}

RESULT HQTexture::CreateFromImage(HQImage* img) {
	Destory();

	bool failed = false;

	glGenTextures(1, &mIdTex);
	glBindTexture(GL_TEXTURE_2D, mIdTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	HQBuffer* pbuf = img->GetBuffer();
	switch (img->GetCompressType()) {
	case HQImage::COMPRESS_TYPE_DXT5:
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 
			img->GetWidth(), img->GetHeight(), 0, (GLsizei)(pbuf->GetSize()), pbuf->GetBuf());
		break;
	default:
		failed = true;
	}

	if (failed == true) {
		Destory();
	}

	return HQRESULT_SUCCESS;
}


