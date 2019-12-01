/*
 * timer_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */
#include "gtest.h"

#include "../src/sfctypes.h"
#include "../src/time/Timer.h"
#include "../src/time/ClockListener.h"

using namespace sfc;

class TimerTest: public testing::Test {
public:
	timer_state_t timer_state { 0, false, false };
	Timer timer = Timer(NULL, 0, false);

	TimerTest() {
		timer = Timer(&timer_state, 10, false);
	}
};

TEST_F(TimerTest, timerDoesNotTicks) {
	timer.onTick(5);
	ASSERT_EQ(0, timer_state.current_time);
}

TEST_F(TimerTest, timerTicks) {
	timer.enable();
	timer.onTick(5);
	ASSERT_EQ(5, timer_state.current_time);
}

TEST_F(TimerTest, timerInterrupts) {
	timer_state.enabled = true;
	timer_state.current_time = 9;
	timer.onTick(5);
	ASSERT_EQ(true, timer_state.interrupted);
	ASSERT_EQ(false, timer_state.enabled);
}

TEST_F(TimerTest, interruptedNonContinousTimerDoesNotTicks) {
	timer_state.enabled = true;
	timer_state.current_time = 9;
	timer.onTick(1);
	ASSERT_EQ(true, timer_state.interrupted);
	ASSERT_EQ(false, timer_state.enabled);
	timer.onTick(4);
	ASSERT_EQ(0, timer_state.current_time);
}
TEST_F(TimerTest, interruptedNonContinousTimerTicks) {
	timer = Timer(&timer_state, 10, true);
	timer_state.enabled = true;
	timer_state.current_time = 9;
	timer.onTick(1);
	ASSERT_EQ(true, timer_state.interrupted);
	ASSERT_EQ(true, timer_state.enabled);
	ASSERT_EQ(0, timer_state.current_time);
	timer.onTick(4);
	ASSERT_EQ(4, timer_state.current_time);
	ASSERT_EQ(true, timer_state.enabled);
}
