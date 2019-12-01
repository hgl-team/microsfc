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
	stateful_state_t *state;
	array<EventListener> listeners;

	void reportState(const stateful_state_t &state);
	void reportState(const stateful_state_t &state,
			EventListener *const&listener);
protected:
	virtual void stateChanged(const stateful_state_t &state);
public:
	StatefulObject();
	StatefulObject(stateful_state_t *state);
	virtual ~StatefulObject();

	virtual void onTick(const sfc::ulong_t &delta);
	virtual void activate();
	virtual void shutdown();

	const array<EventListener>& getListeners() const;
	void setListeners(const array<EventListener> &listeners);
	stateful_state_t* getState() const;
};

} /* namespace sfc */

#endif /* STATEFULOBJECT_H_ */
