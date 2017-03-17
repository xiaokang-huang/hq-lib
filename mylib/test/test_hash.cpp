#include "test_hash.h"

#include <hq_hash.h>

CPPUNIT_TEST_SUITE_REGISTRATION( test_hash );

const char* test_str[] = {
	"hello",
	"my",
	"name",
	"is",
	"huang",
	"xiaokang"
};
const UINT32 test_len = sizeof(test_str) / sizeof(test_str[0]);

void test_hash::setUp() {
}

void test_hash::tearDown() {
}

void test_hash::TC_01_01() {
	HQHashTable hash_table(0);

	CPPUNIT_ASSERT_EQUAL((UINT32)0, hash_table.GetUsed());
	CPPUNIT_ASSERT_EQUAL((UINT32)1024, hash_table.GetCapacity());

	UINT32 pos_list[test_len];
	for (UINT32 i = 0; i < test_len; ++i) {
		pos_list[i] = hash_table.InsertValue(test_str[i]);
		CPPUNIT_ASSERT_EQUAL(i+1, hash_table.GetUsed());
	}
	for (UINT32 i = 0; i < test_len; ++i) {
		UINT32 pos = hash_table.GetValue(test_str[i]);
		CPPUNIT_ASSERT_EQUAL(pos_list[i], pos);
	}

	for (UINT32 i = 0; i < test_len; ++i) {
		UINT32 pos = hash_table.DeleteValue(test_str[i]);
		CPPUNIT_ASSERT_EQUAL(pos_list[i], pos);
		pos = hash_table.GetValue(test_str[i]);
		CPPUNIT_ASSERT_EQUAL(HQHashTable::HASHPOS_INVALID, pos);
		CPPUNIT_ASSERT_EQUAL(test_len - i - 1, hash_table.GetUsed());
	}
}

void test_hash::TC_01_02() {
	HQHashTable hash_table(0);

	const char* pstr = "hello";
	UINT32 pos;
	pos = hash_table.InsertValue(pstr);
	CPPUNIT_ASSERT(pos != HQHashTable::HASHPOS_INVALID);
	pos = hash_table.InsertValue(pstr);
	CPPUNIT_ASSERT_EQUAL(HQHashTable::HASHPOS_INVALID, pos);
}
