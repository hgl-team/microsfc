/*
 * StatefulObject.h
 *
 *  Created on: 17/11/2019
 *      Author: leonardo
 */

#ifndef STATEFULOBJECT_H_
#define STATEFULOBJECT_H_

#include "time/ClockListener.h"
#include "EventListener.h"

namespace sfc {

class StatefulObject: public ClockListener {
private:
	stateful_state_t state;
	array<state_handler_t> handlers;
	array<EventListener*> listeners;

	void reportState(const stateful_state_t &state);
	void reportState(const stateful_state_t &state, state_handler_t *const& handler);
	void reportState(const stateful_state_t &state, EventListener *const& listener);
protected:
	virtual void stateReported(const stateful_state_t &state);
public:
	StatefulObject();
	StatefulObject(array<state_handler_t> const& handlers);
	virtual ~StatefulObject();

	virtual void onTick(const sfc::ulong_t &delta);
	virtual void activate();
	virtual void shutdown();

	const array<EventListener*>& getListeners() const;
	void setListeners(const array<EventListener*> &listeners);
	stateful_state_t* getState();
};

} /* namespace sfc */

#endif /* STATEFULOBJECT_H_ */
