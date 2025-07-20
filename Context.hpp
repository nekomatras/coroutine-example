#pragma once

#include <ucontext.h>
#include <memory>

class Task;

class Context {

    constexpr static size_t STACK_SIZE = 8 * 1024;

    ucontext_t ctx_main, ctx_func;

    char* stack_func = new char[STACK_SIZE];
    //std::shared_ptr<char[]> stack_func = std::make_shared<char[]>(STACK_SIZE);

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