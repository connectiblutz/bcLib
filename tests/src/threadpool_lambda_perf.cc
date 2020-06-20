#include <bcl/threadpool.h>
#include <memory>

#define UNUSED(x) (void)(x)

int main(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);
    int messages = 1000000;
    auto pool = std::make_unique<bcl::ThreadPool>();
    for (int i = 0; i < messages; i++) {
        pool->post(std::make_shared<bcl::ThreadPool::Runnable>([](){}));
    }
    pool->stopWhenEmpty();
    pool->join();
    return 0;
}