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
#include "../src/Step.h"
#include "../src/StoredAction.h"
#include "../src/NonStoredAction.h"
#include "../src/Application.h"
#include "../src/time/Clock.h"
#include "../src/time/Timer.h"
#include <pthread.h>
#include <ctime>
#include <iostream>
#include <unistd.h>

using namespace sfc;

bool transition_0();
bool transition_1();
bool transition_2();
bool transition_3();
bool transition_4();
bool transition_5();

void action_0(const stateful_state_t &state);
void action_1(const stateful_state_t &state);
void action_2(const stateful_state_t &state);
void action_3(const stateful_state_t &state);
void action_4(const stateful_state_t &state);

void print_state();

Step steps[] = {
    Step(true),     // Q0. START
    Step(),         // Q1. TOGGLE
    Step(),         // Q2. WAIT_TOGGLE
    Step(),         // Q3. TURN_ON
    Step(),         // Q4. TURN_OFF
    Step()          // Q5. NEXT
};

Transition transitions[] = {
    Transition(
        {(int[]){0}, 1},        // Current activated steps
        {(int[]){1, 2}, 2},     // Next steps
        transition_0),          // Transition predicate
    Transition(
        {(int[]){2}, 1},        
        {(int[]){3}, 1},        
        transition_1),          
    Transition(                 
        {(int[]){2}, 1},        
        {(int[]){4}, 1},        
        transition_2),          
    Transition(                 
        {(int[]){3}, 1},        
        {(int[]){5}, 1},        
        transition_3),          
    Transition(                 
        {(int[]){4}, 1},        
        {(int[]){5}, 1},        
        transition_4),          
    Transition(                 
        {(int[]){1, 5}, 2},     
        {(int[]){0}, 1},        
        transition_5),          
};

NonStoredAction action0 = NonStoredAction(0, {(state_handler_t[]){ 
        { ACTION_STATE_ACTIVATING, action_0 }   // action 0 is called on activating  
    }, 1});
NonStoredAction action1 = NonStoredAction(1, {(state_handler_t[]){ 
        { ACTION_STATE_ACTIVATING, action_1 }
    }, 1});
NonStoredAction action2 = NonStoredAction(2, {(state_handler_t[]){ 
        { ACTION_STATE_ACTIVATING, action_2 }
    }, 1});
NonStoredAction action3 = NonStoredAction(3, {(state_handler_t[]){ 
        { ACTION_STATE_ACTIVATING, action_3 }
    }, 1});
NonStoredAction action4 = NonStoredAction(4, {(state_handler_t[]){ 
        { ACTION_STATE_ACTIVATING, action_4 }
    }, 1});

Action* actions[] = { &action0, &action1, &action2, &action3, &action4 } ;

Application app = Application({
    {steps, 6}, 
    {actions, 5}, 
    {transitions, 6}
});

Timer tim1 = Timer(1000, false);
Timer tim2 = Timer(1000, false);
Timer tim3 = Timer(1000, false);

Clock theClock = Clock({(ClockListener*[]) {
    &app,
    &tim1, 
    &tim2, 
    &tim3
}, 4});

bool light_state = false;

void *tickClock(void *value) {
    
    do {
        sfc::time_t clocks = (sfc::time_t) std::clock();
        sfc::time_t systicks_ms = (sfc::time_t)(clocks / (CLOCKS_PER_SEC / 10000)); 

        theClock.tick(systicks_ms);
        print_state();

        usleep(100);
    } while(app.getState()->activated);
    std::cout << "thread finished" << std::endl;
}

int main() {
    pthread_t thread;
    int shutdown = 0;
    app.activate();

    if(pthread_create(&thread, NULL, tickClock, NULL)) {
        std::cout << "Could not create thread." << std::endl;
        return 1;
    }

    while(shutdown != 1) {
        std::cin >> shutdown;
    }

    app.shutdown();

    if(pthread_join(thread, NULL)) {
        std::cout << "Could not create thread." << std::endl;
        return 2;
    }
}

void action_0(const stateful_state_t &state) {
    tim1.enable();
}
void action_1(const stateful_state_t &state) {
    light_state = !light_state;
}
void action_2(const stateful_state_t &state) {
    tim2.enable();
}
void action_3(const stateful_state_t &state) {
    std::cout << "The light is on" << std::endl;
    tim3.enable();
}
void action_4(const stateful_state_t &state) {
    std::cout << "The light is off" << std::endl;
    tim3.enable();
}


bool transition_0() {
    return tim1.getState()->interrupted;
}
bool transition_1() {
    return tim2.getState()->interrupted && light_state;
}
bool transition_2() {
    return tim2.getState()->interrupted && !light_state;
}
bool transition_3() {
    return tim3.getState()->interrupted;
}
bool transition_4() {
    return tim3.getState()->interrupted;
}
bool transition_5() {
    true;
}

void print_state() {
    bool changed = false;

   for(size_t i = 0; i < 6; i++) {
       changed = changed || PTR_ACTIVATING(steps[i].getState()) || PTR_DEACTIVATING(steps[i].getState());
   }

   for(size_t i = 0; i < app.getContext()->actions.size; i++) {
        changed = changed 
            || PTR_ACTIVATING((*ARRAY_GET(app.getContext()->actions, i))->getState()) 
            || PTR_DEACTIVATING((*ARRAY_GET(app.getContext()->actions, i))->getState());
   }

    if(changed) {
        std::cout << "Q: ";

        for(size_t i = 0; i < 6; i++) {
            const char * flag = steps[i].getState()->active ? "|O|" : "|_|";
            std::cout << flag;
        }

        std::cout << "   A: ";

        for(size_t i = 0; i < app.getContext()->actions.size; i++) {
            const char * flag = (*ARRAY_GET(app.getContext()->actions, i))->getState()->active ? "|O|" : "|_|";
            std::cout << flag;
        }

        std::cout << std::endl;
    }
}
