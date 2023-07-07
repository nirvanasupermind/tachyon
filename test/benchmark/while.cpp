#include <cinttypes>
#include <chrono>
#include <iostream>

int main() {
    double t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    double i = 0.0;
    while (i < 100000000.0) {
        i = i + 1.0;
    }
    double t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << t2 - t1 << '\n';
}