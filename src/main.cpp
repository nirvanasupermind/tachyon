#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "error.h"
#include "token.h"
#include "lexer.h"
#include "node.h"
#include "parser.h"
#include "interpreter.h"


void run(const std::string& filename, const std::string& text) {
    try {
        eris::Lexer lexer(text, filename);
        std::vector<eris::Token> tokens = lexer.generate_tokens();
        eris::Parser parser(tokens, filename);
        std::shared_ptr<eris::Node> node = parser.parse();
        eris::Interpreter interpreter(filename);
        interpreter.visit(node.get());
    }
    catch (const std::string& e) {
        std::cerr << e << '\n';
    }
}

int main(int argc, char** argv) {
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

    return 0;
}
