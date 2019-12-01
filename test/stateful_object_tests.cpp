/*
 * stateful_object_test.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "gtest.h"

#include "../src/sfctypes.h"
#include "../src/time/ClockListener.h"
#include "../src/StatefulObject.h"

using namespace sfc;

class StatefulObjectTest: public testing::Test {
public:
	stateful_state_t state;
	StatefulObject stateful_obj;

	StatefulObjectTest() {
		state = { 0, false, false };
		stateful_obj = StatefulObject(&state);
	}
};

TEST_F(StatefulObjectTest, objectActivatingAndTransiting) {
	stateful_obj.activate();
	EXPECT_TRUE(state.active);
	EXPECT_TRUE(ACTIVATING(state));
}

TEST_F(StatefulObjectTest, activatedObjectNoTransitingAfterTick) {
	stateful_obj.activate();
	EXPECT_TRUE(state.active);
	EXPECT_TRUE(ACTIVATING(state));
	stateful_obj.onTick(1);
	EXPECT_TRUE(state.active);
	EXPECT_TRUE(!ACTIVATING(state));
}

TEST_F(StatefulObjectTest, deactivatedObjectTransiting) {
	state.active = true;
	state.active_time = 1;
	state.transiting = false;

	stateful_obj.shutdown();

	ASSERT_FALSE(state.active);
	ASSERT_TRUE(DEACTIVATING(state));
}

TEST_F(StatefulObjectTest, deactivatedObjectNotTransitingAfterTick) {
	state.active = true;
	state.active_time = 1;
	state.transiting = false;

	stateful_obj.shutdown();

	ASSERT_FALSE(state.active);
	ASSERT_TRUE(DEACTIVATING(state));

	stateful_obj.onTick(1);

	ASSERT_FALSE(state.active);
	ASSERT_FALSE(DEACTIVATING(state));
}
