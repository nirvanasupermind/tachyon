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
    if(map.count(key) == 0) {
    return TachyonObject::from_double(map.at("proto"))->get(key);
    } else {
    return map.at(key);
    }
    }

    void set(const std::string& key, double val) {
        map[key] = val;
    }

    double to_double() {
        TachyonObject* temp = this;
        double result;
        std::memcpy(&result, &temp, sizeof(result));
        return result;
    }

    static std::shared_ptr<TachyonObject> from_double(double d) {
        TachyonObject* result;
        std::memcpy(&result, &d, sizeof(result));
        return std::shared_ptr<TachyonObject>(result);
    }

};


double make_tachyon_object(const std::map<std::string, double>& map) {
    return std::make_shared<TachyonObject>(TachyonObject(map))->to_double();
}

double tachyon_object_get(double obj, const std::string& key){
    return TachyonObject::from_double(obj)->get(key);
}

void tachyon_object_set(double obj, const std::string& key, double val){
    TachyonObject::from_double(obj)->set(key, val);
}

double Function = make_tachyon_object({});

double make_tachyon_function(const std::function<double(const std::vector<double>&)>& func) {
    return std::make_shared<TachyonObject>(TachyonObject({{"prototype", Function}}, func))->to_double();
}

double String = make_tachyon_object({
    {"toString", make_tachyon_function([](const std::vector<double>& args) {
        return args.at(0);
    })}
});

double Vector = make_tachyon_object({});

double make_tachyon_string(const std::string& str) {
    return std::make_shared<TachyonObject>(TachyonObject({{"prototype", String}}, str))->to_double();
}

double make_tachyon_vector(const std::vector<double>& vec) {
    return std::make_shared<TachyonObject>(TachyonObject({{"prototype", Vector}}, vec))->to_double();
}

double System = make_tachyon_object({});
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