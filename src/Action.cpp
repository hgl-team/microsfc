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
	this->step_id = 0;
	this->handlers = { NULL, 0 };
	this->condition = NULL;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = StatefulObject();
}

Action::Action(const size_t &step_id) :
		StatefulObject() {
	this->step_id = step_id;
	this->handlers = { NULL, 0 };
	this->condition = NULL;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = StatefulObject();
}
Action::Action(const size_t &step_id, activation_predicate_fnc condition) :
		StatefulObject() {
	this->step_id = step_id;
	this->handlers = { NULL, 0 };
	this->condition = condition;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = StatefulObject();
}

Action::Action(const size_t &step_id, array<action_state_handler_t> handlers) {
	this->step_id = step_id;
	this->handlers = handlers;
	this->condition = NULL;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = StatefulObject();
}
Action::Action(const size_t &step_id, activation_predicate_fnc condition, array<action_state_handler_t> handlers) {
	this->step_id = step_id;
	this->handlers = handlers;
	this->condition = condition;

	this->activation_state = { { 0, false, false }, { 0, false, false }, { 0,
			false, false }, false };
	this->condition_state = StatefulObject();
}

Action::~Action() {
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

	this->activation_state.step_state = predicate_state.step_state;
	this->activation_state.condition_state = *(this->condition_state.getState());
	this->activation_state.action_state = *(this->getState());
	this->activation_state.active = this->condition_state.getState()->activated;
}

void Action::onTick(const sfc::ulong_t & delta, StepContext * const& context) {
	StatefulObject::onTick(delta);
	// Update the step state.
	this->activation_state.step_state = context->getStepState(this->step_id);

	for(size_t i = 0; i < this->handlers.size; i++) {
		switch (ARRAY_GET(this->handlers, i)->action_state)
		{
		case ACTION_STATE_ACTIVATING:
			if(PTR_ACTIVATING(this->getState())) {
				ARRAY_GET(this->handlers, i)->function(this->activation_state);
			}
			break;
		case ACTION_STATE_ACTIVE:
			if(this->getState()->active) {
				ARRAY_GET(this->handlers, i)->function(this->activation_state);
			}
			break;
		case ACTION_STATE_DEACTIVATING:
			if(PTR_DEACTIVATING(this->getState())) {
				ARRAY_GET(this->handlers, i)->function(this->activation_state);
			}
			break;
		case ACTION_STATE_INACTIVE:
			if(!(this->getState()->active)) {
				ARRAY_GET(this->handlers, i)->function(this->activation_state);
			}
			break;
		default: break;
		}
	}
}

bool Action::evaluateActivation(const sfc::predicate_state_t &state) {
	return true;
}

} /* namespace sfc */

