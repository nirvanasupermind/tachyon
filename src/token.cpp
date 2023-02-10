
#include <string>
#include "token.h"

namespace eris {
    Token::Token(TokenType type, const std::string& val, int line)
        : type(type), val(val), line(line) {
    }

    std::string Token::str() const {
        std::string result = "";
        switch (type) {
        case TokenType::NUMBER:
            result += "NUMBER";
            break;
        case TokenType::CHAR:
            result += "CHAR";
            break;
        case TokenType::IDENTIFIER:
            result += "IDENTIFIER";
            break;
        case TokenType::NIL:
            result += "NIL";
            break;
        case TokenType::TRUE:
            result += "TRUE";
            break;
        case TokenType::FALSE:
            result += "FALSE";
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
        case TokenType::MOD:
            result += "MOD";
        case TokenType::SL:
            result += "SL";
        case TokenType::SR:
            result += "SR";
        case TokenType::BITAND:
            result += "BITAND";
        case TokenType::BITOR:
            result += "BITOR";
        case TokenType::BITXOR:
            result += "BITXOR";
        case TokenType::AND:
            result += "BITAND";
        case TokenType::OR:
            result += "BITOR";
        case TokenType::XOR:
            result += "BITXOR";
        case TokenType::EQ:
            result += "EQ";
            break;
        case TokenType::NE:
            result += "NE";
            break;
        case TokenType::LT:
            result += "LT";
            break;
        case TokenType::LE:
            result += "LE";
            break;
        case TokenType::GT:
            result += "GT";
            break;
        case TokenType::GE:
            result += "GE";
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
        case TokenType::EOF_:
            result += "EOF";
            break;
        }
        result += ':';
        result += val;
        return result;
    }

} // namespace eris