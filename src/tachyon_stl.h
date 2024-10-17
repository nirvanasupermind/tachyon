#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include <complex>
#include <thread>

using func_ptr = std::function<uint64_t(const std::vector<uint64_t>&)>;

uint64_t pack_number(float x) {
    uint64_t temp = 0ULL;
    std::memcpy(&temp, &x, sizeof(x));
    return (temp << 2) + 1;
}

float unpack_number(uint64_t x) {
    uint64_t y = x >> 2;
    float temp;
    std::memcpy(&temp, &y, sizeof(temp));
    return temp;
}

class TachyonObject {
public:
    std::unordered_map<std::string, uint64_t>* props;
    void* other_data = nullptr;
    TachyonObject(std::unordered_map<std::string, uint64_t>* props) {
        this->props = props;
    }
    TachyonObject(std::unordered_map<std::string, uint64_t>* props, void* other_data) {
        this->props = props;
        this->other_data = other_data;
    }
    ~TachyonObject() {
        free(props);
        free(other_data);
    }
    uint64_t get(const std::string& key) {
        if (props->count(key)) {
            return props->at(key);
        }
        else {
            uint64_t prototype = props->at("prototype");
            return (*(TachyonObject**)(&prototype))->get(key);
        }
    }
    void set(const std::string& key, uint64_t val) {
        (*props)[key] = val;
    }
    bool has(const std::string& key) {
        if (props->count(key)) {
            return true;
        }
        else if (props->count("prototype")) {
            uint64_t prototype = props->at("prototype");
            return (*(TachyonObject**)(&prototype))->has(key);
        }
        else {
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

uint64_t Math = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t String = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Vector = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Function = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t ThreadID = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Thread = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t ThisThread = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Error = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({})));

void tachyon_stl_setup() {
    unpack_object(Math)->set("cos", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::cos(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("sin", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::sin(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("tan", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::tan(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("acos", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::acos(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("asin", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::asin(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("atan", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::atan(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("atan2", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::atan2(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("cosh", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::cosh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("sinh", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::sinh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("tanh", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::tanh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("acosh", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::acosh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("asinh", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::asinh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("atanh", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::atanh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("exp", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::exp(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log10", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log10(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("exp2", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::exp2(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("expm1", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::expm1(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log1p", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log1p(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log2", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log2(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("pow", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::pow(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("sqrt", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::sqrt(unpack_number(_args.at(1))));
            }))));


    unpack_object(Math)->set("cbrt", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::cbrt(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("hypot", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::hypot(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("erf", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::erf(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("erfc", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::erfc(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("tgamma", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::tgamma(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("lgamma", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::lgamma(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("ceil", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::ceil(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("floor", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::floor(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("trunc", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::trunc(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("round", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::round(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log2", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log2(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("random", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::random_device rd;  // Seed
            std::mt19937 gen(rd());  // Mersenne Twister generator
            std::uniform_real_distribution<float> dis(0.0, 1.0); // Uniform distribution between 0 and 1

            return pack_number(dis(gen)); // Generate random float
            }))));

    unpack_object(Math)->set("PI", 2157060023ULL);

    unpack_object(Math)->set("TAU", 2173837239ULL);

    unpack_object(Math)->set("E", 2153509033ULL);

    unpack_object(Math)->set("INFINITY", pack_number(INFINITY));

    unpack_object(Math)->set("NAN", pack_number(NAN));

    unpack_object(String)->set("at", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);

            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(1, self_str.at(index_float))));
            }))));

    unpack_object(String)->set("set", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t val = _args.at(2);

            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);
            std::string val_str = *(std::string*)(unpack_object(val)->other_data);

            (*self_str)[(int)index_float] = val_str.at(0);

            return 6ULL;
            }))));

    unpack_object(String)->set("front", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(1, self_str.front())));
            }))));

    unpack_object(String)->set("back", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(1, self_str.back())));
            }))));

    unpack_object(String)->set("empty", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_number(self_str.empty());
            }))));

    unpack_object(String)->set("size", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_number(self_str.size());
            }))));

    unpack_object(String)->set("empty", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_number(self_str.empty());
            }))));

    unpack_object(String)->set("clear", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            self_str->clear();

            return 6ULL;
            }))));

    unpack_object(String)->set("insert", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t str = _args.at(2);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            self_str->insert(unpack_number(index), str_str);

            return 6ULL;
            }))));

    unpack_object(String)->set("erase", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t position = _args.at(1);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            self_str->erase(unpack_number(position));

            return 6ULL;
            }))));

    unpack_object(String)->set("pushBack", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t chr = _args.at(1);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            std::string chr_str = *(std::string*)(unpack_object(chr)->other_data);
            self_str->push_back(chr_str.at(0));
            return 6ULL;
            }))));

    unpack_object(String)->set("popBack", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            self_str->pop_back();
            return 6ULL;
            }))));

    unpack_object(String)->set("replace", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t first = _args.at(1);
            uint64_t last = _args.at(2);
            uint64_t str = _args.at(3);

            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            float first_float = unpack_number(first);
            float last_float = unpack_number(last);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);

            self_str->replace(self_str->begin() + first_float, self_str->begin() + last_float, str_str);
            return 6ULL;
            }))));

    unpack_object(String)->set("find", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.find(str_str));
            }))));

    unpack_object(String)->set("rfind", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.rfind(str_str));
            }))));

    unpack_object(String)->set("compare", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.compare(str_str));
            }))));

    unpack_object(String)->set("contains", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.find(str_str) != std::string::npos);
            }))));

    unpack_object(String)->set("substr", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t count = _args.at(2);

            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            float pos_float = unpack_number(pos);
            float count_float = unpack_number(count);

            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(self_str.substr(pos_float, count_float))));
            }))));

    unpack_object(String)->set("concat", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(self_str + str_str)));
            }))));


    unpack_object(String)->set("split", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            uint64_t delimiter = _args.at(1);
            std::string delimiter_str = *(std::string*)(unpack_object(delimiter)->other_data);
            std::vector<uint64_t> tokens;
            size_t pos = 0;
            std::string token;
            while ((pos = self_str.find(delimiter_str)) != std::string::npos) {
                token = self_str.substr(0, pos);
                tokens.push_back(pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(token))));
                self_str.erase(0, pos + delimiter_str.size());
            }
            tokens.push_back(pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(self_str))));

            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Vector} }), new std::vector<uint64_t>(tokens)));
            }))));

    unpack_object(String)->set("toString", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return _args.at(0);
            }))));

    unpack_object(String)->set("from", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t x = _args.at(1);
            if ((x & 3) == 0) {
                TachyonObject* obj = unpack_object(x);
                if (obj->has("toString")) {
                    return (*(func_ptr*)(unpack_object(obj->get("toString"))->other_data))({ x });
                }
                else {
                    std::ostringstream oss;
                    oss << obj;
                    return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
                }
            }
            else if (x == 2ULL) {
                return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string("false")));
            }
            else if (x == 10ULL) {
                return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string("true")));
            }
            else if (x == 6ULL) {
                return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string("null")));
            }
            else {
                std::ostringstream oss;
                oss << unpack_number(x);
                return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
            }
            return 6ULL;
            }))));

    unpack_object(Vector)->set("at", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);

            return self_vec.at(index_float);
            }))));


    unpack_object(Vector)->set("set", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t val = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);
            self_vec[index_float] = val;
            return 6ULL;
            }))));

    unpack_object(Vector)->set("front", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return self_vec.front();
            }))));

    unpack_object(Vector)->set("back", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return self_vec.back();
            }))));

    unpack_object(Vector)->set("empty", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return pack_number(self_vec.empty());
            }))));

    unpack_object(Vector)->set("size", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return pack_number(self_vec.size());
            }))));

    unpack_object(Vector)->set("clear", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            self_vec->clear();
            return 6ULL;
            }))));

    unpack_object(Vector)->set("insert", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t val = _args.at(2);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float pos_float = unpack_number(pos);
            self_vec->insert(self_vec->begin() + pos_float, val);
            return 6ULL;
            }))));

    unpack_object(Vector)->set("pushBack", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t val = _args.at(1);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            self_vec->push_back(val);
            ;return 6ULL;
            }))));

    unpack_object(Vector)->set("popBack", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t val = _args.at(1);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            self_vec->pop_back();
            return 6ULL;
            }))));

    unpack_object(Vector)->set("concat", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            std::vector<uint64_t> result = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            std::vector<uint64_t> other_vec = *(std::vector<uint64_t>*)(unpack_object(other)->other_data);

            result.insert(result.end(), other_vec.begin(), other_vec.end());
            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Vector} }), new std::vector<uint64_t>(result)));
            }))));

    unpack_object(Vector)->set("join", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t delimiter = _args.at(1);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            std::string delimiter_str = *(std::string*)(unpack_object(delimiter)->other_data);
            std::string result = "";
            for (int i = 0; i < self_vec.size(); i++) {
                uint64_t temp = ((*(func_ptr*)(unpack_object(unpack_object(String)->get("from"))->other_data))({ String,self_vec.at(i) }));
                result += *(std::string*)(unpack_object(temp)->other_data);
                if (i != self_vec.size() - 1) {
                    result += delimiter_str;
                }
            }
            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(result)));
            }))));


    unpack_object(Vector)->set("toString", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::vector<uint64_t> vec = *(std::vector<uint64_t>*)(unpack_object(_args.at(0))->other_data);
            std::string result = "[";
            if (vec.size() == 0) {
                result = "[]";
            }
            else {
                for (int i = 0; i < vec.size(); i++) {
                    uint64_t temp = ((*(func_ptr*)(unpack_object(unpack_object(String)->get("from"))->other_data))({ String,vec.at(i) }));
                    result += *(std::string*)(unpack_object(temp)->other_data);
                    if (i == vec.size() - 1) {
                        result += "]";
                    }
                    else {
                        result += ",";
                    }
                }
            }

            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(result)));
            }))));


    unpack_object(Thread)->set("spawn", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t func = _args.at(1);
            func_ptr thread_func = *(func_ptr*)(unpack_object(func)->other_data);
            return pack_object(new TachyonObject(new
                std::unordered_map<std::string, uint64_t>({ {"prototype",Thread} }), new std::thread(thread_func, std::vector<uint64_t>{})));
            }))));

    unpack_object(Thread)->set("joinable", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            return (self_thread->joinable() ? 10ULL : 2ULL);
            }))));

    unpack_object(Thread)->set("getID", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            std::thread::id result = self_thread->get_id();
            return pack_object(new TachyonObject(new
                std::unordered_map<std::string, uint64_t>({ {"prototype",Thread} }), new std::thread::id(result)));
            }))));
            
    unpack_object(Thread)->set("hardwareConcurrency", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::thread::hardware_concurrency());
            }))));

    unpack_object(Thread)->set("join", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            self_thread->join();
            return 6ULL;
            }))));
            
    unpack_object(Thread)->set("detach", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            self_thread->detach();
            return 6ULL;
            }))));
            

    unpack_object(ThisThread)->set("sleepFor", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t ms = _args.at(1);
            std::chrono::milliseconds duration{ms};
            std::this_thread::sleep_for(duration);
            return 6ULL;
            }))));
            
    unpack_object(Error)->set("fromMsg", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t msg = _args.at(1);
            std::cout << msg << '\n';
            return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Error}, { "msg", msg } })));
            }))));

    unpack_object(Error)->set("throw", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            throw std::runtime_error(*(std::string*)(unpack_object(unpack_object(self)->get("msg"))->other_data));
            return 6ULL;
            }))));


    // unpack_object(Complex)->set("fromComponents", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         float real = unpack_number(_args.at(1));
    //         float imag = unpack_number(_args.at(2));
    //         return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",Complex} }), new std::complex<float>(real, imag)));
    //         }))));

    // unpack_object(Complex)->set("real", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(self_complex.real());
    //         }))));

    // unpack_object(Complex)->set("imag", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(self_complex.imag());
    //         }))));


    // unpack_object(Complex)->set("abs", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::abs(self_complex));
    //         }))));


    // unpack_object(Complex)->set("arg", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::arg(self_complex));
    //         }))));


    // unpack_object(Complex)->set("norm", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::norm(self_complex));
    //         }))));

    // unpack_object(Complex)->set("norm", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::norm(self_complex));
    //         }))));

    // unpack_object(Complex)->set("toString", pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         std::ostringstream oss;
    //         oss << self_complex;
    //         return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
    //         }))));

}

uint64_t input = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    new func_ptr([](const std::vector<uint64_t>& _args) {
        std::string result;
        std::cin >> result;
        return pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(result)));
        })));

uint64_t print = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}),
    new func_ptr([](const std::vector<uint64_t>& _args) {
        uint64_t x = _args.at(0);
        if ((x & 3) == 0) {
            TachyonObject* obj = unpack_object(x);
            if (obj->has("toString")) {
                uint64_t str = (*(func_ptr*)(unpack_object(obj->get("toString"))->other_data))({ x });
                std::cout << *(std::string*)(unpack_object(str)->other_data) << '\n';
            }
            else {
                std::cout << obj << '\n';
            }
        }
        else if (x == 2ULL) {
            std::cout << "false" << '\n';
        }
        else if (x == 10ULL) {
            std::cout << "true" << '\n';
        }
        else if (x == 6ULL) {
            std::cout << "null" << '\n';
        }
        else {
            std::cout << unpack_number(x) << '\n';
        }
        return 6ULL;
        })));