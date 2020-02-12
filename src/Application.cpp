/*
 * Application.cpp
 *
 *  Created on: 24/11/2019
 *      Author: leonardo
 */

#include "Application.h"

namespace sfc {

Application::Application(const component_context_t & container_context)
	: StatefulObject()
{
	this->container_context = container_context;
	for(size_t i = 0; i < this->container_context.actions.size; i++) {
		ARRAY_GET(this->container_context.actions, i)->setStepContext(this);
	}
	for(size_t i = 0; i < this->container_context.transitions.size; i++) {
		ARRAY_GET(this->container_context.transitions, i)->setStepContext(this);
	}
}

Application::Application() : StatefulObject() {
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
	} else if(state.active) {
		for(size_t i = 0; i < this->container_context.transitions.size; i++) {
			ARRAY_GET(this->container_context.transitions, i)->onActivationChanged(state);
		}
	}
}

void Application::onTick(const sfc::ulong_t &delta) {
	StatefulObject::onTick(delta);
	for(size_t i = 0; i < this->container_context.steps.size; i++) {
		ARRAY_GET(this->container_context.steps, i)->onTick(delta);
	}
	for(size_t i = 0; i < this->container_context.actions.size; i++) {
		ARRAY_GET(this->container_context.actions, i)->onTick(delta);
	}
}

const sfc::stateful_state_t& Application::getStepState(const int &id) {
	return *(ARRAY_GET(this->container_context.steps, id)->getState());
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
