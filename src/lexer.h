#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>
#include <vector>

#include "token.h"

namespace eris {
    // Whitespace characters
    const std::string WHITESPACE = " \n\t";

    // Map of reserved words to token types
    const std::map<std::string, TokenType> RESERVED{
        {"null", TokenType::NULL_},
        {"null", TokenType::NULL_},
        {"var", TokenType::VAR},
    };

    // Converts the source code into a sequence of tokens
    class Lexer {
    private:
        std::string text{};
        size_t pos;
        size_t line;
        char current;
        std::string filename{};
        
        void advance();
        Token generate_number();
        Token generate_identifier();
    public:
        Lexer(const std::string& text, const std::string &filename);
        std::vector<Token> generate_tokens();
    };
} // namespace eris

#endif // LEXER_H