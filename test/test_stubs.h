/*
 * test_stubs.h
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#ifndef TEST_STUBS_H_
#define TEST_STUBS_H_

#include "../src/sfctypes.h"
#include "../src/time/Timer.h"

using namespace sfc;

extern Timer _5_tick_timer;
extern const stateful_state_t deactivatedState;
extern const stateful_state_t activatingState;
extern const stateful_state_t activeState;
extern const stateful_state_t deactivatingState;
extern int input_ids[];
extern int output_ids[];

bool active_after_5_ticks(const predicate_state_t &state);
bool predicate_always_true(const stateful_state_t &state);
bool predicate_always_false(const stateful_state_t &state);

#endif /* TEST_STUBS_H_ */
