#include <string>

#include "token.h"

namespace eris {
    std::string token_type_str(TokenType type) {
        switch(type) {
            case TokenType::NUMBER:
                return "NUMBER";
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
            case TokenType::LPAREN:
                return "LPAREN";
            case TokenType::RPAREN:
                return "RPAREN";
            case TokenType::SEMICOLON:
                return "SEMICOLON";
            case TokenType::EOF_:
                return "EOF_";
            default:
                return "DEFAULT";
        }
    }
    

    Token::Token(TokenType type, size_t line)
        : type(type), line(line), val("") {
    }

    Token::Token(TokenType type, size_t line, const std::string &val) 
        : type(type), line(line), val(val) {
    }

    std::string Token::str() const {
        if(val == "") {
            return token_type_str(type);
        }

        return token_type_str(type) + ":" + val;
    }
} // namespace eris