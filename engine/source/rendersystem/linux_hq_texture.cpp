#include <rendersystem/hq_texture.h>

RESULT HQTexture::Create(UINT32 width) {
	mType = TYPE_TEXTURE_1D;
	glGenTextures(1, &mIdTex);

}

RESULT HQTexture::Create(UINT32 width, UINT32 height) {
	mType = TYPE_TEXTURE_2D;
	glGenTextures(1, &mIdTex);
}
