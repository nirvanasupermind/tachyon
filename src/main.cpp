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
#include <string>
#include <vector>
#include <map>
#include <functional>

class TachyonObject { 
public:
    std::map<std::string, double> map;
    std::string str;
    std::vector<double> vec;
    std::function<double(const std::vector<double>&)> func;

    TachyonObject(const std::map<std::string, double>& map) {
        this->map = map;
    }

    TachyonObject(const std::map<std::string, double>& map, const std::string& str) {
        this->map = map;
        this->str = str;
    }

    TachyonObject(const std::map<std::string, double>& map, const std::vector<double>& vec) {
        this->map = map;
        this->vec = vec;
    }

    TachyonObject(const std::map<std::string, double>& map, const std::function<double(const std::vector<double>&)>& func) {
        this->map = map;
        this->func = func;
    }

    double get(const std::string& key) {
        if(this->map.count("prototype") == 0) {
            return map.at(key);
        } else {
            return TachyonObject::from_double(this->map.at("prototype"))->get(key);
        }
    }

    void set(const std::string& key, double val) {
        map[key] = val;
    }

    double to_double() const {
        double result;
        std::memcpy(&result, this, sizeof(result));
        return result;
    }

    static std::shared_ptr<TachyonObject> from_double(double d) {
        TachyonObject* result;
        std::memcpy(&result, &d, sizeof(result));
        return std::shared_ptr<TachyonObject>(result);
    }
};

double String = TachyonObject({}).to_double();
double Vector = TachyonObject({}).to_double();
double Function = TachyonObject({}).to_double();
double System = TachyonObject({}).to_double();
void setup() {
TachyonObject::from_double(System)->set("printDouble", (TachyonObject({{"prototype",Function}},[](const std::vector<double>& args) {
    double x = args.at(0);
    std::cout << x << '\n';
}).to_double()));
}

    )V0G0N";

    std::string filename_noext = filename.substr(0, filename.size() - 8);
    std::ofstream out_file;
    out_file.open(filename_noext + ".cpp");
    out_file << stl << "int main(){\nsetup();\n" << transpiler.code.str() << "\nreturn 0;\n}";
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