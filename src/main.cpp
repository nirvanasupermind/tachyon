#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "error.h"
#include "token.h"
#include "lexer.h"

void run(const std::string& filename, const std::string& text) {
    eris::Lexer lexer(filename, text);
    std::vector<eris::Token> tokens = lexer.generate_tokens();
    for (const eris::Token& token : tokens) {
        std::cout << static_cast<std::string>(token) << ' ';
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: eris [script]" << '\n';
        return 1;
    }
    else {
        std::string filename(argv[1]);
        std::ifstream inFile;

        inFile.open(filename);

        std::stringstream strStream;
        strStream << inFile.rdbuf();

        std::string text = strStream.str();

        if (text == "") {
            std::cerr << "File \"" + filename + "\" is empty or does not exist" << '\n';
            return 0;
        }

        run(filename, text);
    }

    return 0;
}

