#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "token.h"

namespace eris {
    const std::string WHITESPACE = " \n\t";
    const std::string DIGITS = "0123456789";

    class Lexer {
    private:
        std::string filename{};
        std::string text{};
        size_t pos;
        size_t line;
        char current;
    public:
        Lexer(const std::string &filename, const std::string& text);
        void advance();
        std::vector<Token> generate_tokens();
        Token generate_number();
    };
} // namespace eris

#endif // LEXER_H