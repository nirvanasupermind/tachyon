#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace tachyon {
    enum class TokenType {
        NUMBER,
        CHAR,
        STRING,
        IDENTIFIER,
        NIL,
        TRUE,
        FALSE,
        VAR,
        BLOCK,
        IF,
        ELSE,
        WHILE,
        FOR,
        DEF,
        LAMBDA,
        RETURN,
        IMPORT,
        CIMPORT,
        TRY,
        CATCH,
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
        AND,
        OR,
        EQ,
        EE,
        NE,
        LT,
        LE,
        GT,
        GE,
        LPAREN,
        RPAREN,
        LCURLY,
        RCURLY,
        LSQUARE,
        RSQUARE,
        COMMA,
        COLON,
        SEMICOLON,
        PERIOD,
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
} // namespace tachyon

#endif // TOKEN_H