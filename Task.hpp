#pragma once

#include <functional>
#include <memory>

class Context;

class Task {

    friend Context;

    bool isFin = false;

    std::shared_ptr<Context> context;
    std::function<void(Context&)> task;

    static void taskWrapper(std::function<void(Context&)> task, Context* context, bool* isFinished) {
        task(*context);
        *isFinished = true;
    }

public:

    Task(std::function<void(Context&)> task, std::shared_ptr<Context> context);
    Task(std::function<void(Context&)> task);
    Task(const Task& task);
    Task(Task&& task);
    ~Task();

    Task& operator=(const Task&) = default;
    Task& operator=(Task&&) = default;

    bool isInitialized();

    bool isFinished();

    void init();

    void runTask();

    void suspend();
};