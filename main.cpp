/*
1. Передаем функцию // Создаем класс-обертку, который будет хранить локальные переменные
2. Помещаем ее в очередь // Создаем класс, который держит очередь и ходит по ней
3. Функция исполняется
4. В заданный момент функция останавливается // Либо просто стоп, либо с дескриптором
5. Управление передается следующей функции в очереди // После прохода очереди проверяем дескрипторы
6. Когда возвращаемся к первой, должны продолжить с места остановки
7. По завершению какой-либо функции возвращаем значение и удаляем из очереди.
*/
#include <iostream>
#include <thread>
#include "Task.hpp"
#include "Context.hpp"
#include "TaskManager.hpp"

void testTask1(Context& ctx) {
    std::cout << "[func1] Started\n";
    std::cout << "[func1] Suspend task\n";
    ctx.suspend();
    std::cout << "[func1] Resumed after main\n";
    std::cout << "[func1] Finished\n";
}

void testTask2(Context& ctx) {
    std::cout << "[func2] Started\n";
    std::cout << "[func2] Suspend task\n";
    ctx.suspend();
    std::cout << "[func2] Resumed after main\n";
    std::cout << "[func2] Not finished\n";
    ctx.suspend();
    std::cout << "[func2] Finished\n";
}

void testTask3(Context& ctx) {
    std::cout << "[func3] Started\n";
    while (true) {
        sleep(1);
        std::cout << "[func3] Do somethin " << &ctx << std::endl;
        ctx.suspend();
    }
}

TaskManager manager{};

int main() {
    try {
        Task task1(testTask1);
        Task task2(testTask2);
        Task task3(testTask3);
        manager.addTask(task1);
        manager.addTask(task3);

        std::thread t([&]() {
            sleep(5);
            manager.addTask(task2);
        });

        manager.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}