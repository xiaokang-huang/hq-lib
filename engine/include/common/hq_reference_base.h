#ifndef _HQREFERENCEBASE_H_
#define _HQREFERENCEBASE_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_thread.h>

class HQReferenceBase {
PROTECTED:
	void Attach() {
		mnReferenceCnt.Add(1);
	}
	void Detach() {
		ASSERT(mnReferenceCnt.Get() > 0);
		mnReferenceCnt.Sub(1);
	}
	INT32 Get() {
		return mnReferenceCnt.Get();
	}
PRIVATE:
	HQAtomic mnReferenceCnt;
};

#endif//_HQREFERENCEBASE_H_
