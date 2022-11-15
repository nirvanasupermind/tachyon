#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace eris {
    enum class TokenType {
        // Literals
        NUMBER,
        // Operators
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        // Other symbols
        LPAREN,
        RPAREN,
        SEMICOLON
    };

    std::string token_type_str(TokenType token_type);

    class Token {
    public:
        size_t line;

        TokenType type;
        std::string value;

        Token(size_t line, TokenType type);

        Token(size_t line, TokenType type, const std::string &value);

        operator std::string() const;    
    };
} // namespace eris

#endif // TOKEN_H