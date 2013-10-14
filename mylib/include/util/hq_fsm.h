#ifndef _HQFSM_H_
#define _HQFSM_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_memory_mgr.h>

class HQISMState;

class HQSM : PUBLIC MemoryManagedBase {
PUBLIC:
	typedef UINT32 FSMSTATEIDX;
	typedef UINT32 FSMEVENT;

	const static FSMSTATEIDX INVALID_STATE = -1;

PUBLIC:
	HQSM(UINT32 nTracerIdx, UINT32 nStateNum);
	~HQSM();
	void RegisterState(HQISMState* pState);
	void SetCurentState(const FSMSTATEIDX& idx);
	HQISMState* RunSM(const FSMEVENT& event);

PRIVATE:
	HQISMState**	mStateArray;
	UINT32			mnStateNum;
	FSMSTATEIDX		mnCurrentState;
};

class HQISMState : PUBLIC MemoryAllocatorBase {
PUBLIC:
	HQISMState();
	virtual ~HQISMState();
	virtual HQSM::FSMSTATEIDX onStateChange(const HQSM::FSMEVENT& event);
	const HQSM::FSMSTATEIDX state_idx;
};

#endif//_HQFSM_H_
