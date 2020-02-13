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
	action_fnc on_activation_reported_fnc;
	activation_predicate_fnc condition;

	activation_state_t activation_state;
	stateful_state_t condition_state;
	ulong_t condition_state_t0;

protected:
	virtual void stateReported(const sfc::stateful_state_t &state);
	virtual bool evaluateActivation(const sfc::predicate_state_t &state);
public:
	Action();
	Action(const size_t &step_id);
	Action(const size_t &step_id, action_fnc on_activation_reported_fnc);
	virtual ~Action();
	virtual void activate();
	virtual void shutdown();

	activation_predicate_fnc getCondition() const;
	void setCondition(activation_predicate_fnc condition);
	void setStepContext(StepContext * context);

	action_fnc getOnActivationReported() const;
	void setOnActivationReported(action_fnc on_activation_reported_fnc);
};

typedef Action * ActionPtr;

} /* namespace sfc */

#endif /* ACTION_H_ */
