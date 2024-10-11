
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
    std::map<std::string, uint64_t>* props;
    void* other_data = nullptr;
    TachyonObject(std::map<std::string, uint64_t>* props) {
        this->props = props;
    }
    TachyonObject(std::map<std::string, uint64_t>* props, void* other_data) {
        this->props = props;
        this->other_data = other_data;
    }
    ~TachyonObject() {
        free(props);
        free(other_data);
    }
    uint64_t get(const std::string& key) {
        if(props->count(key)) {
            return props->at(key);
        } else {
            uint64_t prototype = props->at("prototype");
            return (*(TachyonObject**)(&prototype))->get(key);
        }
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
uint64_t Point = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"sum",pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), new func_ptr([] (const std::vector<uint64_t>& _args) {uint64_t self= _args.at(0);
{
return pack_number(unpack_number(unpack_object(self)->get("x"))+unpack_number(unpack_object(self)->get("y")));
}return 1ULL;
})))}})));
uint64_t myPoint = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"x",2147483649ULL},{"y",2147483649ULL},{"prototype",Point}})));
(*(func_ptr*)(unpack_object(print)->other_data))({(*(func_ptr*)(unpack_object(unpack_object(myPoint)->get("sum"))->other_data))({myPoint})});

return 0;
}