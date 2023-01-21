#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace eris {
    enum class TokenType {
        NUMBER,
        PLUS,
        MINUS,
        MUL,
        DIV,
        LPAREN,
        RPAREN,
        SEMICOLON
    };
    
    class Token {
    public:
        TokenType type;
        std::string val{};
        size_t line;
        Token(TokenType type, const std::string& val, size_t line);
        std::string str() const;  
    };
} // namespace eris


#endif // TOKEN_H