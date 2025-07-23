#pragma once

#include "Task.hpp"
#include <vector>

template <typename TTask = HeapTask>
class TaskManager {

    std::vector<TTask> runningTasks;
    std::vector<TTask> tasksToExecute;
    std::mutex queueMutex;


    bool isInProgress = false;

public:

    TaskManager() {}
    ~TaskManager() {}

    void addTask(TTask& task) {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasksToExecute.push_back(task);
    }

    void run() {

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (isInProgress) {
                throw new std::runtime_error("Tasks already in progress");
            }

            isInProgress = true;
        }

        while (isInProgress) {

            runningTasks.reserve(runningTasks.size() + tasksToExecute.size());
            for (const auto task : tasksToExecute) {
                runningTasks.push_back(task);
            }
            tasksToExecute.clear();

            try {
                for (auto& task : runningTasks) {
                    if (!task.isInitialized()) {
                        task.init();
                    }
                }

                for (auto& task : runningTasks) {
                    if (task.isInitialized() && !task.isFinished()) {
                        task.runTask();
                    }
                }
            } catch (const std::exception& ex) {
                throw new std::runtime_error(ex.what());
            }

            runningTasks.erase(
                std::remove_if(
                    runningTasks.begin(),
                    runningTasks.end(),
                    [](TTask& task){ return task.isFinished(); }),
                runningTasks.end()
            );

            isInProgress = !(runningTasks.empty() && tasksToExecute.empty());
        }
    }

};