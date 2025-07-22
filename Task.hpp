#pragma once

#include <stdexcept>
#include <functional>
#include <iostream>
#include <functional>
#include <memory>
#include "Context.hpp"

template <typename TAllocator = std::allocator<char>>
class Task {

    using TContext = Context<TAllocator>;
    using TFunction = std::function<void(std::shared_ptr<Controller>&)>;

    friend TContext;

    bool isFin = false;

    std::shared_ptr<TContext> context;
    std::shared_ptr<Controller> controller
        = reinterpret_cast<std::shared_ptr<Controller>>(context);

    TFunction task;

    static void taskWrapper(TFunction task, std::shared_ptr<Controller>* controller, bool* isFinished) {
        task(*controller);
        *isFinished = true;
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
        , context(task.context) {
        std::cout << "cpy" << std::endl;
    }

    Task(Task&& task)
        : task(std::move(task.task))
        , context(std::move(task.context)) {
        std::cout << "mv" << std::endl;
    }

    ~Task(){ std::cout << "~~~" << std::endl; };

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
            std::cerr << "Task reinitialized" << std::endl;
            throw new std::runtime_error("Task reinitialized");
        }
    }

    void runTask() {
        if (context->isInitialized()) {
            context->run();
        } else {
            std::cerr << "Task uninitialized" << std::endl;
            throw new std::runtime_error("Task uninitialized");
        }
    }

    void suspend() {
        if (context->isInitialized()) {
            context->suspend();
        } else {
            std::cerr << "Task uninitialized" << std::endl;
            throw new std::runtime_error("Task uninitialized");
        }
    }
};

using HeapTask = Task<>;