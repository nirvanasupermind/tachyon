#include <string>
#include <vector>
#include <iostream>
#include "parser.h"

namespace tachyon {
    Parser::Parser(const std::string& fn, const std::vector<Token>& tokens) {
        this->fn = fn;
        this->tokens = tokens;
        this->tok_idx = -1;
        advance();
    }

    void Parser::advance() {
        tok_idx++;
        if (tok_idx < tokens.size()) {
            current_tok = tokens.at(tok_idx);
        }
    }

    void Parser::raise_error() {
        throw std::runtime_error(fn + ":" + std::to_string(current_tok.ln) + ": unexpected '" + (current_tok.val) + "'");
    }

    std::shared_ptr<Node> Parser::parse() {
        std::shared_ptr<Node> res = expr();
        if(current_tok.type != TokenType::EOF_) {
            raise_error();
        }
        return res;
    }

    std::shared_ptr<Node> Parser::expr() {
        return or_expr();
    }


    std::shared_ptr<Node> Parser::factor() {
        Token tok = current_tok;
        if (tok.type == TokenType::PLUS || tok.type == TokenType::MINUS || tok.type == TokenType::NOT) {
            advance();
            return std::make_shared<UnaryOpNode>(UnaryOpNode(tok, expr()));
        } else if(tok.type == TokenType::LPAREN) {
            advance();
            std::shared_ptr<Node> my_expr = expr();
            if(tok.type != TokenType::RPAREN) {
                raise_error();
            }
            advance();
            return my_expr;
        } else if (tok.type == TokenType::NUMBER) {
            advance();
            return std::make_shared<NumberNode>(NumberNode(tok));
        } else {
            raise_error();
        }
    }

    std::shared_ptr<Node> Parser::multiplicative_expr() {
        return bin_op([this]() {return this->factor(); }, { TokenType::MUL, TokenType::DIV });
    }

    std::shared_ptr<Node> Parser::additive_expr() {
        return bin_op([this]() {return this->multiplicative_expr(); }, { TokenType::PLUS, TokenType::MINUS });
    }

    std::shared_ptr<Node> Parser::shift_expr() {
        return bin_op([this]() {return this->additive_expr(); }, { TokenType::LSH, TokenType::RSH });
    }

    std::shared_ptr<Node> Parser::comp_expr() {
        return bin_op([this]() {return this->shift_expr(); }, { TokenType::LT, TokenType::GT, TokenType::LE, TokenType::GE });
    }

    std::shared_ptr<Node> Parser::eq_expr() {
        return bin_op([this]() {return this->comp_expr(); }, { TokenType::EE, TokenType::NE });
    }

    std::shared_ptr<Node> Parser::and_expr() {
        return bin_op([this]() {return this->eq_expr(); }, { TokenType::AND });
    }

    std::shared_ptr<Node> Parser::xor_expr() {
        return bin_op([this]() {return this->and_expr(); }, { TokenType::XOR });
    }

    std::shared_ptr<Node> Parser::or_expr() {
        return bin_op([this]() {return this->xor_expr(); }, { TokenType::OR });
    }

    std::shared_ptr<Node> Parser::bin_op(const std::function<std::shared_ptr<Node>()>& func, const std::vector<TokenType>& ops) {
        std::shared_ptr<Node> left = func();

        while (std::find(ops.begin(), ops.end(), current_tok.type) != ops.end()) {
            Token op_tok = current_tok;
            advance();
            std::shared_ptr<Node> right = func();
            left = std::make_shared<BinOpNode>(BinOpNode(left, op_tok, right));
        }
        
        return left;
    }
};