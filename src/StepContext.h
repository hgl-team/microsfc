/*
 * StepContainer.h
 *
 *  Created on: 24/11/2019
 *      Author: leonardo
 */

#ifndef STEPCONTEXT_H_
#define STEPCONTEXT_H_

#include "sfctypes.h"

namespace sfc {

class StepContext {
public:
	virtual size_t getStepCount() = 0;
	virtual bool isEntryPoint(const int & id) = 0;
	virtual const stateful_state_t & getStepState(const int & id) = 0;
	virtual void toggleStepState(const int & id, const bool & active) = 0;
};

} /* namespace sfc */

#endif /* STEPCONTEXT_H_ */
