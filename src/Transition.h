/*
 * Transition.h
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include "sfctypes.h"
#include "EventListener.h"

#include "StepContext.h"

namespace sfc {

class Transition: EventListener {
private:
	StepContext *context;
	array<int> input_step_ids;
	array<int> output_step_ids;
	predicate_fnc condition;
public:
	Transition();
	Transition(StepContext *context, const array<int> &input_step_ids,
			const array<int> &output_step_ids, predicate_fnc condition);
	virtual ~Transition();
	virtual void onActivationChanged(const sfc::stateful_state_t &state);
	predicate_fnc getCondition() const;
	void setCondition(predicate_fnc condition);
};

} /* namespace sfc */

#endif /* TRANSITION_H_ */
