#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>
#include <vector>

#include "token.h"

namespace eris {
    const std::string WHITESPACE = " \n\t";

    const std::map<std::string, TokenType> KEYWORDS{
        {"nil", TokenType::NIL},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"var", TokenType::VAR},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"func", TokenType::FUNC},
        {"return", TokenType::RETURN}
    };

    class Lexer {
    private:
        std::string text{};
        size_t pos;
        size_t line;
        char current;
        std::string filename{};
        
        void advance();
        Token generate_number();
        Token generate_string();
        Token generate_identifier();
    public:
        Lexer(const std::string& text, const std::string &filename);
        std::vector<Token> generate_tokens();
    };
} // namespace eris

#endif // LEXER_H