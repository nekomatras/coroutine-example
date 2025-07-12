#pragma once

#include <stdexcept>
#include <ucontext.h>
#include <functional>
#include <iostream>

class Task {

    //std::optional<TResult> result = std::nullopt;

    bool isInited = false;
    bool isFin = false;

    constexpr static size_t STACK_SIZE = 8 * 1024;

    ucontext_t ctx_main, ctx_func;
    char stack_func[STACK_SIZE];

    std::function<void(Task&)> task;

    static void taskWrapper(Task* ctx, bool* isFinished) {
        ctx->task(*ctx);
        *isFinished = true;
        /* try {
            
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        } */
    }

public:

    Task(std::function<void(Task&)> task) {
        this->task = task;
    }

    ~Task() {}

    bool isInitialized() {
        return isInited;
    }

    bool isFinished() {
        return isFin;
    }

    void init() {
        if (!isInited) {
            getcontext(&ctx_func);
            ctx_func.uc_stack.ss_sp = stack_func;
            ctx_func.uc_stack.ss_size = STACK_SIZE;
            ctx_func.uc_link = &ctx_main;
            makecontext(&ctx_func, (void (*)())taskWrapper, 2, this, &isFin);
            isInited = true;
        } else {
            std::cerr << "Task reinitialized" << std::endl;
            throw new std::runtime_error("Task reinitialized");
        }
    }

    void runTask() {
        if (isInited) {
            swapcontext(&ctx_main, &ctx_func);
        } else {
            std::cerr << "Task uninitialized" << std::endl;
            throw new std::runtime_error("Task uninitialized");
        }
    }

    void suspend() {
        if (isInited) {
            swapcontext(&ctx_func, &ctx_main);
        } else {
            std::cerr << "Task uninitialized" << std::endl;
            throw new std::runtime_error("Task uninitialized");
        }
    }
};