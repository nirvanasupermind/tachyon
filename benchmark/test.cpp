
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

using func_ptr =  std::function<uint64_t(const std::vector<uint64_t>&)>;

uint64_t pack_number(float x) {
    return (((*(uint64_t*)(&x)) & 0xffffffff) << 1) + 1;
}

float unpack_number(uint64_t x) {
    uint64_t y = x >> 1;
    return *((float*)(&y));
}

class TachyonObject {
public:
    std::map<std::string, uint64_t>* map;
    void* other_data;
    TachyonObject(std::map<std::string, uint64_t>* map) {
        this->map = map;
    }
    TachyonObject(std::map<std::string, uint64_t>* map, void* other_data) {
        this->map = map;
        this->other_data = other_data;
    }
};

uint64_t pack_object(TachyonObject* x) {
    return *(uint64_t*)(&x);
}

TachyonObject* unpack_object(uint64_t x) {
    return *(TachyonObject**)(&x);
}

uint64_t String = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({})));
uint64_t Vector = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({})));
uint64_t Function = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({})));
uint64_t print = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& args) {
std::cout << unpack_number(args.at(0)) << '\n';
return 1ULL;
})));

    int main(){
auto start = std::chrono::system_clock::now();
uint64_t i=1ULL;while((pack_number(unpack_number(i)<unpack_number(2519805185ULL))) != 1ULL) {i=pack_number(unpack_number(i)+unpack_number(2130706433ULL));
{
uint64_t x=pack_number(unpack_number(2147483649ULL)+unpack_number(2147483649ULL));
}}


    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
return 0;
}