/*
 * Clock.cpp
 *
 *  Created on: 16/11/2019
 *      Author: leonardo
 */

#include "Clock.h"

namespace sfc {

Clock::Clock(const array<ClockListener*> &listeners) {
	this->listeners = listeners;
	this->current_time = 0;
}

ulong_t Clock::getCurrentTime() const {
	return current_time;
}

void Clock::tick(const ulong_t &time) {
	ulong_t delta =
			(this->current_time < time) ?
					time - this->current_time :
					(ULONG_MAX - this->current_time + 1) + time;

	this->current_time = time;

	for (size_t i = 0; i < this->listeners.size; i++) {
		ClockListener* listener = *ARRAY_GET(this->listeners, i);
		listener->onTick(delta);
	}
}

Clock::Clock() {
	this->listeners = { NULL, 0 };
	this->current_time = 0;
}

const array<ClockListener*>& Clock::getListeners() const {
	return listeners;
}

Clock::~Clock() {
}

void Clock::offset(const int &value) {
	this->current_time += value;
}

} /* namespace sfc */
