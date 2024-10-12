
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>

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

{"at",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t idx = _args.at(1);

std::string self_str = *(std::string*)(unpack_object(self)->other_data);
float idx_float = unpack_number(idx);

return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(1, self_str.at(idx_float))));
})))},


{"front",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);

return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(1, self_str.front())));
})))},


{"back",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);

return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(1, self_str.back())));
})))},

{"empty",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);

return pack_number(self_str.empty());
})))},

{"size",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);

return pack_number(self_str.size());
})))},

{"empty",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);

return pack_number(self_str.empty());
})))},


{"clear",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
self_str.clear();

return 1ULL;
})))},

{"insert",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t index = _args.at(1);
uint64_t str = _args.at(2);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
self_str.insert(unpack_number(index), str_str);

return 1ULL;
})))},

{"erase",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t position = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
self_str.erase(unpack_number(position));

return 1ULL;
})))},


{"erase",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t position = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
self_str.erase(unpack_number(position));

return 1ULL;
})))},

{"pushBack",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t chr = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string chr_str = *(std::string*)(unpack_object(chr)->other_data);
self_str.push_back(chr_str.at(0));
return 1ULL;
})))},


    {"toString", pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
return _args.at(0);
})))},

{"from",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t x = _args.at(1);
if((x & 1) == 0) {
TachyonObject* obj = unpack_object(x);
if(obj->has("toString")) {
    return (*(func_ptr*)(unpack_object(obj->get("toString"))->other_data))({x});
} else {
std::ostringstream oss;
oss << obj;
return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(oss.str())));
}
} else {
std::ostringstream oss;
oss << unpack_number(x);
return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(oss.str())));
}
return 1ULL;
})))}

})));

uint64_t Vector = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({
    {"toString", pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
std::vector<uint64_t> vec = *(std::vector<uint64_t>*)(unpack_object(_args.at(0))->other_data);
std::string result = "[";
if(vec.size() == 0) {
    result = "[]";   
} else {
for(int i = 0; i < vec.size(); i++) {
    uint64_t temp = ((*(func_ptr*)(unpack_object(unpack_object(String)->get("from"))->other_data))({String,vec.at(i)}));
    result += *(std::string*)(unpack_object(temp)->other_data);
    if(i == vec.size() - 1) {
        result += "]";
    } else {
        result += ",";
    }
}
}

return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(result)));
})))}

})));

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
uint64_t x = pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string("hello guys")));
(*(func_ptr*)(unpack_object(print)->other_data))({(*(func_ptr*)(unpack_object(unpack_object(x)->get("back"))->other_data))({x})});

return 0;
}