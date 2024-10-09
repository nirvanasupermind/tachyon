#include <iostream>
#include <functional>

class MyClass {
public:
    std::function<int(int)> func;
    MyClass(const std::function<int(int)>& func) {
        this->func = func;
    }
};

uint64_t pack_object(const std::shared_ptr<MyClass>& x) {
    std::cout << &(x->func) << '\n';
    return (uint64_t)(x.get());
}

std::shared_ptr<MyClass> unpack_object(uint64_t x) {
    MyClass* y;
    std::memcpy(&y, &x, sizeof(x));
    std::cout << &(y->func) << '\n';
    return std::shared_ptr<MyClass>(y);
}

int main() {
    uint64_t myObject = pack_object(std::make_shared<MyClass>(MyClass([] (int x) {
        return x * x;
    })));
    std::cout << unpack_object(myObject)->func(5) << '\n';
    return 0;
}