#include <string>
#include "token.h"

namespace tachyon {
    Token::Token(int ln, TokenType type, const std::string& val) {
        this->ln = ln;
        this->type = type;
        this->val = val;
    }

    std::string Token::to_string() const {
        std::string result = "";
        switch (type) {
        case TokenType::INT:
            result += "INT:";
            break;
        case TokenType::FLOAT:
            result += "FLOT:";
            break;
        case TokenType::PLUS:
            result += "PLUS:";
            break;
        case TokenType::MINUS:
            result += "MINUS:";
            break;
        case TokenType::MUL:
            result += "MUL:";
            break;
        case TokenType::DIV:
            result += "DIV:";
            break;
        // case TokenType::NOT:
        //     result += "NOT:";
        //     break;
        // case TokenType::AND:
        //     result += "AND:";
        //     break;
        // case TokenType::OR:
        //     result += "OR:";
        //     break;
        // case TokenType::XOR:
        //     result += "XOR:";
        //     break;
        // case TokenType::LSH:
        //     result += "LSH:";
        //     break;
        // case TokenType::RSH:
        //     result += "RSH:";
        //     break;
        case TokenType::LPAREN:
            result += "LPAREN:";
            break;
        case TokenType::RPAREN:
            result += "RPAREN:";
            break;    
        }
        result += val;
        return result;
    }
};