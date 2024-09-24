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
        LPAREN,
        RPAREN
    };

    class Token {
    public:
        // int line;
        int ln;
        TokenType type;
        std::string val;
        Token(int ln, TokenType type, const std::string& val);
        std::string to_string() const;
    };
};

#endif