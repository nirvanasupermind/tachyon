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

class TachyonVal {
public:
    double num;
    TachyonVal() {
        this->num = 0.0;
    }
    TachyonVal(double num) {
        this->num = num;
    }
};

using func_ptr = std::function<std::shared_ptr<TachyonVal>(const std::vector<std::shared_ptr<TachyonVal> >&)>;


class TachyonObject: public TachyonVal {
public:
    std::unordered_map<std::string, std::shared_ptr<TachyonVal> > map;
    std::shared_ptr<TachyonObject> proto;

    TachyonObject(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map) {
        this->map = map;
    }

    TachyonObject(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto) {
        this->map = map;
        this->proto = proto;
    }
};

class TachyonString: public TachyonObject {
public:
    std::string str;

    TachyonString(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto,
        const std::string& str)
        : TachyonObject(map, proto) {
        this->str = str;
    }
};


class TachyonVector: public TachyonObject {
public:
    std::vector<std::shared_ptr<TachyonObject> > vec;

    TachyonVector(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto,
        const std::vector<std::shared_ptr<TachyonObject> >& vec)
        : TachyonObject(map, proto) {
        this->vec = vec;
    }
};

class TachyonFunction: public TachyonObject {
public:
    func_ptr func;

    TachyonFunction(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto,
        const func_ptr& func)
        : TachyonObject(map, proto) {
        this->func = func;
    }
};


std::shared_ptr<TachyonObject> String = std::make_shared<TachyonObject>(TachyonObject({}));
std::shared_ptr<TachyonObject> Vector = std::make_shared<TachyonObject>(TachyonObject({}));
std::shared_ptr<TachyonObject> Function = std::make_shared<TachyonObject>(TachyonObject({}));
std::shared_ptr<TachyonVal> print = std::make_shared<TachyonFunction>(TachyonFunction({}, Function, [] (const std::vector<std::shared_ptr<TachyonVal> > &args) {
    std::cout << args.at(0)->num << '\n';
    return std::make_shared<TachyonVal>(TachyonVal(0.0));
}));

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