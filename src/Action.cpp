/*
 * Action.cpp
 *
 *  Created on: 17/11/2019
 *      Author: leonardo
 */

#include "Action.h"

namespace sfc {

Action::Action() :
		StatefulObject() {
	this->context = NULL;
	this->step_id = 0;
	this->on_activation_changed = NULL;
	this->condition = NULL;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = { 0, false, false };
	this->condition_state_t0 = 0;
}

Action::Action(const size_t &step_id) :
		StatefulObject() {
	this->context = NULL;
	this->step_id = step_id;
	this->on_activation_changed = NULL;
	this->condition = NULL;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = { 0, false, false };
	this->condition_state_t0 = 0;
}

Action::Action(const size_t &step_id, action_fnc on_state_changed) {
	this->context = NULL;
	this->step_id = step_id;
	this->on_activation_changed = on_state_changed;
	this->condition = NULL;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = { 0, false, false };
	this->condition_state_t0 = 0;
}

activation_predicate_fnc Action::getCondition() const {
	return condition;
}

void Action::setCondition(activation_predicate_fnc condition) {
	this->condition = condition;
}

action_fnc Action::getOnActivationChanged() const {
	return on_activation_changed;
}

void Action::setOnActivationChanged(action_fnc onActivationChanged) {
	on_activation_changed = onActivationChanged;
}

Action::~Action() {
	// TODO Auto-generated destructor stub
}

void Action::stateChanged(const sfc::stateful_state_t &state) {
	// stateful_state_t step_state;
	// Join between current action state and step state.
	predicate_state_t predicate_state { 
		state, 
		this->context->getStepState(this->step_id) 
	};
	// Evaluate the user defined predicate. 
	bool condition = this->condition != NULL ? this->condition(predicate_state) : true;
	// Evaluate the action-specific predicate.
	bool activation = this->evaluateActivation(predicate_state);

	// Action is activated if both condition and activation are set. 
	this->condition_state.activated = condition && activation;
	this->condition_state.transiting = (condition ^ this->activation_state.active);
	// Keep the condition value in the active state.
	this->condition_state.active = condition;

	this->condition_state_t0 = ACTIVATING(this->condition_state) ?
					state.active_time : this->condition_state_t0;
	this->condition_state.active_time = ACTIVATING(this->condition_state) ?
					0 : this->condition_state.active_time;
	this->condition_state.active_time +=
			this->condition_state.activated ? (state.active_time - this->condition_state_t0) : 0;

	// Activation state keeps the step state, condition state, and 
	// previous action state, and also the activation signal.  
	this->activation_state.step_state = predicate_state.step_state;
	this->activation_state.condition_state = this->condition_state;
	this->activation_state.action_state = state;
	this->activation_state.active = this->condition_state.activated;

	if (this->condition_state.activated && !state.active) {
		this->activate();
	} else if (!(this->condition_state.activated) && state.active) {
		this->shutdown();
	}

	if (this->on_activation_changed != 0) {
		this->on_activation_changed(this->activation_state);
	}
}

void Action::setStepContext(StepContext * context) {
	this->context = context;
}

bool Action::evaluateActivation(const sfc::predicate_state_t &state) {
	return true;
}

} /* namespace sfc */

