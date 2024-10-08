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
#include <cstdint>

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
    std::function<uint64_t(const std::vector<uint64_t>&)> func;
    TachyonFunction(const std::function<uint64_t(const std::vector<uint64_t>&)> func, uint64_t proto) {
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
uint64_t print = pack_object(std::make_shared<TachyonFunction>(TachyonFunction([](const std::vector<uint64_t>& args) {
    if ((args.at(0) & 1) == 1) {
        std::cout << unpack_number(args.at(0)) << '\n';
    }
    return 0;
    }, Function)));

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