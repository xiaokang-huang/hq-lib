#ifndef HQINTERNALENGINE_H_
#define HQINTERNALENGINE_H_

/*
 * internal engine
 * working flow:
 * FRAME0: (CALCU_LOGICAL) --> (CALCU_PHISICAL) --> (UPDATE_BACK)
 *
 * FRAME1: (FLIP_BACK) --> (CALCU_RENDER_QUEUE) --> (RENDER) --> (HANDLE_EVNET)
 */

class InternalEngine {
	void internal_handle_event();
	void internal_calcu_logical();
	void internal_calcu_phisical();
	void internal_update_back();
	void internal_flip_back();
	void internal_calcu_render_queue();
	void internal_render();
};


#endif//HQINTERNALENGINE_H_
