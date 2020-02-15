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
	size_t step_id;
	activation_predicate_fnc condition;
	array<action_state_handler_t> handlers;

	StatefulObject condition_state;
	activation_state_t activation_state;
protected:
	virtual bool evaluateActivation(const sfc::predicate_state_t &state);
public:
	Action();
	Action(const size_t &step_id);
	Action(const size_t &step_id, activation_predicate_fnc condition);
	Action(const size_t &step_id, array<action_state_handler_t> handlers);
	Action(const size_t &step_id, activation_predicate_fnc condition, array<action_state_handler_t> handlers);
	virtual ~Action();

	virtual void evaluate(StepContext * const& context);
	virtual void onTick(const sfc::ulong_t & delta, StepContext * const& context);

	size_t getStepId();
};

typedef Action * ActionPtr;

} /* namespace sfc */

#endif /* ACTION_H_ */
