/*
 * action_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include <gtest.h>
#include <gmock/gmock.h>

#include "../src/sfctypes.h"
#include "../src/NonStoredAction.h"
#include "../test/test_stubs.h"
#include "../src/StepContext.h"

using namespace sfc;
using testing::_;

class MockNonStoredArtifactContainer : public StepContext {
public:
	MOCK_METHOD0(getStepCount, size_t ());
	MOCK_METHOD1(isEntryPoint, bool (const int & id));;
	MOCK_METHOD1(getStepState, const stateful_state_t & (const int & id));
	MOCK_METHOD2(toggleStepState, void (const int & id, const bool & active));
};

class NonStoredActionTest: public testing::Test, public EventListener {
public:
	stateful_state_t state;
	stateful_state_t reported_state;
	NonStoredAction action = NonStoredAction(0);
	MockNonStoredArtifactContainer container;

	virtual void onActivationChanged(const sfc::stateful_state_t &state) {
		this->reported_state = state;
	}

	NonStoredActionTest() {
		state = { 0, false, false };
		action = NonStoredAction(0);
		action.setStepContext(&container);
		action.setListeners( { this, 1 });
	}
};

TEST_F(NonStoredActionTest, actionDoesNotActivateIfStepDoesnt) {
	EXPECT_CALL(container, getStepState(_)).Times(1).WillOnce(
			::testing::ReturnRefOfCopy(deactivatedState));

	action.onTick(1);

	ASSERT_FALSE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionActivatesIfStepDoes) {
	EXPECT_CALL(container, getStepState(_)).Times(1)
		.WillOnce(::testing::ReturnRefOfCopy(activatingState));

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionActivatesAndNotTransitingIfStepActive) {
	EXPECT_CALL(container, getStepState(_)).Times(2)
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillOnce(testing::ReturnRefOfCopy(activeState));

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionDeactivatesWhenStepDeactives) {
	EXPECT_CALL(container, getStepState(_)).Times(testing::AtLeast(1))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillOnce(testing::ReturnRefOfCopy(activeState))
		.WillOnce(testing::ReturnRefOfCopy(deactivatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(deactivatedState));

	action.onTick(1);
	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.onTick(1);
	ASSERT_TRUE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);

	action.onTick(1);
	ASSERT_FALSE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.onTick(1);
	ASSERT_FALSE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionActivatesAfterConditionSet) {
	action.setCondition(active_after_5_ticks);

	EXPECT_CALL(container, getStepState(_)).Times(testing::AtLeast(1))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(activeState));

	for (int i = 0; i < 4; i++) {
		action.onTick(1);

		ASSERT_FALSE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
	}

	action.onTick(1);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

