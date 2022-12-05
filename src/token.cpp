#include <string>

#include "token.h"

namespace eris {
    std::string token_type_str(TokenType type) {
        switch (type) {
        case TokenType::NUMBER:
            return "NUMBER"; 
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::NULL_:
            return "NULL";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::VAR:
            return "VAR";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::MUL:
            return "MUL";
        case TokenType::DIV:
            return "DIV";
        case TokenType::MOD:
            return "MOD";
        case TokenType::EQ:
            return "EQ";
        case TokenType::EE:
            return "EQ";
        case TokenType::LPAREN:
            return "LPAREN";
        case TokenType::RPAREN:
            return "RPAREN";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::EOF_:
            return "EOF";
        default:
            return "";
        }
    }

    Token::Token(TokenType type, size_t line)
        : type(type), line(line) {
    }

    Token::Token(TokenType type, const std::string& val, size_t line)
        : type(type), val(val), line(line) {
    }

    std::string Token::str() const {
        if(val == "") {
            return token_type_str(type);            
        } else {
            return token_type_str(type) + ":" + val;
        }
    }
} // namespace eris