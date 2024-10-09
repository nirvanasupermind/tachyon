
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <cstdint>

using func_ptr = std::function<uint64_t(const std::vector<uint64_t>&)>;

class TachyonObject {
public:
    std::map<std::string, uint64_t> props;
    uint64_t proto;
    TachyonObject() {
        this->props = {};
        this->proto = 0ULL;
    }

    TachyonObject(const std::map<std::string, uint64_t>& props) {
        this->props = props;
        this->proto = 0ULL;
    }
    TachyonObject(const std::map<std::string, uint64_t>& props, uint64_t proto) {
        this->props = props;
        this->proto = proto;
    }

    uint64_t get(const std::string& prop) {
        if (props.count(prop) == 0) {
            return props.at(prop);
        }
        else {
            return std::shared_ptr<TachyonObject>((TachyonObject*)proto)->props.at(prop);
        }
    }
};

class TachyonString : public TachyonObject {
public:
    std::string str;
    TachyonString(const std::string& str, uint64_t proto) {
        this->str = str;
        this->proto = proto;
    }
};

class TachyonVector : public TachyonObject {
public:
    std::vector<uint64_t> vec;
    TachyonVector(const std::vector<uint64_t>& vec, uint64_t proto) {
        this->vec = vec;
        this->proto = proto;
    }
};


class TachyonFunction : public TachyonObject {
public:
    func_ptr func;
    TachyonFunction(const func_ptr& func, uint64_t proto) {
        this->func = func;
        this->proto = proto;
    }
};


uint64_t pack_number(float x) {
    return (((*(uint64_t*)(&x)) & 0xffffffff) << 1) + 1;
}

float unpack_number(uint64_t x) {
    uint64_t y = x >> 1;
    return *(float*)(&y);
}

uint64_t pack_object(const std::shared_ptr<TachyonObject>& x) {
    return (uint64_t)(x.get());
}

std::shared_ptr<TachyonObject> unpack_object(uint64_t x) {
    return std::shared_ptr<TachyonObject>((TachyonObject*)x);
}

uint64_t String = pack_object(std::make_shared<TachyonObject>(TachyonObject({}, 0ULL)));
uint64_t Vector = pack_object(std::make_shared<TachyonObject>(TachyonObject({}, 0ULL)));
uint64_t Function = pack_object(std::make_shared<TachyonObject>(TachyonObject({}, 0ULL)));
uint64_t print = pack_object(std::make_shared<TachyonFunction>(TachyonFunction([](const std::vector<uint64_t>& args) -> uint64_t {
    if ((args.at(0) & 1) == 1) {
        std::cout << unpack_number(args.at(0)) << '\n';
    }
    return 0;
    }, 0ULL)));

    int main(){
std::cout << unpack_object(print) << '\n';

return 0;
}