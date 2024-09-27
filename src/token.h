#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace tachyon {
    enum class TokenType {
        NUMBER,
        PLUS,
        MINUS,
        MUL,
        DIV,
        NOT,
        AND,
        OR,
        XOR,
        EQ,
        EE,
        NE,
        LT,
        GT,
        LE,
        GE,
        LSH,
        RSH,
        LPAREN,
        RPAREN,
        EOF_
    };

    class Token {
    public:
        // int line;
        int ln;
        TokenType type;
        std::string val;
        Token();
        Token(int ln, TokenType type, const std::string& val);
        std::string to_string() const;
    };
};

#endif