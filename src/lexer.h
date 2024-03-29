#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include "token.h"

namespace tachyon {
    const std::map<std::string, TokenType> KEYWORDS{
        {"nil", TokenType::NIL},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"var", TokenType::VAR},
        {"block", TokenType::BLOCK},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"def", TokenType::DEF},  
        {"lambda", TokenType::LAMBDA},
        {"return", TokenType::RETURN},
        {"import", TokenType::IMPORT},
        {"cimport", TokenType::CIMPORT},
        {"try", TokenType::TRY},
        {"catch", TokenType::CATCH},
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
        Token generate_string();
        Token generate_identifier();
    public:
        Lexer(const std::string& text, const std::string& filename);
        std::vector<Token> generate_tokens();
    };
} // namespace tachyon


#endif // LEXER_H