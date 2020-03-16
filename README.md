# microsfc
Create a simple C++ program directly from a sequential function chart using microsfc.
## motivation
__*microsfc*__ were built as the core library of an automation project using Arduino based PLC's. We took advantage of C++ language syntax to create an automation program keeping the basic concepts of SFC. We successfuly used this library to implement the complex routines of an injection molding machine directly from a sequential function chart. 

In this first version of __*microsfc*__ we provide just a few components of SFC. If you wish to contribute to the project please feel free to contact us. 

## Example
In this example we implemented the program below

![](example.png)

### Setting up variables

First we declare the transition predicates and the action functions. 

``` cpp
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
```

next we declate the steps and actions

``` cpp
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
```

Then we declare the action handlers and the action objects. We create an array with the action references.

```cpp

state_handler_t handlers[] = {
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
```

We must create the component context, which will hold the steps, transitions and actions. 

```cpp
component_context_t context =  {
    {steps, 6}, actions, {transitions, 6}
};
```

Finaly we create our application using that context.

```cpp
Application app = Application(context);
```

We need three timers in this example.

```cpp
Timer tim[] = {
    Timer(1000, false),
    Timer(1000, false),
    Timer(1000, false)
};
```

The application works as a time-event-based component. So we must define a clock and register the application within it. We must register the timers of this example to this clock too.

```cpp
ClockListener * listeners[] = {
    &app,
    tim, 
    tim + 1, 
    tim + 2
};
Clock theClock = Clock({listeners, 4});
```

The clock ticks every time the system performs a loop. In order to trigger the time-event based components we must provide the system time in milliseconds. 

```cpp
ulong_t sys_time_ms = ...; //system current time, eg. millis() in arduino
clock.tick(sys_time_ms);

```