
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
    bool has(const std::string& key) {
        if(props->count(key)) {
            return true;
        } else if(props->count("prototype")) {
            uint64_t prototype = props->at("prototype");
            return (*(TachyonObject**)(&prototype))->has(key);
        } else {
            return false;
        }
    }
};

uint64_t pack_object(TachyonObject* x) {
    return *(uint64_t*)(&x);
}

TachyonObject* unpack_object(uint64_t x) {
    return *(TachyonObject**)(&x);
}

uint64_t String = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({
    {"toString", pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
return _args.at(0);
})))}

})));

uint64_t Vector = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({})));
uint64_t Function = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({})));
uint64_t print = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t x = _args.at(0);
if((x & 1) == 0) {
TachyonObject* obj = unpack_object(x);
if(obj->has("toString")) {
uint64_t str = (*(func_ptr*)(unpack_object(obj->get("toString"))->other_data))({x});
std::cout << *(std::string*)(unpack_object(str)->other_data) << '\n';
} else {
    std::cout << obj << '\n';
}
} else {
std::cout << unpack_number(x) << '\n';
}
return 1ULL;
})));

    int main(){
uint64_t Complex = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"toString",pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), new func_ptr([=] (const std::vector<uint64_t>& _args) {uint64_t x= _args.at(0);
{
return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string("SO COMPLEX")));
}return 1ULL;
})))}})));
(*(func_ptr*)(unpack_object(print)->other_data))({pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",Complex}})))});

return 0;
}