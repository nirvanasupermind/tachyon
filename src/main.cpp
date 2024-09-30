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

void transpile(const std::string& filename, const std::string& text, bool i) {
    tachyon::Lexer lexer(filename, text);
    std::vector<tachyon::Token> tokens = lexer.make_tokens();
    tachyon::Parser parser(filename, tokens);
    std::shared_ptr<tachyon::Node> tree = parser.parse();
    std::string filename_noext = filename.substr(0, filename.size() - 8);
    std::ofstream out_file;
    out_file.open(filename_noext + ".txt");
    out_file << tree->to_string();
    out_file.close();
// #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//     // Windows
//     system(("clang++ " + filename_noext + ".cpp -o " + filename_noext + ".exe-std=c++11").c_str());
//     if (!i) {
//         system(("del " + ffilename_noext + ".cpp").c_str());
//     }
// #else
//     // Linux and Mac
//     system(("clang++ " + filename_noext + ".cpp -o " + filename_noext + " -std=c++11").c_str());
//     if (!i) {
//         system(("rm -rf " + filename_noext + ".cpp").c_str());
//     }
// #endif
    std::size_t idx = filename.find_last_of("/\\");
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