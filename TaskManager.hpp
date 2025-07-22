#pragma once

#include "Task.hpp"
#include <vector>
#include <iostream>

template <typename TTask = HeapTask>
class TaskManager {

    std::vector<TTask> runningTasks;
    std::vector<TTask> tasksToExecute;


    bool isInProgress = false;

public:

    TaskManager() {}
    ~TaskManager() {}

    void addTask(TTask& task) {
        std::cout << "add" << std::endl;
        tasksToExecute.push_back(task);
    }

    void run() {
        if (isInProgress) {
            std::cerr << "TTasks already in progress" << std::endl;
            throw new std::runtime_error("TTasks already in progress");
        }

        isInProgress = true;

        while (isInProgress) {

            //runningTTasks.reserve(runningTTasks.size() + tasksToExecute.size());
            for (const auto task : tasksToExecute) {
                runningTasks.push_back(task);
            }
            tasksToExecute.clear();

            try {
                for (auto& task : runningTasks) {
                    if (!task.isInitialized()) {
                        std::cout << "init" << std::endl;
                        task.init();
                    }
                }

                for (auto& task : runningTasks) {
                    if (task.isInitialized() && !task.isFinished()) {
                        std::cout << "run " << &task << std::endl;
                        task.runTask();
                    }
                }
            } catch (const std::exception& ex) {
                std::cerr << ex.what() << std::endl;
                throw new std::runtime_error(ex.what());
            }

            runningTasks.erase(
                std::remove_if(
                    runningTasks.begin(),
                    runningTasks.end(),
                    [](TTask& task){ if (task.isFinished()) {std::cout << "rm" << std::endl;}; return task.isFinished(); }),
                runningTasks.end()
            );

            isInProgress = !(runningTasks.empty() && tasksToExecute.empty());
        }
    }

};