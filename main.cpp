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
#include "Task.hpp"

void testTask(Task& ctx) {
    std::cout << "[func] Started\n";
    std::cout << "[func] Suspend task\n";
    ctx.suspend();
    std::cout << "[func] Resumed after main\n";
    std::cout << "[func] Finished\n";

}

int main() {
    std::cout << "[main] Started\n";
    Task ctx(testTask);
    std::cout << "[main] Init and run task\n";
    ctx.initTask();
    ctx.runTask();
    std::cout << "[main] Back to the main after suspend\n";
    std::cout << "[main] Resume task\n";
    ctx.runTask();
    std::cout << "[main] Back in main after task finished\n";
    return 0;
}