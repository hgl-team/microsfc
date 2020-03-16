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
