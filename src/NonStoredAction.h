/*
 * NonStoredAction.h
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#ifndef NONSTOREDACTION_H_
#define NONSTOREDACTION_H_

#include "Action.h"

namespace sfc {

class NonStoredAction: public Action {
public:
	NonStoredAction(const size_t &step_id);
	NonStoredAction(const size_t &step_id, activation_predicate_fnc condition);
	NonStoredAction(const size_t &step_id, array<state_handler_t> handlers);
	NonStoredAction(const size_t &step_id, activation_predicate_fnc condition, array<state_handler_t> handlers);
	virtual ~NonStoredAction();

protected:
	bool evaluateActivation(const sfc::predicate_state_t &state) override;
};

} /* namespace sfc */

#endif /* NONSTOREDACTION_H_ */
