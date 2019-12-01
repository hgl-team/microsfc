/*
 * Application.cpp
 *
 *  Created on: 24/11/2019
 *      Author: leonardo
 */

#include "Application.h"

namespace sfc {

Application::Application(stateful_state_t * application_state, const state_context_t & state_context, const component_context_t & container_context)
	: StatefulObject(application_state)
{
	this->state_context = state_context;
	this->container_context = container_context;
}

Application::Application() : StatefulObject() {
	this->state_context = { {NULL, 0}, {NULL, 0}, { NULL, 0 } };
	this->container_context = { {NULL, 0}, {NULL, 0}, { NULL, 0 } };
}

Application::~Application() { }

void Application::stateChanged(const stateful_state_t &state) {
	if(ACTIVATING(state)) {
		for(size_t i = 0; i < this->getStepCount(); i++) {
			if(this->isEntryPoint(i)) {
				this->toggleStepState(i, true);
			}
		}
	} else if(DEACTIVATING(state)) {
		for(size_t i = 0; i < this->getStepCount(); i++) {
			this->toggleStepState(i, false);
		}
	}
}

const sfc::stateful_state_t& Application::getStepState(const int &id) {
	return *(this->state_context.step_states.ptr + id);
}

size_t Application::getStepCount() {
	return this->container_context.steps.size;
}

bool Application::isEntryPoint(const int &id) {
	Step * step = this->container_context.steps.ptr + id;
	return step->isEntryPoint();
}

void Application::toggleStepState(const int &id, const bool &active) {
	if(active) {
		(this->container_context.steps.ptr + id)->activate();
	} else {
		(this->container_context.steps.ptr + id)->shutdown();
	}
}

} /* namespace sfc */
