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

Action::Action(StepContext *context, stateful_state_t *state,
		const size_t &step_id) :
		StatefulObject(state) {
	this->context = context;
	this->step_id = step_id;
	this->on_activation_changed = NULL;
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
	predicate_state_t predicate_state { state, this->context->getStepState(
			this->step_id) };

	bool condition =
			this->condition != NULL ? this->condition(predicate_state) : true;
	bool activation = this->evaluateActivation(predicate_state);
	bool active = condition && activation;

	this->condition_state.transiting = (condition
			^ this->activation_state.active);
	this->condition_state.active = condition;
	this->condition_state_t0 =
			ACTIVATING(this->condition_state) ?
					state.active_time : this->condition_state_t0;

	this->condition_state.active_time =
			ACTIVATING(this->condition_state) ?
					0 : this->condition_state.active_time;
	this->condition_state.active_time +=
			active ? (state.active_time - this->condition_state_t0) : 0;

	this->activation_state.step_state = predicate_state.step_state;
	this->activation_state.condition_state = this->condition_state;
	this->activation_state.action_state = state;
	this->activation_state.active = active;

	if (active && !state.active) {
		this->activate();
	} else if (!active && state.active) {
		this->shutdown();
	}

	if (this->on_activation_changed != 0) {
		this->on_activation_changed(this->activation_state);
	}
}

} /* namespace sfc */

