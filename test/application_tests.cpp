/*
 * application_tests.cpp
 *
 *  Created on: 24/11/2019
 *      Author: leonardo
 */

#include "gtest.h"
#include "gmock.h"

#include "../src/sfctypes.h"
#include "../src/Application.h"
#include "../src/Step.h"
#include "test_stubs.h"

using namespace sfc;

stateful_state_t step_states[] = {
		{ 0, false, false },
		{ 0, false, false },
		{ 0, false, false },
};
Step steps[3] = {
		Step(&(step_states[0]), true),
		Step(&(step_states[1]), true),
		Step(&(step_states[2]), true)
};

class ApplicationTest : public testing::Test {
public:
	stateful_state_t application_state = { 0, false, false };
	state_context_t state_context = {
			{step_states, 3}, {NULL, 0}, {NULL, 0}
	};

	component_context_t component_context = {
			array<Step> { steps, 3 },
			array<Action> { NULL, 0 },
			array<Transition> { NULL, 0 },
	};
	Application application;

	ApplicationTest() {
		application = Application(&application_state, state_context, component_context);
	}
};

TEST_F(ApplicationTest, activatesStepsWhenActivating) {
	application.activate();
	application.onTick(1);

	for(size_t i = 0; i < 3; i++) {
		EXPECT_TRUE(ACTIVATING(step_states[i]));
	}
}

TEST_F(ApplicationTest, deactivatesStepsWhenDeactivating) {
	for(size_t i = 0; i < 3; i++) {
		(step_states + i)->active = 1;
		(step_states + i)->transiting = 0;
	}

	application_state.active = true;
	application_state.transiting = false;

	application.shutdown();
	application.onTick(1);

	for(size_t i = 0; i < 3; i++) {
		EXPECT_TRUE(DEACTIVATING(step_states[i]));
	}
}

TEST_F(ApplicationTest, shutdownStepWhenToggleStepToFalse) {
	step_states->active = 1;
	step_states->transiting = 0;

	application.toggleStepState(0, false);

	ASSERT_TRUE(DEACTIVATING(step_states[0]));
}

