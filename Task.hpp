#include <ucontext.h>
#include <functional>

struct Task {

    Task(std::function<void(Task&)> task) {
        this->task = task;
    }

    ~Task() {}

    constexpr static size_t STACK_SIZE = 8 * 1024;

    ucontext_t ctx_main, ctx_func;
    char stack_func[STACK_SIZE];

    std::function<void(Task&)> task;

    static void taskWrapper(Task* ctx) {
        ctx->task(*ctx);
    }

    void initTask() {
        getcontext(&ctx_func);
        ctx_func.uc_stack.ss_sp = stack_func;
        ctx_func.uc_stack.ss_size = STACK_SIZE;
        ctx_func.uc_link = &ctx_main;
        makecontext(&ctx_func, (void (*)())taskWrapper, 1, this);
    }

    void runTask() {
        swapcontext(&ctx_main, &ctx_func);  // перейти в my_func
    }

    void suspend() {
        swapcontext(&ctx_func, &ctx_main);
    }
};