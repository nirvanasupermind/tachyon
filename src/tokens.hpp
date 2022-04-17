#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace eris
{
    class Token
    {
    public:
        std::string type;
        std::string value;

        Token() = default;
        Token(const std::string &type, const std::string &value) : type(type), value(value) {}

        std::string str() const
        {
            return "[" + this->type + ":" + this->value + "]";
        }
    };
}

#endif