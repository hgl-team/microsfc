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

void action_0(const activation_state_t &state);
void action_1(const activation_state_t &state);
void action_2(const activation_state_t &state);
void action_3(const activation_state_t &state);
void action_4(const activation_state_t &state);

void print_state();

Step steps[] = {
    Step(true),
    Step(),
    Step(),
    Step(),
    Step(),
    Step()
};

Transition transitions[] = {
    Transition({(int[]){0}, 1}, {(int[]){1, 2}, 2}, transition_0),
    Transition({(int[]){2}, 1}, {(int[]){3}, 1}, transition_1),
    Transition({(int[]){2}, 1}, {(int[]){4}, 1}, transition_2),
    Transition({(int[]){3}, 1}, {(int[]){5}, 1}, transition_3),
    Transition({(int[]){4}, 1}, {(int[]){5}, 1}, transition_4),
    Transition({(int[]){1, 5}, 2}, {(int[]){0}, 1}, transition_5),
};

action_state_handler_t handlers[] = {
    { ACTION_STATE_ACTIVATING, action_0 }
    ,{ ACTION_STATE_ACTIVATING, action_1 }
    ,{ ACTION_STATE_ACTIVATING, action_2 }
    ,{ ACTION_STATE_ACTIVATING, action_3 }
    ,{ ACTION_STATE_ACTIVATING, action_4 }
};

NonStoredAction action0 = NonStoredAction(0, {handlers, 1});
NonStoredAction action1 = NonStoredAction(1, {handlers + 1, 1});
NonStoredAction action2 = NonStoredAction(2, {handlers + 2, 1});
NonStoredAction action3 = NonStoredAction(3, {handlers + 3, 1});
NonStoredAction action4 = NonStoredAction(4, {handlers + 4, 1});

array<Action*> actions = {(Action*[]){ 
        &action0,
        &action1,
        &action2,
        &action3,
        &action4
    }, 5};
// Action actions[] = {
//     ,
//     NonStoredAction(1, {handlers + 1, 1}),
//     NonStoredAction(2, {handlers + 2, 1}),
//     NonStoredAction(3, {handlers + 3, 1}),
//     NonStoredAction(4, {handlers + 4, 1}),
// };

component_context_t context =  {
    {steps, 6}, actions, {transitions, 6}
};

Timer tim[] = {
    Timer(1000, false),
    Timer(1000, false),
    Timer(1000, false)
};

Application app = Application(context);

ClockListener * listeners[] = {
    &app,
    tim, 
    tim + 1, 
    tim + 2
};

Clock theClock = Clock({listeners, 4});
bool light_state = false;

void *tickClock(void *value) {
    
    do {
        ulong_t clocks = std::clock();
        ulong_t systicks_ms = (ulong_t)(clocks / (CLOCKS_PER_SEC / 10000)); 

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

void action_0(const activation_state_t &state) {
    tim[0].enable();
    if(ACTIVATING(state.step_state)) { }
}
void action_1(const activation_state_t &state) {
    light_state = !light_state;
    if(ACTIVATING(state.step_state)) { }
}
void action_2(const activation_state_t &state) {
    tim[1].enable();
    if(ACTIVATING(state.step_state)) { }
}
void action_3(const activation_state_t &state) {
    std::cout << "The light is on" << std::endl;
    tim[2].enable();

    if(ACTIVATING(state.step_state)) { }
}
void action_4(const activation_state_t &state) {
    std::cout << "The light is off" << std::endl;
    tim[2].enable();
    if(ACTIVATING(state.step_state)) { }
}


bool transition_0() {
    return tim->getState()->interrupted;
}
bool transition_1() {
    return (tim + 1)->getState()->interrupted && light_state;
}
bool transition_2() {
    return (tim + 1)->getState()->interrupted && !light_state;
}
bool transition_3() {
    return (tim + 2)->getState()->interrupted;
}
bool transition_4() {
    return (tim + 2)->getState()->interrupted;
}
bool transition_5() {
    true;
}

void print_state() {
    bool changed = false;

   for(size_t i = 0; i < 6; i++) {
       changed = changed || PTR_ACTIVATING(steps[i].getState()) || PTR_DEACTIVATING(steps[i].getState());
   }

   for(size_t i = 0; i < actions.size; i++) {
        changed = changed 
            || PTR_ACTIVATING((*ARRAY_GET(actions, i))->getState()) 
            || PTR_DEACTIVATING((*ARRAY_GET(actions, i))->getState());
   }

    if(changed) {
        std::cout << "Q: ";

        for(size_t i = 0; i < 6; i++) {
            const char * flag = steps[i].getState()->active ? "|O|" : "|_|";
            std::cout << flag;
        }

        std::cout << "   A: ";

        for(size_t i = 0; i < actions.size; i++) {
            const char * flag = (*ARRAY_GET(actions, i))->getState()->active ? "|O|" : "|_|";
            std::cout << flag;
        }

        std::cout << "    T: ";

        for(size_t i = 0; i < 3; i++) {
            Timer * t = tim + i;
            timer_state_t * state = t->getState();
            std::cout << "|" << (state->current_time) << "|";
        }

        std::cout << std::endl;
    }
}
