/*
 * EventListener.h
 *
 *  Created on: 17/11/2019
 *      Author: leonardo
 */

#ifndef EVENTLISTENER_H_
#define EVENTLISTENER_H_

#include "sfctypes.h"

namespace sfc {

class EventListener {
public:
	EventListener() {
	}
	virtual ~EventListener() {
	}

	virtual void onActivationChanged(const stateful_state_t &state) = 0;
};

} /* namespace sfc */

#endif /* EVENTLISTENER_H_ */
