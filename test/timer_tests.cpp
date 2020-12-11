/*
 * timer_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */
#include <gtest/gtest.h>

#include "sfctypes.h"
#include "Timer.h"
#include "ClockListener.h"

using namespace sfc;

bool callback_ack 			= false;
sfc::time_t callback_time 	= 0;

void timer_on_interrupt(const sfc::time_t &time) {
	callback_ack = true;
	callback_time = time;
}

class TimerTest: public testing::Test {
public:
	Timer timer = Timer(0, false);

	TimerTest() {
		timer = Timer(10, false);
		callback_ack = false;
		callback_time = 0;
	}
};

TEST_F(TimerTest, timerDoesNotTicks) {
	timer.onTick(5);
	ASSERT_EQ(0, timer.getState()->current_time);
}

TEST_F(TimerTest, timerTicks) {
	timer.enable();
	timer.onTick(5);
	ASSERT_EQ(5, timer.getState()->current_time);
}

TEST_F(TimerTest, timerInterrupts) {
	timer.getState()->enabled = true;
	timer.getState()->current_time = 9;
	timer.onTick(5);
	ASSERT_EQ(true, timer.getState()->interrupted);
	ASSERT_EQ(false, timer.getState()->enabled);
}

TEST_F(TimerTest, timerInterruptsAndCallback) {
	timer.getState()->enabled = true;
	timer.getState()->current_time = 9;
	timer.setInterruptCallback(timer_on_interrupt);
	timer.onTick(5);

	ASSERT_EQ(true, timer.getState()->interrupted);
	ASSERT_EQ(false, timer.getState()->enabled);
	ASSERT_TRUE(callback_ack);
	ASSERT_EQ(14, callback_time);
}

TEST_F(TimerTest, interruptedNonContinousTimerDoesNotTicks) {
	timer.getState()->enabled = true;
	timer.getState()->current_time = 9;
	timer.onTick(1);
	ASSERT_EQ(true, timer.getState()->interrupted);
	ASSERT_EQ(false, timer.getState()->enabled);
	timer.onTick(4);
	ASSERT_EQ(0, timer.getState()->current_time);
}
TEST_F(TimerTest, interruptedNonContinousTimerTicks) {
	timer = Timer(10, true);
	timer.getState()->enabled = true;
	timer.getState()->current_time = 9;
	timer.onTick(1);
	ASSERT_EQ(true, timer.getState()->interrupted);
	ASSERT_EQ(true, timer.getState()->enabled);
	ASSERT_EQ(0, timer.getState()->current_time);
	timer.onTick(4);
	ASSERT_EQ(4, timer.getState()->current_time);
	ASSERT_EQ(true, timer.getState()->enabled);
}
