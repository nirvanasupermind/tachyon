#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace tachyon {
    enum class TokenType {
        NUMBER,
        IDENTIFIER,
        KEYWORD,
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        NOT,
        AND,
        OR,
        XOR,
        LSH,
        RSH,
        EE,
        NE,
        LT,
        GT,
        LE,
        GE,
        EQ,
        PLUS_EQ,
        MINUS_EQ,
        MUL_EQ,
        DIV_EQ,
        MOD_EQ,
        AND_EQ,
        OR_EQ,
        XOR_EQ,
        LSH_EQ,
        RSH_EQ,
        LPAREN,
        RPAREN,
        LCURLY,
        RCURLY,
        SEMICOLON,
        EOF_
    };

    class Token {
    public:
        int line;
        TokenType type;
        std::string val;
        Token();
        Token(int line, TokenType type, const std::string& val);
        std::string to_string() const;
    };
};

#endif