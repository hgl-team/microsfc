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
	virtual ~NonStoredAction();

protected:
	virtual bool evaluateActivation(const sfc::predicate_state_t &state);
};

} /* namespace sfc */

#endif /* NONSTOREDACTION_H_ */
