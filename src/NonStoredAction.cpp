/*
 * NonStoredAction.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "NonStoredAction.h"

namespace sfc {

NonStoredAction::NonStoredAction(StepContext *context,
		stateful_state_t *state, const size_t &step_id) :
		Action(context, state, step_id) {
}

NonStoredAction::~NonStoredAction() {
}

bool NonStoredAction::evaluateActivation(const sfc::predicate_state_t &state) {
	return state.step_state.active;
}

} /* namespace sfc */
