/*
Copyright 2020 Jerson Leonardo Huerfano Romero

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
#define PTR_ACTIVATED(state) ((state)->activated)
#define PTR_DEACTIVATING(state) (!((state)->active) && ((state)->transiting))
#define PTR_DEACTIVATED(state) ((state)->active)

#define ARRAY_GET(a,i) ((a).ptr + (i)) 

#define ACTION_STATE_ACTIVATING 'A'
#define ACTION_STATE_ACTIVE 'S'
#define ACTION_STATE_DEACTIVATING 'D'
#define ACTION_STATE_INACTIVE 'I'
#define ACTION_STATE_ALL 'O'

#define STATE_ACTIVATING 'A'
#define STATE_ACTIVE 'S'
#define STATE_DEACTIVATING 'D'
#define STATE_INACTIVE 'I'
#define STATE_ALL 'O'

namespace sfc {

template<typename T>
struct array {
	T *ptr;
	size_t size;
};

template<typename T>
inline array<T> arrayof(T * ptr, const size_t& size) {
	return {ptr, size};
}

typedef unsigned char byte_t;
typedef unsigned long ulong_t;
typedef void * pointer_t;
typedef struct {
	ulong_t current_time;
	bool enabled :1;
	bool interrupted :1;
} timer_state_t;
typedef struct {
	ulong_t active_time;
	bool activated :1;
	bool active :1;
	bool transiting :1;
} stateful_state_t;
typedef void (*state_handler_fnc)(const stateful_state_t & state);

typedef struct {
	char state;
	state_handler_fnc handler_fnc;
} state_handler_t;

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
typedef struct {
	char action_state;
	action_fnc function;
} action_state_handler_t;

typedef bool (*activation_predicate_fnc)(const predicate_state_t &state);
typedef bool (*predicate_fnc)(void);


}
;

#endif /* SFCTYPES_H_ */
