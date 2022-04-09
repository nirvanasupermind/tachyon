#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <vector>

namespace volcano
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
        TokenType type;
        double value;

        Token(TokenType type) : type(type) {}
        Token(TokenType type, double value) : type(type), value(value) {}

        explicit operator bool() const; // returns true if type != TokenType::EOF_ else returns false

        std::string str() const;
        operator std::string() const; // this will call str(), this is just to make kind of like a "__repr__" function in Python
    };

    void print_tokens(const std::vector<Token> &tokens);

} // namespace volcano

#endif // TOKENS_H