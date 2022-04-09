#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "tokens.h"

namespace volcano
{

    std::string Token::str() const
    {
        std::string result;

        switch (type)
        {
        case TokenType::NUMBER:
            result += "NUMBER:" + std::to_string(value);
            break;
        case TokenType::PLUS:
            result += "PLUS";
            break;
        case TokenType::MINUS:
            result += "MINUS";
            break;
        case TokenType::MULTIPLY:
            result += "MULTIPLY";
            break;
        case TokenType::DIVIDE:
            result += "DIVIDE";
            break;
        case TokenType::LPAREN:
            result += "LPAREN";
            break;
        case TokenType::RPAREN:
            result += "RPAREN";
            break;
        case TokenType::EOF_:
            result += "EOF";
            break;
        }

        return result;
    }

    Token::operator bool() const
    {
        if (type != TokenType::EOF_)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Token::operator std::string() const
    {
        return str();
    }

    void print_tokens(const std::vector<Token> &tokens)
    {
        std::ostringstream oss;
        bool empty = true;

        oss << "[";

        for (const Token &token : tokens)
        {
            if (empty)
            {
                empty = false;
                oss << token.str();
            }
            else
            {
                oss << ", " << token.str();
            }
        }

        oss << "]";

        std::cout << oss.str() << '\n';
    }

} // namespace volcano
