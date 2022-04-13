#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace eris
{
    class Token
    {
    public:
        std::string type;
        std::string lexeme;

        Token(const std::string &type, const std::string &lexeme) : type(type), lexeme(lexeme) {}

        std::string str() const
        {
            return "[" + this->type + ":" + this->lexeme + "]";
        }
    };
}

#endif