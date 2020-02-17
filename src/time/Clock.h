/*
 * Clock.h
 *
 *  Created on: 16/11/2019
 *      Author: leonardo
 */

#ifndef TIME_CLOCK_H_
#define TIME_CLOCK_H_

#include "../sfctypes.h"
#include "ClockListener.h"

namespace sfc {

/**
 * Handles time events.
 */
class Clock {
private:
	ulong_t current_time;
	array<ClockListener*> listeners;
public:
	/**
	 *
	 */
	Clock();
	/**
	 * Setup a new clock with given listeners.
	 * @param listeners array of time event listeners.
	 */
	Clock(const array<ClockListener*> &listeners);
	virtual ~Clock();

	void tick(const ulong_t &time);

	ulong_t getCurrentTime() const;
	const array<ClockListener*>& getListeners() const;
	void offset(const int & value);
};

} /* namespace sfc */

#endif /* TIME_CLOCK_H_ */
