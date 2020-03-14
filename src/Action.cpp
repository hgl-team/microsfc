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
#include "Action.h"

namespace sfc {

Action::Action() :
		StatefulObject() {
	this->step_id = 0;
	this->condition = NULL;
	this->condition_state = StatefulObject();
}

Action::Action(const size_t &step_id) :
		StatefulObject() {
	this->step_id = step_id;
	this->condition = NULL;
	this->condition_state = StatefulObject();
}
Action::Action(const size_t &step_id, activation_predicate_fnc condition) :
		StatefulObject() {
	this->step_id = step_id;
	this->condition = condition;
	this->condition_state = StatefulObject();
}

Action::Action(const size_t &step_id, array<state_handler_t> handlers) 
		: StatefulObject(handlers) {
	this->step_id = step_id;
	this->condition = NULL;
	this->condition_state = StatefulObject();
}
Action::Action(const size_t &step_id, activation_predicate_fnc condition, array<state_handler_t> handlers)
		: StatefulObject(handlers) {
	this->step_id = step_id;
	this->condition = condition;
	this->condition_state = StatefulObject();
}

Action::~Action() { }

bool Action::evaluateActivation(const sfc::predicate_state_t &state) {
	return true;
}

void Action::evaluate(StepContext * const& context) {
	predicate_state_t predicate_state { 
		*(this->getState()), 
		context->getStepState(this->step_id)
	};

	// Evaluate the user defined predicate. 
	bool condition = this->condition != NULL ? this->condition(predicate_state) : true;
	// Evaluate the action-specific predicate.
	bool activation = this->evaluateActivation(predicate_state);
	
	if(condition && activation) {
		this->condition_state.activate();
	} else {
		this->condition_state.shutdown();
	}

	if (PTR_ACTIVATED(this->condition_state.getState()) && !PTR_ACTIVATED(this->getState())) {
		this->activate();
	} else if (!PTR_ACTIVATED(this->condition_state.getState()) && PTR_ACTIVATED(this->getState())) {
		this->shutdown();
	}
}

} /* namespace sfc */

