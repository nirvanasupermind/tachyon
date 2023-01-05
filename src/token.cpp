#include <string>

#include "token.h"

namespace eris {
    std::string token_type_str(TokenType type) {
        switch (type) {
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::NIL:
            return "NULL";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::VAR:
            return "VAR";
        case TokenType::IF:
            return "IF";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::FOR:
            return "FOR";
        case TokenType::FUNC:
            return "FUNC";
        case TokenType::RETURN:
            return "RETURN";
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
        case TokenType::LT:
            return "LT";
        case TokenType::LE:
            return "LE";
        case TokenType::GT:
            return "GT";
        case TokenType::GE:
            return "GE";
        case TokenType::EQ:
            return "EQ";
        case TokenType::PE:
            return "PE";
        case TokenType::EE:
            return "EE";
        case TokenType::NE:
            return "NE";
        case TokenType::LPAREN:
            return "LPAREN";
        case TokenType::RPAREN:
            return "RPAREN";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::COMMA:
            return "COMMA";
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
        if (val == "") {
            return token_type_str(type);
        }
        else {
            return token_type_str(type) + ":" + val;
        }
    }
} // namespace eris