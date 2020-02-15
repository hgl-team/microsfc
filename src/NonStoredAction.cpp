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
NonStoredAction::NonStoredAction(const size_t &step_id, activation_predicate_fnc condition) :
		Action(step_id, condition) {
}

NonStoredAction::NonStoredAction(const size_t &step_id, array<action_state_handler_t> handlers) :
		Action(step_id, handlers) {
}
NonStoredAction::NonStoredAction(const size_t &step_id, activation_predicate_fnc condition, array<action_state_handler_t> handlers) :
		Action(step_id, condition, handlers) {
}

NonStoredAction::~NonStoredAction() {
}

bool NonStoredAction::evaluateActivation(const sfc::predicate_state_t &state) {
	return state.step_state.activated;
}

} /* namespace sfc */
