
#include <iostream>
uint64_t to_number(float x) {
    return (((*(uint64_t*)(&x)) & 0xffffffff) << 1) + 1;
}
float from_number(uint64_t x) {
    uint64_t y = x >> 1;
    return *(float*)(&y);
}

    int main(){
    auto start = std::chrono::system_clock::now();
for(uint64_t i=1ULL;(to_number(from_number(i)<from_number(2519805185ULL))) != 1ULL;i=to_number(from_number(i)+from_number(2130706433ULL))){
uint64_t x=to_number(from_number(2147483649ULL)+from_number(2147483649ULL));
}

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

return 0;
}