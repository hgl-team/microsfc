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
	array<Action*> actions;
	array<Transition> transitions;
} component_context_t;

class Application : public StatefulObject, public StepContext {
private:
	component_context_t container_context;
	bool evaluate;
	ulong_t component_delta;
protected:
	virtual void stateReported(const stateful_state_t &state);

	virtual void evaluateStates(const sfc::ulong_t &delta);
	virtual void performComponentTick(const sfc::ulong_t &delta);

	virtual void evaluateTransitions();
	virtual void evaluateActions();
public:
	Application();
	Application(const component_context_t & container_context);
	virtual ~Application();
	const virtual sfc::stateful_state_t& getStepState(const int &id);
	virtual size_t getStepCount();
	virtual bool isEntryPoint(const int &id);
	virtual void toggleStepState(const int &id, const bool &active);
	virtual void onTick(const sfc::ulong_t &delta);
};

} /* namespace sfc */

#endif /* APPLICATION_H_ */
