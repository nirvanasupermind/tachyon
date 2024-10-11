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