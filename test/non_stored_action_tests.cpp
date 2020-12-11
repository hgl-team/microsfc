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
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "sfctypes.h"
#include "NonStoredAction.h"
#include "test_stubs.h"
#include "StepContext.h"

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
	stateful_state_t reported_state;
	NonStoredAction action = NonStoredAction(0);
	MockNonStoredArtifactContainer container;
	EventListener* listener[1];

	virtual void onActivationChanged(const sfc::stateful_state_t &state) {
		this->reported_state = state;
	}

	NonStoredActionTest() {
		listener[0] = this;
		action = NonStoredAction(0);
		action.setListeners({ listener, 1 });
	}

	void givenAnActionWith5TickCondition() {
		listener[0] = this;
		action = NonStoredAction(0, active_after_5_ticks);
		action.setListeners({ listener, 1 });
	}
};

TEST_F(NonStoredActionTest, actionDoesNotActivateIfStepDoesnt) {
	EXPECT_CALL(container, getStepState(_)).Times(1)
		.WillOnce(::testing::ReturnRefOfCopy(inactiveState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_FALSE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionActivatesIfStepDoes) {
	EXPECT_CALL(container, getStepState(_)).Times(1)
		.WillOnce(::testing::ReturnRefOfCopy(activatedState));

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionActivatesAndNotTransitingIfStepActive) {
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

TEST_F(NonStoredActionTest, actionDeactivatesWhenStepDeactives) {
	EXPECT_CALL(container, getStepState(_)).Times(testing::AtLeast(1))
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillOnce(testing::ReturnRefOfCopy(deactivatedState))
		.WillRepeatedly(testing::ReturnRefOfCopy(deactivatingState));

	action.evaluate(&container);
	action.onTick(1); //, &container);
	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.evaluate(&container);
	action.onTick(1); //, &container);
	ASSERT_TRUE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);

	action.evaluate(&container);
	action.onTick(1); //, &container);
	ASSERT_FALSE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);

	action.evaluate(&container);
	action.onTick(1); //, &container);
	ASSERT_FALSE(reported_state.active);
	ASSERT_FALSE(reported_state.transiting);
}

TEST_F(NonStoredActionTest, actionActivatesAfterConditionSet) {
	this->givenAnActionWith5TickCondition();

	EXPECT_CALL(container, getStepState(_)).Times(testing::AtLeast(1))
		.WillOnce(testing::ReturnRefOfCopy(activatedState))
		.WillOnce(testing::ReturnRefOfCopy(activatingState))
		.WillRepeatedly(testing::ReturnRefOfCopy(activeState));

	for (int i = 0; i < 4; i++) {
		action.evaluate(&container);
		action.onTick(1); //, &container);

		ASSERT_FALSE(reported_state.active);
		ASSERT_FALSE(reported_state.transiting);
	}

	action.evaluate(&container);
	action.onTick(1); //, &container);

	ASSERT_TRUE(reported_state.active);
	ASSERT_TRUE(reported_state.transiting);
}

