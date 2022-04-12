#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <vector>

namespace eris
{

    enum class TokenType
    {
        INT,
        DOUBLE,
        IDENTIFIER,
        VAR,
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
        std::string lexeme;

        Token(int line, TokenType type, std::string lexeme) : line(line), type(type), lexeme(lexeme) {}

        explicit operator bool() const; // returns true if type != TokenType::EOF_ else returns false

        std::string str() const;
        operator std::string() const; // this will call str(), this is just to make kind of like a "__repr__" function in Python
    };

    void print_tokens(const std::vector<Token> &tokens);

} // namespace eris

#endif // TOKENS_H