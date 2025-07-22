#pragma once

#include <ucontext.h>
#include <memory>
#include "Controller.hpp"

template <typename TAllocator = std::allocator<char>>
class Context : Controller{

    using TFunc = void (*)();

    constexpr static size_t STACK_SIZE = 8 * 1024;

    ucontext_t ctx_main, ctx_func;

    std::shared_ptr<char[]> stack_func = std::allocate_shared<char[]>(TAllocator{}, STACK_SIZE);

    bool isInited = false;

public:

Context() {}
~Context() {}

template <typename TTask>
void init(TTask& task, bool& result) {
    getcontext(&ctx_func);
    ctx_func.uc_stack.ss_sp = stack_func.get();
    ctx_func.uc_stack.ss_size = STACK_SIZE;
    ctx_func.uc_link = &ctx_main;
    makecontext(&ctx_func, (TFunc)TTask::taskWrapper, 2, &task.task, std::make_shared(this), &result);
    isInited = true;
}

bool isInitialized() {
    return isInited;
}

void run() {
    swapcontext(&ctx_main, &ctx_func);
}

void suspend() override{
    swapcontext(&ctx_func, &ctx_main);
}
};