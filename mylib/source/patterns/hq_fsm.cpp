#include <patterns/hq_fsm.h>

HQFSM::HQFSM(UINT32 nTracerIdx, UINT32 nStateNum) : MemoryManagedBase(nTracerIdx), mnStateNum(nStateNum) {
	mStateArray = Managed_Alloc(nStateNum * sizeof(HQIFSMState*));
}

HQFSM::~HQFSM() {
	Managed_Free(mStateArray);
}

BOOLEAN HQFSM::RegisterState(const FSMSTATEIDX& idx, HQIFSMState* pState) {
	mStateArray[pState->state_idx] = pState;
}
