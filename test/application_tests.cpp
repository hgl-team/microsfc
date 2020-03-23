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
#include "Application.h"
#include "Step.h"
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
			array<Action*> { NULL, 0 },
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
		EXPECT_TRUE(step_states[i]->activated);
	}

	application.onTick(1);
	for(size_t i = 0; i < 3; i++) {
		EXPECT_TRUE(PTR_ACTIVATING(step_states[i]));
	}
}

TEST_F(ApplicationTest, deactivatesStepsWhenDeactivating) {
	for(size_t i = 0; i < 3; i++) {
		(step_states[i])->activated = 1;
		(step_states[i])->active = 1;
		(step_states[i])->transiting = 0;
	}

	application.getState()->activated = true;
	application.getState()->active = true;
	application.getState()->transiting = false;

	application.shutdown();

	application.onTick(1);
	for(size_t i = 0; i < 3; i++) {
		EXPECT_FALSE(steps[i].getState()->activated);
	}

	application.onTick(1);
	for(size_t i = 0; i < 3; i++) {
		EXPECT_TRUE(PTR_DEACTIVATING(steps[i].getState()));
	}
}

TEST_F(ApplicationTest, shutdownStepWhenToggleStepToFalse) {
	step_states[0]->activated = 1;
	step_states[0]->active = 1;
	step_states[0]->transiting = 0;

	application.toggleStepState(0, false);
	application.onTick(1);
	ASSERT_FALSE(step_states[0]->activated);
	application.onTick(1);
	ASSERT_TRUE(PTR_DEACTIVATING(step_states[0]));
}
