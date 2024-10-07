
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

class Object {
public:
    std::map<std::string, uint64_t> props;
    uint64_t proto;
    std::string str;
    std::vector<uint64_t> vec;
    std::function<uint64_t(const std::vector<uint64_t>&)> func;
    Object (const std::map<std::string, uint64_t>& props) {
        this->props = props;
    }
    Object (const std::map<std::string, uint64_t>& props, uint64_t proto) {
        this->props = props;
        this->proto = proto;
    }
    Object(const std::string& str, uint64_t proto) {
        this->str = str;
        this->proto = proto;
    }
    Object(const std::vector<uint64_t>& vec, uint64_t proto) {
        this->vec = vec;
        this->proto = proto;
    }
    Object(const std::function<uint64_t(const std::vector<uint64_t>&)> func, uint64_t proto) {
        this->func = func;
        this->proto = proto;
    }

    uint64_t get(const std::string& prop) {
        if(props.count(prop) == 0) {
            return props.at(prop);
        } else {
            return std::shared_ptr<Object>((Object*)proto)->props.at(prop);
        }
    }
};


uint64_t pack_number(float x) {
    return (((*(uint64_t*)(&x)) & 0xffffffff) << 1) + 1;
}

float unpack_number(uint64_t x) {
    uint64_t y = x >> 1;
    return *(float*)(&y);
}

uint64_t pack_object(const std::shared_ptr<Object>& x) {
    return (uint64_t)(x.get());
}

std::shared_ptr<Object> unpack_object(uint64_t x) {
    return std::shared_ptr<Object>((Object*)x);
}

uint64_t String = pack_object(std::make_shared<Object>(Object({})));
uint64_t Vector = pack_object(std::make_shared<Object>(Object({})));
uint64_t Function = pack_object(std::make_shared<Object>(Object({})));
uint64_t print = pack_object(std::make_shared<Object>(Object([] (const std::vector<uint64_t>& args) {
        if((args.at(0) & 1) == 1) {
            std::cout << unpack_number(args.at(0)) << '\n';
        }
        return 0;
}, Function)));

    int main(){
unpack_object(print)->func({2168455169ULL});

return 0;
}