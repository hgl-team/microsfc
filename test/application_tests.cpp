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

Step steps[3] = {
	Step(true),
	Step(true),
	Step(true)
};

stateful_state_t * step_states[] = {
	steps[0].getState(),
	steps[1].getState(),
	steps[2].getState()
};

class ApplicationTest : public testing::Test {
public:
	component_context_t component_context = {
			array<Step> { steps, 3 },
			array<Action> { NULL, 0 },
			array<Transition> { NULL, 0 },
	};
	Application application;

	ApplicationTest() {
		application = Application(component_context);
	}
};

TEST_F(ApplicationTest, activatesStepsWhenActivating) {
	application.activate();
	application.onTick(1);

	for(size_t i = 0; i < 3; i++) {
		EXPECT_TRUE(PTR_ACTIVATING(step_states[i]));
	}
}

TEST_F(ApplicationTest, deactivatesStepsWhenDeactivating) {
	for(size_t i = 0; i < 3; i++) {
		(step_states[i])->active = 1;
		(step_states[i])->transiting = 0;
	}

	application.getState()->active = true;
	application.getState()->transiting = false;

	application.shutdown();
	application.onTick(1);

	for(size_t i = 0; i < 3; i++) {
		EXPECT_TRUE(PTR_DEACTIVATING(steps[i].getState()));
	}
}

TEST_F(ApplicationTest, shutdownStepWhenToggleStepToFalse) {
	step_states[0]->active = 1;
	step_states[0]->transiting = 0;

	application.toggleStepState(0, false);

	ASSERT_TRUE(PTR_DEACTIVATING(step_states[0]));
}
