#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace eris {
    // Token type
    enum class TokenType {
        NUMBER,
        STRING,
        IDENTIFIER,
        NULL_,
        TRUE,
        FALSE,
        VAR,
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        LT,
        LE,
        GT,
        GE,
        EQ,
        EE,
        NE,
        LPAREN,
        RPAREN,
        SEMICOLON,
        EOF_
    };

    std::string token_type_str(TokenType type);

    class Token {
    public:
        TokenType type;
        std::string val;
        std::size_t line;

        Token(TokenType type, std::size_t line);
        Token(TokenType type, const std::string& val, std::size_t line);
        std::string str() const;    
    };
} // namespace eris

#endif // TOKEN_H