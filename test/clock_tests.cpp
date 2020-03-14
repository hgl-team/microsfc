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
#include <gtest.h>

#include "../src/sfctypes.h"
#include "../src/time/Clock.h"
#include "../src/time/ClockListener.h"

using namespace testing;

sfc::ulong_t currentDelta = 0;

class TestListener: public sfc::ClockListener {
public:
	TestListener() { }
	~TestListener() { }

	virtual void onTick(const sfc::ulong_t &delta) {
		currentDelta = delta;
	}
};

class ClockTest: public testing::Test {
public:
	sfc::Clock clock;
	TestListener listener;
	sfc::ClockListener * listeners[1];

	ClockTest() {
		listeners[0] = &listener ;
		clock = sfc::Clock({ listeners, 1 });
		currentDelta = 0;
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
