/*
Copyright 2020 Jerson Leonardo Huerfano Romero

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
