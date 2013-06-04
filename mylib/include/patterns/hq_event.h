#ifndef HQEVENT_H_
#define HQEVENT_H_

#include <hq_type.h>

class HQIEventHandler;
class HQIEvent;

class HQIEventHandler {
PUBLIC:
	virtual ~HQIEventHandler()	{}
	virtual void* ActionPerformed(void* param) = 0;
};

class HQIEvent {

};

#endif//HQEVENT_H_
