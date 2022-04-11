#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <vector>

namespace eris
{

    enum class TokenType
    {
        NUMBER,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        LPAREN,
        RPAREN,
        EOF_
    };

    class Token
    {
    public:
        int line;
        TokenType type;
        double value;

        Token(int line, TokenType type) : line(line), type(type) {}
        Token(int line, TokenType type, double value) : line(line), type(type), value(value) {}

        explicit operator bool() const; // returns true if type != TokenType::EOF_ else returns false

        std::string str() const;
        operator std::string() const; // this will call str(), this is just to make kind of like a "__repr__" function in Python
    };

    void print_tokens(const std::vector<Token> &tokens);

} // namespace eris

#endif // TOKENS_H