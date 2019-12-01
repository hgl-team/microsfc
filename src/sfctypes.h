/*
 * sfctypes.h
 *
 *  Created on: 16/11/2019
 *      Author: leonardo
 */

#ifndef SFCTYPES_H_
#define SFCTYPES_H_

#include <cstddef>
#include <climits>

#define PTR(a) ((sfc::pointer_t)(&a))
#define BIT(b,n) ((b >> n) & 1)
#define BIT_SET(b,n) ((1 << n) | b)
#define BIT_RESET(b,n) ((~(1 << n)) & b)
#define ACTIVATING(state) (((state).active) && ((state).transiting))
#define DEACTIVATING(state) (!((state).active) && ((state).transiting))
#define PTR_ACTIVATING(state) (((state)->active) && ((state)->transiting))
#define PTR_DEACTIVATING(state) (!((state)->active) && ((state)->transiting))

namespace sfc {

template<typename T>
struct array {
	T *ptr;
	size_t size;
};

typedef unsigned char byte_t;
typedef unsigned long ulong_t;
typedef void *pointer_t;
typedef struct {
	ulong_t current_time;
	bool enabled :1;
	bool interrupted :1;
} timer_state_t;
typedef struct {
	ulong_t active_time;
	bool active :1;
	bool transiting :1;
} stateful_state_t;
typedef struct {
	stateful_state_t action_state;
	stateful_state_t step_state;
} predicate_state_t;
typedef struct {
	stateful_state_t action_state;
	stateful_state_t step_state;
	stateful_state_t condition_state;
	bool active;
} activation_state_t;

typedef void (*action_fnc)(const activation_state_t &state);
typedef bool (*activation_predicate_fnc)(const predicate_state_t &state);
typedef bool (*predicate_fnc)(const stateful_state_t &state);
typedef const stateful_state_t& (*step_state_getter_fnc)(const int &id);
typedef void (*step_state_toggle_fnc)(const int &id, const bool &active);

typedef struct {
	array<stateful_state_t> step_states;
	array<stateful_state_t> action_states;
	array<predicate_fnc> transition_predicates;
} state_context_t;
}
;

#endif /* SFCTYPES_H_ */
