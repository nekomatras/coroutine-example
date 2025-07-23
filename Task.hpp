#pragma once

#include <stdexcept>
#include <functional>
#include <functional>
#include <memory>
#include "Context.hpp"

template <typename TAllocator = std::allocator<char>>
class Task {

    using TContext = Context<TAllocator>;
    using TFunction = std::function<void(Controller&)>;

    friend TContext;

    bool isFin = false;

    std::shared_ptr<TContext> context;

    TFunction task;

    static void taskWrapper(TFunction& task, Controller& controller, bool& isFinished) {
        task(controller);
        isFinished = true;
    }

public:

    Task& operator=(const Task&) = default;
    Task& operator=(Task&&) = default;

    Task(TFunction task, std::shared_ptr<TContext> context)
    : context(context)
    , task(task) {}

    Task(TFunction task)
        : task(task) {
        context = std::make_shared<TContext>();
    }

    Task(const Task& task)
        : task(task.task)
        , context(task.context) {}

    Task(Task&& task)
        : task(std::move(task.task))
        , context(std::move(task.context)) {}

    ~Task(){};

    bool isInitialized() {
        return context->isInitialized();
    }

    bool isFinished() {
        return isFin;
    }

    void init() {
        if (!context->isInitialized()) {
            context->init(*this, isFin);
        } else {
            throw new std::runtime_error("Task reinitialized");
        }
    }

    void runTask() {
        if (context->isInitialized()) {
            context->run();
        } else {
            throw new std::runtime_error("Task uninitialized");
        }
    }

    void suspend() {
        if (context->isInitialized()) {
            context->suspend();
        } else {
            throw new std::runtime_error("Task uninitialized");
        }
    }
};

using HeapTask = Task<>;