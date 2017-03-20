#include "test_image.h"
CPPUNIT_TEST_SUITE_REGISTRATION( test_image );

#include <image/hq_image.h>
#include <hq_errorcode.h>

void test_image::setUp() {
}

void test_image::tearDown() {
}

void test_image::TC_01_01() {
	const char* test_file = "../../../testData/image/test.dds";
	HQImage::Info info = { 0 };
	HQImage img(&info);
	RESULT ret = img.LoadFromFile(test_file);
	CPPUNIT_ASSERT_EQUAL(ret, HQRESULT_SUCCESS);
	UINT32 width = img.GetWidth();
	UINT32 height = img.GetHeight();
	HQImage::CompressType type = img.GetCompressType();
	CPPUNIT_ASSERT(width == (UINT32)512);
	CPPUNIT_ASSERT_EQUAL(height, (UINT32)512);
	CPPUNIT_ASSERT_EQUAL(type, HQImage::COMPRESS_TYPE_DXT5);
}

void test_image::TC_01_02() {
	const char* test_file = "test_data/image/test_non.dds";
	HQImage::Info info = { 0 };
	HQImage img(&info);
	RESULT ret = img.LoadFromFile(test_file);
	CPPUNIT_ASSERT_EQUAL(ret, HQRESULT_IMAGE_OPENFILE_FAILED);
}