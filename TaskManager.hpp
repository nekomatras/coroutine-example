#pragma once

#include "Task.hpp"
#include <vector>

class TaskManager {

    std::vector<Task> tasks;
    bool isInProgress = false;

    void initTasks() {
        for (auto& task : tasks) {
            if (!task.isInitialized()) {
                task.initTask();
            }
        }
    }

public:

    TaskManager() {}
    ~TaskManager() {}

    void addTask(Task task) {
        if (!isInProgress) {
            tasks.push_back(task);
        } else {
            throw new std::runtime_error("Tasks already in progress");
        }
    }

    void run() {
        if (isInProgress) {
            throw new std::runtime_error("Tasks already in progress");
        }

        isInProgress = !tasks.empty();
        initTasks();

        while (isInProgress) {

            for (auto& task : tasks) {
                if (task.isInitialized() && !task.isFinished()) {
                    task.runTask();
                }
            }

            tasks.erase(
                std::remove_if(tasks.begin(), tasks.end(), [](Task& task){ return task.isFinished(); }), 
                tasks.end()
            );

            isInProgress = !tasks.empty();
        }
    }

};