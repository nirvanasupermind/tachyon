#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace eris {
    enum class TokenType {
        NUMBER,
        CHAR,
        IDENTIFIER,
        NIL,
        TRUE,
        FALSE,
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        SL,
        SR,
        BITAND,
        BITOR,
        BITXOR,
        EQ,
        NE,
        LT,
        LE,
        GT,
        GE,
        LPAREN,
        RPAREN,
        SEMICOLON,
        EOF_
    };
    
    class Token {
    public:
        TokenType type;
        std::string val{};
        int line;
        Token(TokenType type, const std::string& val, int line);
        std::string str() const;  
    };
} // namespace eris


#endif // TOKEN_H