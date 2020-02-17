/*
 * StatefulObject.cpp
 *
 *  Created on: 17/11/2019
 *      Author: leonardo
 */

#include "StatefulObject.h"

namespace sfc {

StatefulObject::~StatefulObject() {
}

StatefulObject::StatefulObject() {
	this->state = { 0, false, false };
	this->handlers = { NULL, 0 };
	this->listeners = { NULL, 0 };
}
StatefulObject::StatefulObject(array<state_handler_t> const& handlers) {
	this->state = { 0, false, false };
	this->handlers = handlers;
	this->listeners = { NULL, 0 };
}

void StatefulObject::reportState(const stateful_state_t &state) {
	this->stateReported(this->state);

	for(size_t i = 0; i < this->handlers.size; i++) {
		this->reportState(state, ARRAY_GET(this->handlers, i));
	}
	for(size_t i = 0; i < this->listeners.size; i++) {
		this->reportState(state, *ARRAY_GET(this->listeners, i));
	}
}

void StatefulObject::reportState(const stateful_state_t &state, state_handler_t *const& handler) {
	switch (handler->state)
	{
	case STATE_ACTIVATING:
		if(PTR_ACTIVATING(&state)) {
			handler->handler_fnc(state);
		}
		break;
	case STATE_ACTIVE:
		if(state.active) {
			handler->handler_fnc(state);
		}
		break;
	case STATE_DEACTIVATING:
		if(PTR_DEACTIVATING(&state)) {
			handler->handler_fnc(state);
		}
		break;
	case STATE_INACTIVE:
		if(!state.active) {
			handler->handler_fnc(state);
		}
		break;
	case STATE_ALL:
		handler->handler_fnc(state);
		break;
	default: break;
	}
}

void StatefulObject::reportState(const stateful_state_t &state, EventListener *const&listener) {
	listener->onActivationChanged(state);
}

void StatefulObject::onTick(const sfc::ulong_t &delta) {
	this->state.transiting = (this->state.activated) ^ (this->state.active);
	this->state.active = this->state.activated;
	this->state.active_time =
			(!(this->state.active) || ACTIVATING(this->state)) ?
					this->state.active_time : this->state.active_time + delta;
	this->reportState(this->state);
}

void StatefulObject::activate() {
	if (!(this->state.activated)) {
		this->state.activated = true;
		this->state.active_time = 0;
	}
}

void StatefulObject::shutdown() {
	this->state.activated = false;
}

void StatefulObject::stateReported(const stateful_state_t &state) {
}

const array<EventListener*>& StatefulObject::getListeners() const {
	return listeners;
}

void StatefulObject::setListeners(const array<EventListener*> &listeners) {
	this->listeners = listeners;
}

stateful_state_t* StatefulObject::getState() {
	return &(this->state);
}

} /* namespace sfc */
