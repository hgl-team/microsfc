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
	this->state = state;
	this->listeners = { NULL, 0 };
}

StatefulObject::StatefulObject(stateful_state_t * state) {
	this->state = state;
	this->listeners = { NULL, 0 };
}

void StatefulObject::reportState(const stateful_state_t &state) {
	for (size_t i = 0; i < this->listeners.size; i++) {
		this->reportState(state, this->listeners.ptr + i);
	}
}

void StatefulObject::onTick(const sfc::ulong_t &delta) {

	this->stateChanged(*(this->state));
	this->reportState(*(this->state));

	this->state->active_time =
			(!(this->state->active) || PTR_ACTIVATING(this->state)) ?
					this->state->active_time : this->state->active_time + delta;
	this->state->transiting = false;
}

void StatefulObject::activate() {
	if (!(this->state->active)) {
		this->state->active = true;
		this->state->active_time = 0;
		this->state->transiting = true;
	}
}

void StatefulObject::shutdown() {
	this->state->transiting = this->state->active;
	this->state->active = false;
}

void StatefulObject::reportState(const stateful_state_t &state,
		EventListener *const&listener) {
	listener->onActivationChanged(state);
}

void StatefulObject::stateChanged(const stateful_state_t &state) {
}

const array<EventListener>& StatefulObject::getListeners() const {
	return listeners;
}

void StatefulObject::setListeners(const array<EventListener> &listeners) {
	this->listeners = listeners;
}

const stateful_state_t& StatefulObject::getState() {
	return *(state);
}

} /* namespace sfc */
