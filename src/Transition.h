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

class Transition {
private:
	array<int> input_step_ids;
	array<int> output_step_ids;
	predicate_fnc condition;
public:
	Transition();
	Transition(const array<int> &input_step_ids,
			const array<int> &output_step_ids, 
			predicate_fnc condition);
	virtual ~Transition();
	virtual void onActivationChanged(StepContext * const& context);
};

} /* namespace sfc */

#endif /* TRANSITION_H_ */
