/*
 * stateful_object_test.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include <gtest/gtest.h>

#include "sfctypes.h"
#include "ClockListener.h"
#include "StatefulObject.h"

using namespace sfc;

class StatefulObjectTest: public testing::Test {
public:
	StatefulObject stateful_obj;

	StatefulObjectTest() {
		stateful_obj = StatefulObject();
	}
};

TEST_F(StatefulObjectTest, objectActivatingAndTransiting) {
	stateful_obj.activate();
	EXPECT_TRUE(stateful_obj.getState()->activated);
	EXPECT_FALSE(stateful_obj.getState()->active);
	EXPECT_FALSE(stateful_obj.getState()->transiting);

	stateful_obj.onTick(1);
	EXPECT_TRUE(stateful_obj.getState()->active);
	EXPECT_TRUE(PTR_ACTIVATING(stateful_obj.getState()));
}

TEST_F(StatefulObjectTest, activatedObjectNoTransitingAfterTick) {
	stateful_obj.activate();
	stateful_obj.onTick(1);
	EXPECT_TRUE(stateful_obj.getState()->active);
	EXPECT_TRUE(PTR_ACTIVATING(stateful_obj.getState()));
	stateful_obj.onTick(1);
	EXPECT_TRUE(stateful_obj.getState()->active);
	EXPECT_TRUE(!PTR_ACTIVATING(stateful_obj.getState()));
}

TEST_F(StatefulObjectTest, deactivatedObjectTransiting) {
	stateful_obj.getState()->active = true;
	stateful_obj.getState()->active_time = 1;
	stateful_obj.getState()->transiting = false;

	stateful_obj.shutdown();
	EXPECT_FALSE(stateful_obj.getState()->activated);
	EXPECT_TRUE(stateful_obj.getState()->active);
	EXPECT_FALSE(stateful_obj.getState()->transiting);

	stateful_obj.onTick(1);
	ASSERT_FALSE(stateful_obj.getState()->active);
	ASSERT_TRUE(PTR_DEACTIVATING(stateful_obj.getState()));
}

TEST_F(StatefulObjectTest, deactivatedObjectNotTransitingAfterTick) {
	stateful_obj.getState()->active = true;
	stateful_obj.getState()->active_time = 1;
	stateful_obj.getState()->transiting = false;

	stateful_obj.shutdown();
	stateful_obj.onTick(1);

	ASSERT_FALSE(stateful_obj.getState()->active);
	ASSERT_TRUE(PTR_DEACTIVATING(stateful_obj.getState()));

	stateful_obj.onTick(1);
	ASSERT_FALSE(stateful_obj.getState()->active);
	ASSERT_FALSE(PTR_DEACTIVATING(stateful_obj.getState()));
}
