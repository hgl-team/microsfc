/*
 * Timer.cpp
 *
 *  Created on: 16/11/2019
 *      Author: leonardo
 */

#include "Timer.h"

namespace sfc {

Timer::Timer() {
	this->period = period;
	this->continous = continous;
	this->timer_state = { 0, false, false };
}

Timer::~Timer() {
}

Timer::Timer(const ulong_t &period, const bool &continous) {
	this->period = period;
	this->continous = continous;
	this->timer_state = {0, false, false};
}

Timer::Timer(const ulong_t &period) {
	this->period = period;
	this->continous = false;
	this->timer_state = {0, false, false};
}

void Timer::onTick(const sfc::ulong_t &delta) {
	if (this->timer_state.enabled) {
		ulong_t elapsed_time = this->timer_state.current_time + delta;
		this->timer_state.current_time = elapsed_time % (this->period);
		this->timer_state.interrupted = elapsed_time >= this->period;
		this->timer_state.enabled = this->timer_state.enabled
				&& (this->continous || (!(this->timer_state.interrupted)));
	}
	if(this->timer_state.interrupted) {
		this->timer_state.interrupted = true;
	}
}

void Timer::enable() {
	if (!(this->timer_state.enabled = true)) {
		this->reset();
		this->timer_state.enabled = true;
	}
}
void Timer::reset() {
	this->timer_state.current_time = 0l;
	this->timer_state.interrupted = false;
}

void Timer::disable() {
	this->reset();
	this->timer_state.enabled = false;
}

timer_state_t * Timer::getState() {
	return &(this->timer_state);
}

} /* namespace sfc */

