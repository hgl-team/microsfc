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
	virtual ~StoredAction();

protected:
	virtual bool evaluateActivation(const sfc::predicate_state_t &state);
};

} /* namespace sfc */

#endif /* STOREDACTION_H_ */
