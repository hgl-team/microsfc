/*
 * transition_tests.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "gtest.h"
#include "gmock.h"

#include "../src/sfctypes.h"
#include "../src/Transition.h"
#include "../test/test_stubs.h"

using namespace sfc;
using testing::_;

class MockTransitionArtifactContainer : public StepContext {
public:
	MOCK_METHOD0(getStepCount, size_t ());
	MOCK_METHOD1(isEntryPoint, bool (const int & id));;
	MOCK_METHOD1(getStepState, const stateful_state_t & (const int & id));
	MOCK_METHOD2(toggleStepState, void (const int & id, const bool & active));
};

class TransitionTest: public testing::Test {
public:
	MockTransitionArtifactContainer container;
	Transition transition;

	TransitionTest() {
		transition = Transition(
			{ input_ids, 3 }, 
			{ output_ids, 3 }, NULL);
		transition.setStepContext(&container);
	}
};

TEST_F(TransitionTest, activationSkipWhenStepNoActiveAndConditionSet) {

	EXPECT_CALL(container, getStepState(_)).Times(3).WillOnce(
			::testing::ReturnRefOfCopy(deactivatedState)).WillRepeatedly(
			::testing::ReturnRefOfCopy(activeState));

	EXPECT_CALL(container, toggleStepState(_,_)).Times(0);

	transition.setCondition(predicate_always_true);
	transition.onActivationChanged(activeState);
}

TEST_F(TransitionTest, activationSkipWhenStepActiveAndConditionFalse) {

	EXPECT_CALL(container, getStepState(_)).Times(3).WillRepeatedly(
			::testing::ReturnRefOfCopy(activeState));

	EXPECT_CALL(container, toggleStepState(_,_)).Times(0);

	transition.setCondition(predicate_always_false);
	transition.onActivationChanged(activeState);
}

TEST_F(TransitionTest, activationSetWhenStepActiveAndConditionSet) {

	EXPECT_CALL(container, getStepState(_)).Times(3).WillRepeatedly(
			::testing::ReturnRefOfCopy(activeState));

	EXPECT_CALL(container, toggleStepState(_,_)).Times(6);

	transition.setCondition(predicate_always_true);
	transition.onActivationChanged(activeState);
}
