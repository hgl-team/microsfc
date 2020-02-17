/*
 * Transition.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "Transition.h"

namespace sfc {

Transition::Transition(const array<int> &input_step_ids, 
		const array<int> &output_step_ids,
		predicate_fnc condition) {
	this->input_step_ids = input_step_ids;
	this->output_step_ids = output_step_ids;
	this->condition = condition;
}

Transition::~Transition() {
}

Transition::Transition() {
	this->input_step_ids = { NULL, 0 };
	this->output_step_ids = { NULL, 0 };
	this->condition = NULL;
}

void Transition::onActivationChanged(StepContext * const& context) {
	bool step_precondition = true;

	for (size_t i = 0; i < this->input_step_ids.size; i++) {
		stateful_state_t step_state = context->getStepState(
				*(this->input_step_ids.ptr + i));
		step_precondition = step_precondition & step_state.active;
	}

	if (step_precondition && this->condition()) {
		for (size_t i = 0; i < this->input_step_ids.size; i++) {
			context->toggleStepState(*(this->input_step_ids.ptr + i),
					false);
		}
		for (size_t i = 0; i < this->output_step_ids.size; i++) {
			context->toggleStepState(*(this->output_step_ids.ptr + i),
					true);
		}
	}
}

} /* namespace sfc */

