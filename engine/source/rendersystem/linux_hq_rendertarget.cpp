#include <hq_rendertarget.h>
/*	======================================================================	*/
//	render target base class
/*	======================================================================	*/
RESULT HQRenderTarget::AttachToSystem() {
	glBindFramebuffer(GL_FRAMEBUFFER, mIdFBO);
}

RESULT HQRenderTarget::DetachFromSystem() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*	======================================================================	*/
//	Texture render target
/*	======================================================================	*/
RESULT HQTextureRenderTarget::Create(UINT32 width, UINT32 height, UINT32 buffertype) {
	glGenFramebuffers(1, &mIdFBO);
	if (mIdFBO == 0) {
		return HQRESULT_RENDERTARGET_ERROR;
	}

}

RESULT HQTextureRenderTarget::Destory() {
	if (mIdFBO == 0)	return HQRESULT_SUCCESS;
}

HQRenderTarget::Type HQTextureRenderTarget::GetType() {
	return HQRenderTarget::TYPE_TEXTURE;
}

/*	======================================================================	*/
//	framebuffer render target
/*	======================================================================	*/
