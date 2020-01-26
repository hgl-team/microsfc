/*
 * Action.h
 *
 *  Created on: 17/11/2019
 *      Author: leonardo
 */

#ifndef ACTION_H_
#define ACTION_H_

#include "sfctypes.h"
#include "StatefulObject.h"
#include "StepContext.h"

namespace sfc {

class Action: public StatefulObject {
private:
	StepContext *context;
	size_t step_id;
	action_fnc on_activation_changed;
	activation_predicate_fnc condition;

	activation_state_t activation_state;
	stateful_state_t condition_state;
	ulong_t condition_state_t0;

protected:
	virtual void stateChanged(const sfc::stateful_state_t &state);
	virtual bool evaluateActivation(const sfc::predicate_state_t &state) = 0;
public:
	Action();
	Action(stateful_state_t * state, StepContext *context, const size_t &step_id);
	virtual ~Action();
	activation_predicate_fnc getCondition() const;
	void setCondition(activation_predicate_fnc condition);
	action_fnc getOnActivationChanged() const;
	void setOnActivationChanged(action_fnc onActivationChanged);
};

} /* namespace sfc */

#endif /* ACTION_H_ */
