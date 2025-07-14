#pragma once

#include <functional>
#include <memory>

class Context;

class Task {

    friend Context;

    bool isFin = false;

    std::shared_ptr<Context> context;
    std::function<void(Task&)> task;

    static void taskWrapper(Task* task, bool* isFinished) {
        task->task(*task);
        *isFinished = true;
    }

public:

    Task(std::function<void(Task&)> task, std::shared_ptr<Context> context);
    Task(std::function<void(Task&)> task);
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