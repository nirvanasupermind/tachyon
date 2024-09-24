#include <iostream>
#include "../src/token.h"
#include "../src/lexer.h"
int main() {
    while(true) {
        std::string text;
        std::cout << "\n> ";
        std::cin >> text;
        tachyon::Lexer lexer("<stdin>", text);
        std::vector<tachyon::Token> tokens = lexer.make_tokens();
        // for(int i = 0; i < tokens.size(); i++) {
        //     std::cout << tokens.at(i).to_string() << ' ';
        // }
    }
}