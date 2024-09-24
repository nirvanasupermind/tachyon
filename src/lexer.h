#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

namespace tachyon {
    class Lexer {
    public:
        std::string fn;
        std::string text;
        int ln;
        int pos;
        char current_char;
        Lexer(const std::string& fn, const std::string& text);
        void advance();
        std::vector<Token> make_tokens();
        Token make_number();
    };
};

#endif