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

StoredAction::StoredAction(const size_t &step_id, activation_predicate_fnc condition) :
		Action(step_id, condition) {
}

StoredAction::StoredAction(const size_t &step_id, array<action_state_handler_t> handlers) :
		Action(step_id, handlers) {
}

StoredAction::StoredAction(const size_t &step_id, activation_predicate_fnc condition, array<action_state_handler_t> handlers) :
		Action(step_id, condition, handlers) {
}

StoredAction::~StoredAction() {
}

bool StoredAction::evaluateActivation(const sfc::predicate_state_t &state) {
	if(state.action_state.activated) {
		this->should_activate = false;
	} else if (state.step_state.activated && !state.step_state.active) {
		this->should_activate = true;
	}
	return state.action_state.activated || this->should_activate;
}

} /* namespace sfc */
