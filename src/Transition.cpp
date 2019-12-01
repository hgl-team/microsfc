/*
 * Transition.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "Transition.h"

namespace sfc {

Transition::Transition(StepContext *context,
		const array<int> &input_step_ids, const array<int> &output_step_ids,
		predicate_fnc condition) {
	this->context = context;
	this->input_step_ids = input_step_ids;
	this->output_step_ids = output_step_ids;
	this->condition = condition;
}

Transition::~Transition() {
}

Transition::Transition() {
	this->context = NULL;
	this->input_step_ids = { NULL, 0 };
	this->output_step_ids = { NULL, 0 };
	this->condition = NULL;
}

void Transition::onActivationChanged(const sfc::stateful_state_t &state) {
	bool step_precondition = true;

	for (size_t i = 0; i < this->input_step_ids.size; i++) {
		stateful_state_t step_state = this->context->getStepState(
				*(this->input_step_ids.ptr + i));
		step_precondition = step_precondition & step_state.active;
	}

	if (step_precondition && this->condition(state)) {
		for (size_t i = 0; i < this->input_step_ids.size; i++) {
			this->context->toggleStepState(*(this->input_step_ids.ptr + i),
					false);
		}
		for (size_t i = 0; i < this->output_step_ids.size; i++) {
			this->context->toggleStepState(*(this->output_step_ids.ptr + i),
					true);
		}
	}
}

predicate_fnc Transition::getCondition() const {
	return condition;
}

void Transition::setCondition(predicate_fnc condition) {
	this->condition = condition;
}

} /* namespace sfc */

