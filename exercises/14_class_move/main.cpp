#include "../exercise.h"

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // 构造函数：动态设置容量
    explicit DynFibonacci(int capacity): cache(new size_t[capacity]), cached(2) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
    }

    // 移动构造函数
    DynFibonacci(DynFibonacci &&other) noexcept: cached(other.cached), cache(other.cache) {
        other.cache = nullptr;
        other.cached = 0;
    }

    // 移动赋值运算符
    DynFibonacci& operator=(DynFibonacci &&other) noexcept {
        if (this == &other) return *this;
        delete[] cache; // 释放当前缓存
        cached = other.cached;
        cache = other.cache;
        other.cache = nullptr;
        other.cached = 0;
        return *this;
    }

    // 析构函数
    ~DynFibonacci() {
        delete[] cache;
    }

    // 运算符重载：斐波那契数列计算
    size_t operator[](int i)  {
        if (i > cached) {
            for (int j = cached+1; j <= i; ++j) {
                cache[j] = cache[j - 1] + cache[j - 2];
            }
            cached = i;
        }
        return cache[i];
    }

    // 检查缓存是否有效
    bool is_alive() const {
        return cache != nullptr;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    printf("fib[10]=%zu\n",fib[10]);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
