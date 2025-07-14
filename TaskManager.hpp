#pragma once

#include "Task.hpp"
#include "Context.hpp"
#include <vector>
#include <iostream>

class TaskManager {

    std::vector<Task> runningTasks;
    std::vector<Task> tasksToExecute;


    bool isInProgress = false;

public:

    TaskManager() {}
    ~TaskManager() {}

    void addTask(Task& task) {
        std::cout << "add" << std::endl;
        tasksToExecute.push_back(task);
    }

    void run() {
        if (isInProgress) {
            std::cerr << "Tasks already in progress" << std::endl;
            throw new std::runtime_error("Tasks already in progress");
        }

        isInProgress = true;

        while (isInProgress) {

            //runningTasks.reserve(runningTasks.size() + tasksToExecute.size());
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
                        std::cout << "run" << std::endl;
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
                    [](Task& task){ if (task.isFinished()) {std::cout << "rm" << std::endl;}; return task.isFinished(); }),
                runningTasks.end()
            );

            isInProgress = !(runningTasks.empty() && tasksToExecute.empty());
        }
    }

};