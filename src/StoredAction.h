/*
 * StoredAction.h
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#ifndef STOREDACTION_H_
#define STOREDACTION_H_

#include "Action.h"

namespace sfc {

class StoredAction: public Action {
private:
	bool should_activate = false;
public:
	StoredAction();
	StoredAction(const size_t &step_id);
	StoredAction(const size_t &step_id, activation_predicate_fnc condition);
	StoredAction(const size_t &step_id, array<action_state_handler_t> handlers);
	StoredAction(const size_t &step_id, activation_predicate_fnc condition, array<action_state_handler_t> handlers);
	virtual ~StoredAction();

protected:
	virtual bool evaluateActivation(const sfc::predicate_state_t &state) override;
};

} /* namespace sfc */

#endif /* STOREDACTION_H_ */
