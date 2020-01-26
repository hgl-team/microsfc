/*
 * action_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "gtest.h"
#include "gmock.h"

#include "../src/sfctypes.h"
#include "../src/time/Timer.h"
#include "../src/time/ClockListener.h"
#include "gtest.h"

#include "../src/sfctypes.h"
#include "../src/time/Timer.h"
#include "../src/time/ClockListener.h"
#include "../src/StoredAction.h"
#include "../test/test_stubs.h"

using namespace sfc;
using testing::_;

class MockArtifactContainer : public StepContext {
public:
	MOCK_METHOD0(getStepCount, size_t ());
	MOCK_METHOD1(isEntryPoint, bool (const int & id));;
	MOCK_METHOD1(getStepState, const stateful_state_t & (const int & id));
	MOCK_METHOD2(toggleStepState, void (const int & id, const bool & active));
};

class StoredActionTest: public testing::Test, public EventListener {
public:
	stateful_state_t state;
	stateful_state_t reported_state;
	StoredAction action;
	MockArtifactContainer container;

	virtual void onActivationChanged(const sfc::stateful_state_t &state) {
		this->reported_state = state;
	}

	StoredActionTest() {
		state = { 0, false, false };
		action = StoredAction(&state, &container, 0);
		action.setListeners( { this, 1 });
	}
};

TEST_F(StoredActionTest, actionDoesNotActivateIfStepDoesnt) {
	EXPECT_CALL(container, getStepState(_)).Times(1).WillOnce(
			::testing::ReturnRefOfCopy(deactivatedState));

	action.onTick(1);

	ASSERT_FALSE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionActivatesIfStepDoes) {
	EXPECT_CALL(container, getStepState(_)).Times(1).WillOnce(
			::testing::ReturnRefOfCopy(activatingState));

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionActivatesAndNotTransitingIfStepActive) {
	EXPECT_CALL(container, getStepState(_)).Times(2).WillOnce(
			testing::ReturnRefOfCopy(activatingState)).WillOnce(
			testing::ReturnRefOfCopy(activeState));

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionKeepsActiveAfterStepDeactivated) {
	EXPECT_CALL(container, getStepState(_)).Times(10).WillOnce(
			testing::ReturnRefOfCopy(activatingState)).WillOnce(
			testing::ReturnRefOfCopy(activeState)).WillOnce(
			testing::ReturnRefOfCopy(deactivatingState)).WillRepeatedly(
			testing::ReturnRefOfCopy(deactivatedState));

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	for (int i = 0; i < 9; i++) {
		action.onTick(1);

		ASSERT_TRUE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
	}
}

TEST_F(StoredActionTest, actionKeepsDeactiveEvenIfStepIsActive) {
	EXPECT_CALL(container, getStepState(_)).Times(10).WillOnce(
			testing::ReturnRefOfCopy(activatingState)).WillRepeatedly(
			testing::ReturnRefOfCopy(activeState));

	action.onTick(1);

	ASSERT_TRUE(ACTIVATING(reported_state));

	for (int i = 0; i < 9; i++) {
		if (i == 5) {
			action.shutdown();
		}

		action.onTick(1);

		if (i < 5) {
			EXPECT_TRUE(reported_state.active);
			EXPECT_FALSE(reported_state.transiting);
		} else if (i == 5) {
			EXPECT_FALSE(reported_state.active);
			EXPECT_TRUE(DEACTIVATING(reported_state));
		} else {
			EXPECT_FALSE(reported_state.active);
			EXPECT_FALSE(reported_state.transiting);
		}
	}
}

TEST_F(StoredActionTest, actionActivatesAfterConditionSet) {
	action.setCondition(active_after_5_ticks);

	EXPECT_CALL(container, getStepState(_)).Times(testing::AtLeast(1)).WillOnce(
			testing::ReturnRefOfCopy(activatingState)).WillRepeatedly(
			testing::ReturnRefOfCopy(activeState));

	for (int i = 0; i < 4; i++) {
		action.onTick(1);

		ASSERT_FALSE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
	}

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionActivatesAfterConditionSetEvenIfStepDeactivated) {
	action.setCondition(active_after_5_ticks);

	EXPECT_CALL(container, getStepState(_)).Times(testing::AtLeast(1)).WillOnce(
			testing::ReturnRefOfCopy(activatingState)).WillOnce(
			testing::ReturnRefOfCopy(activeState)).WillOnce(
			testing::ReturnRefOfCopy(deactivatingState)).WillRepeatedly(
			testing::ReturnRefOfCopy(deactivatedState));

	for (int i = 0; i < 4; i++) {
		action.onTick(1);

		ASSERT_FALSE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
	}

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}
