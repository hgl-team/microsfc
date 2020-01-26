/*
 * Application.h
 *
 *  Created on: 24/11/2019
 *      Author: leonardo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "sfctypes.h"
#include "StatefulObject.h"
#include "Step.h"
#include "Action.h"
#include "Transition.h"
#include "StepContext.h"

namespace sfc {

typedef struct {
	array<Step> steps;
	array<Action> actions;
	array<Transition> transitions;
} component_context_t;

class Application : public StatefulObject, public StepContext {
private:
	state_context_t state_context;
	component_context_t container_context;
protected:
	virtual void stateChanged(const stateful_state_t &state);
public:
	Application();
	Application(stateful_state_t * application_state, const state_context_t & state_context, const component_context_t & container_context);
	virtual ~Application();
	const virtual sfc::stateful_state_t& getStepState(const int &id);
	virtual size_t getStepCount();
	virtual bool isEntryPoint(const int &id);
	virtual void toggleStepState(const int &id, const bool &active);
};

} /* namespace sfc */

#endif /* APPLICATION_H_ */
