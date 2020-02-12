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

bool transition_0(const stateful_state_t& state);
bool transition_1(const stateful_state_t& state);
bool transition_2(const stateful_state_t& state);
bool transition_3(const stateful_state_t& state);
bool transition_4(const stateful_state_t& state);
bool transition_5(const stateful_state_t& state);

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

Action actions[] = {
    NonStoredAction(0, action_0),
    NonStoredAction(1, action_1),
    NonStoredAction(2, action_2),
    NonStoredAction(3, action_3),
    NonStoredAction(4, action_4),
};

component_context_t context =  {
    {steps, 6}, {actions, 5}, {transitions, 6}
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
    } while(PTR_ACTIVE(app.getState()));
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
    if(ACTIVATING(state.step_state)) {
        tim[0].enable();
    }
}
void action_1(const activation_state_t &state) {
    if(ACTIVATING(state.step_state)) {
        light_state = !light_state;
    }
}
void action_2(const activation_state_t &state) {
    if(ACTIVATING(state.step_state)) {
        tim[1].enable();
    }
}
void action_3(const activation_state_t &state) {
    if(ACTIVATING(state.step_state)) {
        std::cout << "The light is on" << std::endl;
        tim[2].enable();
    }
}
void action_4(const activation_state_t &state) {
    if(ACTIVATING(state.step_state)) {
        std::cout << "The light is off" << std::endl;
        tim[2].enable();
    }
}


bool transition_0(const stateful_state_t& state) {
    return tim->getState()->interrupted;
}
bool transition_1(const stateful_state_t& state) {
    return (tim + 1)->getState()->interrupted && light_state;
}
bool transition_2(const stateful_state_t& state) {
    return (tim + 1)->getState()->interrupted && !light_state;
}
bool transition_3(const stateful_state_t& state) {
    return (tim + 2)->getState()->interrupted;
}
bool transition_4(const stateful_state_t& state) {
    return (tim + 2)->getState()->interrupted;
}
bool transition_5(const stateful_state_t& state) {
    true;
}

void print_state() {
    bool changed = false;

    for(size_t i = 0; i < 6; i++) {
        changed = changed || PTR_ACTIVATING(steps[i].getState()) || PTR_DEACTIVATING(steps[i].getState());
    }

    if(changed) {
        std::cout << "Q: ";

        for(size_t i = 0; i < 6; i++) {
            const char * flag = steps[i].getState()->active ? "|O|" : "|_|";
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
