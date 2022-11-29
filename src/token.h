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
        SEMICOLON,
        // EOF
        EOF_
    };

    std::string token_type_str(TokenType type);

    class Token {
    public:
        TokenType type;
        size_t line;
        std::string val{};

        Token(TokenType type, size_t line);
        Token(TokenType type, size_t line, const std::string &val);

        std::string str() const;    
    };
} // namespace eris

#endif // TOKEN_H