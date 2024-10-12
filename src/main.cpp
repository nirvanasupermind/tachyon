#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include "token.h"
#include "lexer.h"
#include "node.h"
#include "parser.h"
#include "transpiler.h"

void transpile(const std::string& filename, const std::string& text, bool i) {
    tachyon::Lexer lexer(filename, text);
    std::vector<tachyon::Token> tokens = lexer.make_tokens();
    tachyon::Parser parser(filename, tokens);
    std::shared_ptr<tachyon::Node> tree = parser.parse();
    tachyon::Transpiler transpiler;
    transpiler.visit(tree);
    std::string stl = R"V0G0N(
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
uint64_t index = _args.at(1);

std::string self_str = *(std::string*)(unpack_object(self)->other_data);
float index_float = unpack_number(index);

return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(1, self_str.at(index_float))));
})))},


{"set",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t index = _args.at(1);
uint64_t val = _args.at(2);

std::string* self_str = (std::string*)(unpack_object(self)->other_data);
float index_float = unpack_number(index);
std::string val_str = *(std::string*)(unpack_object(val)->other_data);

(*self_str)[(int)index_float] = val_str.at(0);

return 1ULL;
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
std::string* self_str = (std::string*)(unpack_object(self)->other_data);
self_str->clear();

return 1ULL;
})))},

{"insert",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t index = _args.at(1);
uint64_t str = _args.at(2);
std::string* self_str = (std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
self_str->insert(unpack_number(index), str_str);

return 1ULL;
})))},

{"erase",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t position = _args.at(1);
std::string* self_str = (std::string*)(unpack_object(self)->other_data);
self_str->erase(unpack_number(position));

return 1ULL;
})))},



{"pushBack",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t chr = _args.at(1);
std::string* self_str = (std::string*)(unpack_object(self)->other_data);
std::string chr_str = *(std::string*)(unpack_object(chr)->other_data);
self_str->push_back(chr_str.at(0));
return 1ULL;
})))},


{"popBack",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
std::string* self_str = (std::string*)(unpack_object(self)->other_data);
self_str->pop_back();
return 1ULL;
})))},

{"replace",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t first = _args.at(1);
uint64_t last = _args.at(2);
uint64_t str = _args.at(3);

std::string* self_str = (std::string*)(unpack_object(self)->other_data);
float first_float = unpack_number(first);
float last_float = unpack_number(last);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);

self_str->replace(self_str->begin() + first_float, self_str->begin() + last_float, str_str);
return 1ULL;
})))},


{"find",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t str = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
return pack_number(self_str.find(str_str));
})))},


{"rfind",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t str = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
return pack_number(self_str.rfind(str_str));
})))},


{"compare",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t str = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
return pack_number(self_str.compare(str_str));
})))},


{"contains",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t str = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
return pack_number(self_str.find(str_str) != std::string::npos);
})))},


{"substr",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t pos = _args.at(1);
uint64_t count = _args.at(2);

std::string self_str = *(std::string*)(unpack_object(self)->other_data);
float pos_float = unpack_number(pos);
float count_float = unpack_number(count);

return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(self_str.substr(pos_float, count_float))));
})))},

{"concat",  pack_object(new TachyonObject(new std::map<std::string, uint64_t>({}), 
new func_ptr([] (const std::vector<uint64_t>& _args) {
uint64_t self = _args.at(0);
uint64_t str = _args.at(1);
std::string self_str = *(std::string*)(unpack_object(self)->other_data);
std::string str_str = *(std::string*)(unpack_object(str)->other_data);
return pack_object(new TachyonObject(new std::map<std::string, uint64_t>({{"prototype",String}}),new std::string(self_str + str_str)));
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

    )V0G0N";

    std::string filename_noext = filename.substr(0, filename.size() - 8);
    std::ofstream out_file;
    out_file.open(filename_noext + ".cpp");
    out_file << stl << "int main(){\n" << transpiler.code.str() << "\nreturn 0;\n}";
    out_file.close();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    // Windows
    system(("clang++ " + filename_noext + ".cpp -o " + filename_noext + ".exe-std=c++11").c_str());
    if (!i) {
        system(("del " + filename_noext + ".cpp").c_str());
    }
#else
    // Linux and Mac
    system(("clang++ " + filename_noext + ".cpp -o " + filename_noext + " -std=c++11").c_str());
    if (!i) {
        system(("rm -rf " + filename_noext + ".cpp").c_str());
    }
#endif
    // std::size_t idx = filename.find_last_of("/\\");
}

int main(int argc, char** argv) {
    if ((argc < 2)) {
        std::cerr << "Usage: tachyonc [file]" << '\n';
        std::cerr << "Options (must be added after filename):" << '\n';
        std::cerr << "-i: Keep intermediate C++ file" << '\n';
        return 1;
    }
    else {
        std::string filename(argv[1]);
        std::ifstream in_file;

        in_file.open(filename);

        std::stringstream strStream;
        strStream << in_file.rdbuf();

        std::string text = strStream.str();

        if (text == "") {
            std::cerr << "File \"" + filename + "\" is empty or does not exist" << '\n';
            return 0;
        }

        try {
            if (argc == 3 && std::string(argv[2]) == "-i") {
                transpile(filename, text, true);
            }
            else {
                transpile(filename, text, false);
            }
            in_file.close();
        }
        catch (const std::string& e) {
            std::cerr << e << '\n';
            return 1;
        }
    }

    return 0;
}