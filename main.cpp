/*
1. Передаем функцию // Создаем класс-обертку, который будет хранить локальные переменные
2. Помещаем ее в очередь // Создаем класс, который держит очередь и ходит по ней
3. Функция исполняется
4. В заданный момент функция останавливается // Либо просто стоп, либо с дескриптором
5. Управление передается следующей функции в очереди // После прохода очереди проверяем дескрипторы
6. Когда возвращаемся к первой, должны продолжить с места остановки
7. По завершению какой-либо функции возвращаем значение и удаляем из очереди.
*/
#include <algorithm>
#include <iostream>
#include "Task.hpp"
#include "TaskManager.hpp"

void testTask1(Task& ctx) {
    std::cout << "[func1] Started\n";
    std::cout << "[func1] Suspend task\n";
    ctx.suspend();
    std::cout << "[func1] Resumed after main\n";
    std::cout << "[func1] Finished\n";
}

void testTask2(Task& ctx) {
    std::cout << "[func2] Started\n";
    std::cout << "[func2] Suspend task\n";
    ctx.suspend();
    std::cout << "[func2] Resumed after main\n";
    std::cout << "[func2] Not finished\n";
    ctx.suspend();
    std::cout << "[func2] Finished\n";
}



int main() {
    Task task1(testTask1);
    Task task2(testTask2);
    TaskManager manager{};
    manager.addTask(task1);
    manager.addTask(task2);
    manager.run();
    return 0;
}