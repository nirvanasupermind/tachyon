#include <iostream>
#include "../src/token.h"
#include "../src/lexer.h"
#include "../src/parser.h"
int main() {
    while(true) {
        std::string text;
        std::cout << "\n> ";
        std::cin >> text;
        tachyon::Lexer lexer("<stdin>", text);
        std::vector<tachyon::Token> tokens = lexer.make_tokens();
        tachyon::Parser parser("<stdin>", tokens);
        std::cout << parser.expr()->to_string() << '\n';
        // for(int i = 0; i < tokens.size(); i++) {
        //     std::cout << tokens.at(i).to_string() << ' ';
        // }
    }
}