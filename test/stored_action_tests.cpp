/*
 * action_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include <gtest.h>
#include <gmock/gmock.h>

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
	stateful_state_t reported_state;
	StoredAction action;
	MockArtifactContainer container;
	EventListener* listener[1];

	virtual void onActivationChanged(const sfc::stateful_state_t &state) {
		this->reported_state = state;
	}

	StoredActionTest() {
		listener[0] = this;
		action = StoredAction(0);
		action.setListeners( { listener, 1 });
	}

	void givenAStoredActionWithA5TickCondition() {
		listener[0] = this;
		action = StoredAction(0, active_after_5_ticks);
		action.setListeners( { listener, 1 });
	}
};

TEST_F(StoredActionTest, actionDoesNotActivateIfStepDoesnt) {
	EXPECT_CALL(container, getStepState(_)).Times(1)
		.WillRepeatedly(::testing::ReturnRefOfCopy(inactiveState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_FALSE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionActivatesIfStepDoes) {
	EXPECT_CALL(container, getStepState(_)).Times(1)
		.WillOnce(::testing::ReturnRefOfCopy(activatedState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionActivatesAndNotTransitingIfStepActive) {
	EXPECT_CALL(container, getStepState(_)).Times(2)
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionKeepsActiveAfterStepDeactivated) {
	EXPECT_CALL(container, getStepState(_)).Times(10)
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillOnce(testing::ReturnRefOfCopy(activeState))
		.WillOnce(testing::ReturnRefOfCopy(deactivatedState))
		.WillOnce(testing::ReturnRefOfCopy(deactivatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(inactiveState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	for (int i = 0; i < 9; i++) {
		action.evaluate(&container);
		action.onTick(1); //, &container);

		ASSERT_TRUE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
	}
}

TEST_F(StoredActionTest, actionKeepsDeactiveEvenIfStepIsActive) {
	EXPECT_CALL(container, getStepState(_)).Times(10)
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(activeState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(ACTIVATING(reported_state));

	for (int i = 0; i < 9; i++) {
		if (i == 5) {
			action.shutdown();
		}

		action.evaluate(&container);
		action.onTick(1); //, &container);

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
	this->givenAStoredActionWithA5TickCondition();

	EXPECT_CALL(container, getStepState(_)).Times(5)
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(activeState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	for (int i = 0; i < 4; i++) {
		ASSERT_FALSE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);

		action.evaluate(&container);
		action.onTick(1); //, &container);
	}

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

TEST_F(StoredActionTest, actionActivatesAfterConditionSetEvenIfStepDeactivated) {
	this->givenAStoredActionWithA5TickCondition();

	EXPECT_CALL(container, getStepState(_)).Times(5)
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillOnce(testing::ReturnRefOfCopy(deactivatedState))
		.WillOnce(testing::ReturnRefOfCopy(deactivatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(inactiveState));
	
	action.evaluate(&container);
	action.onTick(1); //, &container);
	
	for (int i = 0; i < 4; i++) {
		ASSERT_FALSE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
		
		action.evaluate(&container);
		action.onTick(1); //, &container);
	}

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}
