#include <util/hq_fsm.h>


HQSM::HQSM(UINT32 nTracerIdx, UINT32 nStateNum) : 
	MemoryManagedBase(nTracerIdx), 
	mStateArray(NULL), 
	mnStateNum(nStateNum),
	mnCurrentState(-1)
{
	mStateArray = (HQISMState**)Managed_Alloc(nStateNum * sizeof(HQISMState*));
	hq_memset(mStateArray, 0, nStateNum * sizeof(HQISMState*));
}

HQSM::~HQSM() {
	Managed_Free(mStateArray);
}

void HQSM::RegisterState(HQISMState* pState) {
	ASSERT(pState->state_idx < mnStateNum);
	mStateArray[pState->state_idx] = pState;
}

void HQSM::SetCurentState(const FSMSTATEIDX& idx) {
	ASSERT(idx < mnStateNum);
	mnCurrentState = idx;
}

HQISMState* HQSM::RunSM(const FSMEVENT& event) {
	HQISMState* pcurrent = mStateArray[mnCurrentState];
	mnCurrentState = pcurrent->onStateChange(event);
	return (mnCurrentState == HQSM::INVALID_STATE)? NULL :  mStateArray[mnCurrentState];
}