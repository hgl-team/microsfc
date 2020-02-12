/*
 * ClockListener.h
 *
 *  Created on: 16/11/2019
 *      Author: leonardo
 */

#ifndef TIME_CLOCKLISTENER_H_
#define TIME_CLOCKLISTENER_H_

#include "../sfctypes.h"

namespace sfc {

class ClockListener {
public:
	ClockListener() { }
	virtual ~ClockListener() { }
	/**
	 * Invoked when clock raises a new time event.
	 * @param delta Time elapsed since last time event.
	 */
	virtual void onTick(const ulong_t &delta) { }
};

} /* namespace sfc */

#endif /* TIME_CLOCKLISTENER_H_ */
