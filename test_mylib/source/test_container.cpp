#include "test_container.h"

#include <hq_errorcode.h>
#include <container/hq_node.h>
#include <container/hq_quadnode.h>
#include <container/hq_binarynode.h>
#include <container/hq_concurrentqueue.h>
#include <string.h>

CPPUNIT_TEST_SUITE_REGISTRATION( test_container );

#define RUN_CHECKEQUAL(exp, value_should_be) do {\
	rc = (exp);\
	CPPUNIT_ASSERT_EQUAL((RESULT)value_should_be, rc);\
} while (0)

struct test_nodes {
	HQTreeNode* pNodeThis;
	HQTreeNode* pNodeParent;
	HQTreeNode* pNodeNext;
	HQTreeNode* pNodePrev;
	HQTreeNode* pNodeNextSibling;
	HQTreeNode* pNodePrevSibling;
	HQTreeNode* pNodeFirstChild;
	HQTreeNode* pNodeLastChild;
	void dump_node () const {
		printf("\n{");
		printf("\n\t%p", pNodeThis);
		printf("\n\t%p [%p]", pNodeParent, pNodeThis->GetParent());
		printf("\n\t%p [%p]", pNodeNext, pNodeThis->GetNext());
		printf("\n\t%p [%p]", pNodePrev, pNodeThis->GetPrev());
		printf("\n\t%p [%p]", pNodeNextSibling, pNodeThis->GetNextSibling());
		printf("\n\t%p [%p]", pNodePrevSibling, pNodeThis->GetPrevSibling());
		printf("\n\t%p [%p]", pNodeFirstChild, pNodeThis->GetFirstChild());
		printf("\n\t%p [%p]", pNodeLastChild, pNodeThis->GetLastChild());
		printf("\n}");
	}
};

static void check_node(test_nodes* pnodes, const char* str) {
#define CHECK_ITEM(item1, item2, name) if ((item1) != (item2))	strcat(name_tmp, name);
	static char name_tmp[2048] = {0};
	CHECK_ITEM(pnodes->pNodeThis->GetParent(), pnodes->pNodeParent, "\n\t[parent]");
	CHECK_ITEM(pnodes->pNodeThis->GetNext(), pnodes->pNodeNext, "\n\t[next]");
	CHECK_ITEM(pnodes->pNodeThis->GetPrev(), pnodes->pNodePrev, "\n\t[prev]");
	CHECK_ITEM(pnodes->pNodeThis->GetNextSibling(), pnodes->pNodeNextSibling, "\n\t[nextsibling]");
	CHECK_ITEM(pnodes->pNodeThis->GetPrevSibling(), pnodes->pNodePrevSibling, "\n\t[prevsibling]");
	CHECK_ITEM(pnodes->pNodeThis->GetFirstChild(), pnodes->pNodeFirstChild, "\n\t[firstchild]");
	CHECK_ITEM(pnodes->pNodeThis->GetLastChild(), pnodes->pNodeLastChild, "\n\t[lastchild]");
	if (name_tmp[0]) {
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		printf("\nNode conflict <%s>:%s", str, name_tmp);
		pnodes->dump_node();
		CPPUNIT_ASSERT(false);
	}
#undef CHECK_ITEM
}

static void check_nodelist(test_nodes* pnodes, UINT32 num, const char* str) {
	static char str_tmp[128];
	for (UINT32 i = 0; i < num; ++i) {
		sprintf(str_tmp, "%s<%d>", str, i);
		check_node(pnodes + i, str_tmp);
	}
}

void test_container::setUp() {
}

void test_container::tearDown() {
}

static HQTreeNode* createNodes(UINT32 num, UINT32 idx) {
	HQTreeNode* pnodes = new(idx) HQTreeNode[num];
	return pnodes;
}

static void destroyNodes(HQTreeNode* pnodes, UINT32 num) {
	delete[] pnodes;
}

static void assembleTree(HQTreeNode* pnodes) {
	(pnodes + 1)->Attach(pnodes + 0);
	(pnodes + 2)->Attach(pnodes + 0);
	(pnodes + 3)->Attach(pnodes + 0);
	(pnodes + 4)->Attach(pnodes + 2);
	(pnodes + 5)->Attach(pnodes + 2);
}

