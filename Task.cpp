#include <stdexcept>
#include <functional>
#include <iostream>

#include "Task.hpp"
#include "Context.hpp"

Task::Task(std::function<void(Task&)> task, std::shared_ptr<Context> context)
    : context(context)
    , task(task) {}

Task::Task(std::function<void(Task&)> task)
    : task(task) {
    context = std::make_shared<Context>();
}

Task::Task(const Task& task)
    : task(task.task)
    , context(task.context) {
    std::cout << "cpy" << std::endl;
}

Task::Task(Task&& task)
    : task(std::move(task.task))
    , context(std::move(task.context)) {
    std::cout << "mv" << std::endl;
}

Task::~Task(){ std::cout << "~~~" << std::endl; };

bool Task::isInitialized() {
    return context->isInitialized();
}

bool Task::isFinished() {
    return isFin;
}

void Task::init() {
    if (!context->isInitialized()) {
        context->init(*this, isFin);
    } else {
        std::cerr << "Task reinitialized" << std::endl;
        throw new std::runtime_error("Task reinitialized");
    }
}

void Task::runTask() {
    if (context->isInitialized()) {
        context->run();
    } else {
        std::cerr << "Task uninitialized" << std::endl;
        throw new std::runtime_error("Task uninitialized");
    }
}

void Task::suspend() {
    if (context->isInitialized()) {
        context->suspend();
    } else {
        std::cerr << "Task uninitialized" << std::endl;
        throw new std::runtime_error("Task uninitialized");
    }
}