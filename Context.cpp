#include <ucontext.h>

#include "Context.hpp"
#include "Task.hpp"

Context::Context() {}
Context::~Context() {}

void Context::init(Task& task, bool& result) {
    getcontext(&ctx_func);
    ctx_func.uc_stack.ss_sp = stack_func;
    ctx_func.uc_stack.ss_size = STACK_SIZE;
    ctx_func.uc_link = &ctx_main;
    makecontext(&ctx_func, (TFunc)Task::taskWrapper, 2, &task, &result);
    isInited = true;
}

bool Context::isInitialized() {
    return isInited;
}

void Context::run() {
    swapcontext(&ctx_main, &ctx_func);
}

void Context::suspend() {
    swapcontext(&ctx_func, &ctx_main);
}