static void checkSubtree(HQTreeNode* pnodes, const char* str) {
	test_nodes nodes[] = {
		{	pnodes + 0, NULL, pnodes + 1, NULL, NULL, NULL, pnodes + 1, pnodes + 3	},
		{	pnodes + 1, pnodes + 0, pnodes + 2, pnodes + 0, pnodes + 2, NULL, NULL, NULL	},
		{	pnodes + 2, pnodes + 0, pnodes + 4, pnodes + 1, pnodes + 3, pnodes + 1, pnodes + 4, pnodes + 5	},
		{	pnodes + 3, pnodes + 0, NULL, pnodes + 5, NULL, pnodes + 2, NULL, NULL	},
		{	pnodes + 4, pnodes + 2, pnodes + 5, pnodes + 2, pnodes + 5, NULL, NULL, NULL	},
		{	pnodes + 5, pnodes + 2, pnodes + 3, pnodes + 4, NULL, pnodes + 4, NULL, NULL	},
	};

	check_nodelist(&(nodes[0]), sizeof(nodes) / sizeof(test_nodes), str);
}

static void deassembleTree(HQTreeNode* pnodes, const char* str) {
	char str_temp[64];
	for (UINT32 i = 0; i < 6; ++i) {
		pnodes[i].Detach();
	}
	for (UINT32 i = 0; i < 6; ++i) {
		test_nodes temp = {pnodes + i, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		sprintf(str_temp, "%s[%d]", str, i);
		check_node(&temp, str);
	}
}

void test_container::TC_01_01() {
#define NODENUM 6
	HQTreeNode* pnodes = createNodes(NODENUM, 0);
	CPPUNIT_ASSERT(NULL != pnodes);

	assembleTree(pnodes);
	checkSubtree(pnodes, __FUNCTION__);
	deassembleTree(pnodes, __FUNCTION__);

	destroyNodes(pnodes, NODENUM);
#undef NODENUM
}

void test_container::TC_01_02() {
#define NODENUM 12
	HQTreeNode* pnodes = createNodes(NODENUM, 0);
	CPPUNIT_ASSERT(NULL != pnodes);
	HQTreeNode* ptree1 = pnodes;
	HQTreeNode* ptree2 = pnodes + 6;

	assembleTree(ptree1);
	checkSubtree(ptree1, __FUNCTION__);
	assembleTree(ptree2);
	checkSubtree(ptree2, __FUNCTION__);

	ptree2->Attach(ptree1 + 1);
	test_nodes nodes[] = {
			{	ptree1 + 0, NULL, ptree1 + 1, NULL, NULL, NULL, ptree1 + 1, ptree1 + 3	},
			{	ptree1 + 1, ptree1 + 0, ptree2 + 0, ptree1 + 0, ptree1 + 2, NULL, ptree2 + 0, ptree2 + 0	},
			{	ptree1 + 2, ptree1 + 0, ptree1 + 4, ptree2 + 3, ptree1 + 3, ptree1 + 1, ptree1 + 4, ptree1 + 5	},
			{	ptree1 + 3, ptree1 + 0, NULL, ptree1 + 5, NULL, ptree1 + 2, NULL, NULL	},
			{	ptree1 + 4, ptree1 + 2, ptree1 + 5, ptree1 + 2, ptree1 + 5, NULL, NULL, NULL	},
			{	ptree1 + 5, ptree1 + 2, ptree1 + 3, ptree1 + 4, NULL, ptree1 + 4, NULL, NULL	},

			{	ptree2 + 0, ptree1 + 1, ptree2 + 1, ptree1 + 1, NULL, NULL, ptree2 + 1, ptree2 + 3	},
			{	ptree2 + 1, ptree2 + 0, ptree2 + 2, ptree2 + 0, ptree2 + 2, NULL, NULL, NULL	},
			{	ptree2 + 2, ptree2 + 0, ptree2 + 4, ptree2 + 1, ptree2 + 3, ptree2 + 1, ptree2 + 4, ptree2 + 5	},
			{	ptree2 + 3, ptree2 + 0, ptree1 + 2, ptree2 + 5, NULL, ptree2 + 2, NULL, NULL	},
			{	ptree2 + 4, ptree2 + 2, ptree2 + 5, ptree2 + 2, ptree2 + 5, NULL, NULL, NULL	},
			{	ptree2 + 5, ptree2 + 2, ptree2 + 3, ptree2 + 4, NULL, ptree2 + 4, NULL, NULL	},
		};
	check_nodelist(&(nodes[0]), sizeof(nodes) / sizeof(test_nodes), __FUNCTION__);

	deassembleTree(ptree2, __FUNCTION__);
	deassembleTree(ptree1, __FUNCTION__);

	destroyNodes(pnodes, NODENUM);
#undef NODENUM
}

void test_container::TC_01_03() {
#define NODENUM 12
	HQTreeNode* pnodes = createNodes(NODENUM, 0);
	CPPUNIT_ASSERT(NULL != pnodes);
	HQTreeNode* ptree1 = pnodes;
	HQTreeNode* ptree2 = pnodes + 6;

	assembleTree(ptree1);
	checkSubtree(ptree1, __FUNCTION__);
	assembleTree(ptree2);
	checkSubtree(ptree2, __FUNCTION__);

	ptree2->Attach(ptree1 + 3);
	test_nodes nodes[] = {
			{	ptree1 + 0, NULL, ptree1 + 1, NULL, NULL, NULL, ptree1 + 1, ptree1 + 3	},
			{	ptree1 + 1, ptree1 + 0, ptree1 + 2, ptree1 + 0, ptree1 + 2, NULL, NULL, NULL	},
			{	ptree1 + 2, ptree1 + 0, ptree1 + 4, ptree1 + 1, ptree1 + 3, ptree1 + 1, ptree1 + 4, ptree1 + 5	},
			{	ptree1 + 3, ptree1 + 0, ptree2 + 0, ptree1 + 5, NULL, ptree1 + 2, ptree2 + 0, ptree2 + 0	},
			{	ptree1 + 4, ptree1 + 2, ptree1 + 5, ptree1 + 2, ptree1 + 5, NULL, NULL, NULL	},
			{	ptree1 + 5, ptree1 + 2, ptree1 + 3, ptree1 + 4, NULL, ptree1 + 4, NULL, NULL	},

			{	ptree2 + 0, ptree1 + 3, ptree2 + 1, ptree1 + 3, NULL, NULL, ptree2 + 1, ptree2 + 3	},
			{	ptree2 + 1, ptree2 + 0, ptree2 + 2, ptree2 + 0, ptree2 + 2, NULL, NULL, NULL	},
			{	ptree2 + 2, ptree2 + 0, ptree2 + 4, ptree2 + 1, ptree2 + 3, ptree2 + 1, ptree2 + 4, ptree2 + 5	},
			{	ptree2 + 3, ptree2 + 0, NULL, ptree2 + 5, NULL, ptree2 + 2, NULL, NULL	},
			{	ptree2 + 4, ptree2 + 2, ptree2 + 5, ptree2 + 2, ptree2 + 5, NULL, NULL, NULL	},
			{	ptree2 + 5, ptree2 + 2, ptree2 + 3, ptree2 + 4, NULL, ptree2 + 4, NULL, NULL	},
		};
	check_nodelist(&(nodes[0]), sizeof(nodes) / sizeof(test_nodes), __FUNCTION__);

	deassembleTree(ptree2, __FUNCTION__);
	deassembleTree(ptree1, __FUNCTION__);

	destroyNodes(pnodes, NODENUM);
#undef NODENUM
}

void test_container::TC_02_01() {
	HQQuadNode nodes[5];
	HQQuadNode* pnode = &nodes[0];
	for (UINT32 i = 0; i < 5; ++i) {
		for (UINT32 c = 0; c < 4; ++c) {
			HQQuadNode* ptemp = pnode[i].QuadGetChild(c);
			CPPUNIT_ASSERT(ptemp == NULL);
		}
	}

	for (UINT32 i = 0; i < 4; ++i) {
		pnode[0].QuadAddChild(pnode + i + 1, i);
	}
	for (UINT32 i = 0; i < 4; ++i) {
		HQQuadNode* ptemp = pnode[0].QuadGetChild(i);
		CPPUNIT_ASSERT_EQUAL(ptemp, pnode + i + 1);
	}

	for (UINT32 i = 0; i < 4; ++i) {
		pnode[0].QuadRemoveChild(i);
	}
	for (UINT32 i = 0; i < 5; ++i) {
		for (UINT32 c = 0; c < 4; ++c) {
			HQQuadNode* ptemp = pnode[i].QuadGetChild(c);
			CPPUNIT_ASSERT(ptemp == NULL);
		}
	}
}

void test_container::TC_03_01() {
	HQBinaryNode nodes[3];
	HQBinaryNode* pnode = &nodes[0];
	for (UINT32 i = 0; i < 3; ++i) {
		for (UINT32 c = 0; c < 4; ++c) {
			HQBinaryNode* ptemp = pnode[i].BinaryGetChild(c);
			CPPUNIT_ASSERT(ptemp == NULL);
		}
	}

	for (UINT32 i = 0; i < 2; ++i) {
		pnode[0].BinaryAddChild(pnode + i + 1, i);
	}
	for (UINT32 i = 0; i < 2; ++i) {
		HQBinaryNode* ptemp = pnode[0].BinaryGetChild(i);
		CPPUNIT_ASSERT_EQUAL(ptemp, pnode + i + 1);
	}

	for (UINT32 i = 0; i < 2; ++i) {
		pnode[0].BinaryRemoveChild(i);
	}
	for (UINT32 i = 0; i < 3; ++i) {
		for (UINT32 c = 0; c < 2; ++c) {
			HQBinaryNode* ptemp = pnode[i].BinaryGetChild(c);
			CPPUNIT_ASSERT(ptemp == NULL);
		}
	}
}

#define QUEUESIZE 16
void test_container::TC_04_01() {
	BOOLEAN ret;
	UINT32 value;
	HQConcurrentRingQueue<UINT32> test_queue(QUEUESIZE, 0);

	for (UINT32 i = 0; i < QUEUESIZE; ++i) {
		ret = test_queue.PushBack(i);
		CPPUNIT_ASSERT_EQUAL(TRUE, ret);
	}
	ret = test_queue.PushBack(100);
	CPPUNIT_ASSERT_EQUAL(FALSE, ret);

	for (UINT32 i = 0; i < QUEUESIZE; ++i) {
		ret = test_queue.PopFront(&value);
		CPPUNIT_ASSERT(ret == TRUE);
		CPPUNIT_ASSERT_EQUAL(i, value);
	}
	ret = test_queue.PopFront(&value);
	CPPUNIT_ASSERT(ret == FALSE);
}
#undef QUEUESIZE

#define QUEUESIZE 14
void test_container::TC_04_02() {
	BOOLEAN ret;
	UINT32 value;
	HQConcurrentRingQueue<UINT32> test_queue(QUEUESIZE, 0);

	value = test_queue.GetSize();
	CPPUNIT_ASSERT_EQUAL((UINT32)16, value);

	for (UINT32 i = 0; i < QUEUESIZE; ++i) {
		ret = test_queue.PushBack(i);
		CPPUNIT_ASSERT_EQUAL(TRUE, ret);
	}

	for (UINT32 i = 0; i < QUEUESIZE; ++i) {
		ret = test_queue.PopFront(&value);
		CPPUNIT_ASSERT(ret == TRUE);
		CPPUNIT_ASSERT_EQUAL(i, value);
	}
	ret = test_queue.PopFront(&value);
	CPPUNIT_ASSERT(ret == FALSE);
}
#undef QUEUESIZE

#define QUEUESIZE 16
#define MULCOEF	4
static void* push_function(void* param) {
	HQConcurrentRingQueue<UINT32>* pqueue = (HQConcurrentRingQueue<UINT32>*)param;
	for (UINT32 i = 0; i < (QUEUESIZE * MULCOEF + QUEUESIZE); ++i) {
		while (pqueue->PushBack(i) == FALSE) {}
	}
	return NULL;
}

static void* pop_function(void* param) {
	HQConcurrentRingQueue<UINT32>* pqueue = (HQConcurrentRingQueue<UINT32>*)param;
	UINT32 value = 0;
	for (UINT32 i = 0; i < (QUEUESIZE * MULCOEF); ++i) {
		while (pqueue->PopFront(&value) == FALSE) {}
		CPPUNIT_ASSERT_EQUAL(i, value);
	}
	return NULL;
}

void test_container::TC_04_03() {
	BOOLEAN ret;
	UINT32 used;
	HQConcurrentRingQueue<UINT32> test_queue(QUEUESIZE, 0);

	HQThread push_thread;
	HQThread pop_thread;

	push_thread.Create(push_function, &test_queue);
	pop_thread.Create(pop_function, &test_queue);

	push_thread.Destroy(NULL);
	pop_thread.Destroy(NULL);

	ret = test_queue.IsFull();
	CPPUNIT_ASSERT_EQUAL(TRUE, ret);
	used = test_queue.GetUsed();
	CPPUNIT_ASSERT_EQUAL((UINT32)QUEUESIZE, used);
}
#undef QUEUESIZE
