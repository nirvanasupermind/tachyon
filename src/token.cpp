
#include <string>
#include "token.h"

namespace eris {
    Token::Token(TokenType type, const std::string& val, std::size_t line)
        : type(type), val(val), line(line) {
    }

    std::string Token::str() const {
        std::string result = "";
        switch (type) {
        case TokenType::NUMBER:
            result += "NUMBER";
            break;
        case TokenType::PLUS:
            result += "PLUS";
            break;
        case TokenType::MINUS:
            result += "MINUS";
            break;
        case TokenType::MUL:
            result += "MUL";
            break;
        case TokenType::DIV:
            result += "DIV";
            break;
        case TokenType::LPAREN:
            result += "LPAREN";
            break;
        case TokenType::RPAREN:
            result += "RPAREN";
            break;
        case TokenType::SEMICOLON:
            result += "SEMICOLON";
            break;
        }
        result += ':';
        result += val;
        return result;
    }

} // namespace eris