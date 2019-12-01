/*
 * Step.h
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#ifndef STEP_H_
#define STEP_H_

#include "StatefulObject.h"

namespace sfc {

class Step: public StatefulObject {
private:
	bool entry_point;
public:
	Step();
	Step(stateful_state_t *state, bool entry_point);
	virtual ~Step();
	virtual bool isEntryPoint() const;
	virtual void setEntryPoint(bool entryPoint);
};

} /* namespace sfc */

#endif /* STEP_H_ */
