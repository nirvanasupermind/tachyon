
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
        case TokenType::VAR:
            result += "VAR";
            break;
        case TokenType::BLOCK:
            result += "BLOCK";
            break;
        case TokenType::IF:
            result += "IF";
            break;
        case TokenType::ELSE:
            result += "IF";
            break;
        case TokenType::WHILE:
            result += "WHILE";
            break;
        case TokenType::FOR:
            result += "WHILE";
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
            break;
        case TokenType::SL:
            result += "SL";
            break;
        case TokenType::SR:
            result += "SR";
            break;
        case TokenType::BITAND:
            result += "BITAND";
            break;
        case TokenType::BITOR:
            result += "BITOR";
            break;
        case TokenType::BITXOR:
            result += "BITXOR";
            break;
        case TokenType::AND:
            result += "BITAND";
            break;
        case TokenType::OR:
            result += "BITOR";
            break;
        case TokenType::XOR:
            result += "BITXOR";
        case TokenType::EQ:
            result += "EQ";
            break;
        case TokenType::EE:
            result += "EE";
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
        case TokenType::LCURLY:
            result += "LCURLY";
            break;
        case TokenType::RCURLY:
            result += "RCURLY";
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