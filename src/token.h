#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace eris {
    // Token type
    enum class TokenType {
        NUMBER,
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
        EQ,
        EE,
        LPAREN,
        RPAREN,
        SEMICOLON,
        EOF_
    };

    std::string token_type_str(TokenType type);

    // String with an identified meaning like a numeric literal or double-equals operator
    class Token {
    public:
        TokenType type;
        std::string val;
        std::size_t line;

        // For tokens where the string value is inferred (like PLUS)
        Token(TokenType type, std::size_t line);

        Token(TokenType type, const std::string& val, std::size_t line);

        std::string str() const;    
    };
} // namespace eris

#endif // TOKEN_H