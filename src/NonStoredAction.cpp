/*
 * NonStoredAction.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "NonStoredAction.h"

namespace sfc {

NonStoredAction::NonStoredAction(const size_t &step_id) :
		Action(step_id) {
}

NonStoredAction::NonStoredAction(const size_t &step_id, action_fnc on_state_changed) :
		Action(step_id, on_state_changed) {
}

NonStoredAction::~NonStoredAction() {
}

bool NonStoredAction::evaluateActivation(const sfc::predicate_state_t &state) {
	return state.step_state.active;
}

} /* namespace sfc */
