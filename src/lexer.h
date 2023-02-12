#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include "token.h"

namespace eris {
    const std::map<std::string, TokenType> KEYWORDS{
        {"nil", TokenType::NIL},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"var", TokenType::VAR},
        {"block", TokenType::BLOCK},
        {"if", TokenType::IF},
        {"while", TokenType::WHILE},
    };

    class Lexer {
    private:
        std::string text{};
        std::string filename{};
        char current;
        int pos;
        int line;
        void advance();
        Token generate_number();
        Token generate_identifier();
    public:
        Lexer(const std::string& text, const std::string& filename);
        std::vector<Token> generate_tokens();
    };
} // namespace eris


#endif // LEXER_H