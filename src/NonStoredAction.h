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
	NonStoredAction(const size_t &step_id, action_fnc on_state_changed);
	virtual ~NonStoredAction();

protected:
	virtual bool evaluateActivation(const sfc::predicate_state_t &state);
};

} /* namespace sfc */

#endif /* NONSTOREDACTION_H_ */
