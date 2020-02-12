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

StoredAction::StoredAction(const size_t &step_id) :
		Action(step_id) {
}

StoredAction::StoredAction(const size_t &step_id, action_fnc on_state_changed) :
		Action(step_id, on_state_changed) {
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
