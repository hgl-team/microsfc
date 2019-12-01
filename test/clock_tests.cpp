/*
 * time_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "gtest.h"

#include "../src/sfctypes.h"
#include "../src/time/Clock.h"
#include "../src/time/ClockListener.h"

using namespace testing;

class ClockTest: public testing::Test, public sfc::ClockListener {
public:
	sfc::Clock clock;
	sfc::ulong_t currentDelta;

	ClockTest() {
		clock = sfc::Clock( { this, 1 });
		currentDelta = 0;
	}

	virtual void onTick(const sfc::ulong_t &delta) {
		currentDelta = delta;
	}

};

struct clock_tick_state {
	sfc::ulong_t initial_time;
	sfc::ulong_t elapsed_time;
	sfc::ulong_t expected_delta;
};

class ClockTickTest: public ClockTest, public testing::WithParamInterface<
		clock_tick_state> {
public:
	ClockTickTest() :
			ClockTest() {
		clock.offset(GetParam().initial_time);
		currentDelta = 0;
	}
};

TEST_P(ClockTickTest, TestClockOverflow) {
	clock_tick_state test_state = GetParam();
	clock.tick(test_state.elapsed_time);
	EXPECT_EQ(test_state.expected_delta, currentDelta);
	EXPECT_EQ(test_state.elapsed_time, clock.getCurrentTime());
}

INSTANTIATE_TEST_CASE_P(ClockTests, ClockTickTest, testing::Values(
				clock_tick_state {0, 10, 10},	// Control case 1
				clock_tick_state {50, 55, 5},	// Control case 2
				clock_tick_state {ULONG_MAX, 10, 11},	// Overflow test case 1
				clock_tick_state {ULONG_MAX - 50, 55, 106}// Overflow test case 2
		))
;
