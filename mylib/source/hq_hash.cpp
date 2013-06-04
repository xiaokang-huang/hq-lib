#include <hq_hash.h>
#include <hq_os.h>
#include <hq_type.h>

const UINT32 CRYPTTABLE[] = {
	#include <hq_crypttable>
};
const HQHash HASH_EMPTY;

static UINT32 hq_get_hashseed(const char* str, const UINT32 hash_type) {
	UINT8 *key = (UINT8*)str;
	UINT32 seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	UINT32 ch;

	while(*key != 0)
	{
		ch = (UINT32)hq_toupper(*key++);

		seed1 = CRYPTTABLE[(hash_type << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

static void set_hash(HQHash* phash, const char* str) {
	ASSERT(phash != NULL);
	phash->hashA = hq_get_hashseed(str, 0);
	phash->hashB = hq_get_hashseed(str, 1);
}

static INLINE BOOLEAN equal_hash(const HQHash& hashA, const HQHash& hashB) {
	return (hashA.hashA == hashB.hashA && hashA.hashB == hashB.hashB);
}

static UINT32 insert_find_pos(HQHash* ptable, UINT32 lengthcoef, const HQHash& hash) {
	UINT32 pos_start = hash.hashA & ((1 << lengthcoef) - 1);
	UINT32 pos_end = pos_start;
	if (equal_hash(ptable[pos_start], HASH_EMPTY)) {
		ptable[pos_start] = hash;
		return pos_start;
	}
	do {
		if (equal_hash(ptable[pos_start], hash)) {
			return HQHashTable::HASHPOS_INVALID;
		}
		pos_start = (pos_start + 1) & ((1 << lengthcoef) - 1);
	} while (!equal_hash(ptable[pos_start], HASH_EMPTY) && pos_start != pos_end);
	if (pos_start == pos_end)
		return HQHashTable::HASHPOS_INVALID;
	ptable[pos_start] = hash;
	return pos_start;
}

static UINT32 fetch_find_pos(HQHash* ptable, UINT32 lengthcoef, const HQHash& hash) {
	UINT32 pos_start = hash.hashA & ((1 << lengthcoef) - 1);
	UINT32 pos_end = pos_start;
	if (equal_hash(ptable[pos_start], hash)) {
		return pos_start;
	} else if (equal_hash(ptable[pos_start], HASH_EMPTY)) {
		return HQHashTable::HASHPOS_INVALID;
	}
	do {
		pos_start = (pos_start + 1) & ((1 << lengthcoef) - 1);
	} while (!equal_hash(ptable[pos_start], hash) && !equal_hash(ptable[pos_start], HASH_EMPTY) && pos_start != pos_end);
	if (pos_start == pos_end || equal_hash(ptable[pos_start], HASH_EMPTY))
		return HQHashTable::HASHPOS_INVALID;
	return pos_start;
}

HQHashTable::HQHashTable(UINT32 nTrackIdx, UINT32 nTableLength)
	: MemoryManagedBase(nTrackIdx),
	  m_ptable(NULL),
	  m_lengthcoef(0),
	  m_nused(0)
{
	nTableLength = nTableLength >> 1;
	while (nTableLength) {
		++ m_lengthcoef;
		nTableLength = nTableLength >> 1;
	}
	m_ptable = new(nTrackIdx) HQHash[1 << m_lengthcoef];
}

HQHashTable::~HQHashTable() {
	delete[] m_ptable;
}

UINT32 HQHashTable::InsertValue(const char* str) {
	HQHash hash;
	set_hash(&hash, str);
	UINT32 pos = insert_find_pos(m_ptable, m_lengthcoef, hash);
	if (pos != HQHashTable::HASHPOS_INVALID) {
		++ m_nused;
	}
	return pos;
}

UINT32 HQHashTable::GetValue(const char* str) {
	HQHash hash;
	set_hash(&hash, str);
	return fetch_find_pos(m_ptable, m_lengthcoef, hash);
}

UINT32 HQHashTable::DeleteValue(const char* str) {
	HQHash hash;
	set_hash(&hash, str);
	UINT32 pos = fetch_find_pos(m_ptable, m_lengthcoef, hash);
	if (pos != HQHashTable::HASHPOS_INVALID) {
		m_ptable[pos] = HASH_EMPTY;
		-- m_nused;
	}
	return pos;
}

void HQHashTable::Clear() {
	UINT32 length = 1 << m_lengthcoef;
	for (UINT32 i = 0; i < length; ++i) {
		m_ptable[i] = HASH_EMPTY;
	}
}

UINT32 HQHashTable::GetCapacity() const {
	return 1 << m_lengthcoef;
}

UINT32 HQHashTable::GetUsed() const {
	return m_nused;
}

BOOLEAN HQHashTable::IsFull() const {
	return (GetCapacity() == GetUsed());
}

BOOLEAN HQHashTable::IsEmpty() const {
	return (m_nused == 0);
}
