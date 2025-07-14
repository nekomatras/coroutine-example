#pragma once

#include <ucontext.h>

class Task;

class Context {

    constexpr static size_t STACK_SIZE = 8 * 1024;

    ucontext_t ctx_main, ctx_func;
    char stack_func[STACK_SIZE];

    using TFunc = void (*)();

    bool isInited = false;

public:

    Context();
    ~Context();

    void init(Task& task, bool& result);

    bool isInitialized();

    void run();

    void suspend();
};