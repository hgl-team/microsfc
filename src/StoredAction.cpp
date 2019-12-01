/*
 * StoredAction.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "StoredAction.h"

namespace sfc {

StoredAction::StoredAction() :
		Action() {
}

StoredAction::StoredAction(StepContext *context, stateful_state_t *state,
		const size_t &step_id) :
		Action(context, state, step_id) {
}

StoredAction::~StoredAction() {
}

bool StoredAction::evaluateActivation(const sfc::predicate_state_t &state) {
	if (ACTIVATING(state.step_state)) {
		this->should_activate = true;
	} else if (state.action_state.active) {
		this->should_activate = false;
	}
	return state.action_state.active || this->should_activate;
}

} /* namespace sfc */
