#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace tachyon {
    enum class TokenType {
        INT,
        FLOAT,
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
        LPAREN,
        RPAREN
    };

    class Token {
    public:
        TokenType type;
        std::string val;
        int line;

        Token(TokenType type, const std::string& val, int line);
        std::string to_string() const;
    };
};

#endif