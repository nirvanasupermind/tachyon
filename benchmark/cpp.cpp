
#include <iostream>

int main() {
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 10000000; i++) {
        double x = 2.0 + 2.0;
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}