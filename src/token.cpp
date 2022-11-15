#include <string>

#include "token.h"

namespace eris {
    std::string token_type_str(TokenType tokenType) {
        switch(tokenType) {
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
        }
    }

    Token::Token(size_t line, TokenType type)
        : line(line), type(type), value("") {
    }

    Token::Token(size_t line, TokenType type, const std::string &value) 
        : line(line), type(type), value(value) {
    }

    Token::operator std::string() const {
        if(value == "") {
            return token_type_str(type);
        }

        return token_type_str(type) + ":" + value;
    }
} // namespace eris