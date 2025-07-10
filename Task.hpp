#pragma once

#include <stdexcept>
#include <ucontext.h>
#include <functional>
#include <optional>

//template<typename TResult = std::nullopt_t>
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
    }

public:

    Task(std::function<void(Task&)> task) {
        this->task = task;
    }

    ~Task() {}

    /* std::enable_if_t<!std::is_same_v<TResult, std::nullopt_t>, bool>
    hasResult() {
        return result.has_value();
    }

    std::enable_if_t<!std::is_same_v<TResult, std::nullopt_t>, void>
    saveResult(TResult result) {
        this->result = result;
    }

    std::enable_if_t<!std::is_same_v<TResult, std::nullopt_t>, TResult>
    getResult() {
        if (result.has_value()) {
            return result;
        } else {
            throw new std::runtime_error("There is no result yet");
        }
    } */

    bool isInitialized() {
        return isInited;
    }

    bool isFinished() {
        return isFin;
    }

    void initTask() {
        if (!isInited) {
            getcontext(&ctx_func);
            ctx_func.uc_stack.ss_sp = stack_func;
            ctx_func.uc_stack.ss_size = STACK_SIZE;
            ctx_func.uc_link = &ctx_main;
            makecontext(&ctx_func, (void (*)())taskWrapper, 2, this, &isFin);
            isInited = true;
        } else {
            throw new std::runtime_error("Task reinitialized");
        }
    }

    void runTask() {
        if (isInited) {
            swapcontext(&ctx_main, &ctx_func);
        } else {
            throw new std::runtime_error("Task uninitialized");
        }
    }

    void suspend() {
        if (isInited) {
            swapcontext(&ctx_func, &ctx_main);
        } else {
            throw new std::runtime_error("Task uninitialized");
        }
    }
};