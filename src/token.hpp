#ifndef TOKEN_HPP
#define TOKEN_HPP

namespace eris {
    enum class Token {
        // Literals
        INT,
        DOUBLE,
        // Operators
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        AND,
        OR,
        NOT,
        LT,
        LE,
        // Keywords
        LET
    };

} // namespace eris

#endif // TOKEN_HPP