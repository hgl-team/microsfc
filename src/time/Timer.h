/*
 * Timer.h
 *
 *  Created on: 16/11/2019
 *      Author: leonardo
 */

#ifndef TIME_TIMER_H_
#define TIME_TIMER_H_

#include "ClockListener.h"

namespace sfc {

class Timer: public ClockListener {
private:
	ulong_t period;
	bool continous;
	timer_state_t timer_state;
public:
	Timer();
	Timer(const ulong_t &period, const bool &continous);
	Timer(const ulong_t &period);
	virtual ~Timer();

	virtual void onTick(const sfc::ulong_t &time);

	virtual void enable();
	virtual void reset();
	virtual void disable();
	virtual timer_state_t * getState();
};

} /* namespace sfc */

#endif /* TIME_TIMER_H_ */
