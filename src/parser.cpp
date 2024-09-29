#include <string>
#include <vector>
#include <iostream>
#include "parser.h"

namespace tachyon {
    Parser::Parser(const std::string& filename, const std::vector<Token>& tokens) {
        this->filename = filename;
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
        throw std::runtime_error(filename + ":" + std::to_string(current_tok.line) + ": unexpected '" + (current_tok.val) + "'");
    }

    std::shared_ptr<Node> Parser::parse() {
        return stmt_list(TokenType::EOF_);
    }

    std::shared_ptr<Node> Parser::factor() {
        Token tok = current_tok;
        if (tok.type == TokenType::PLUS || tok.type == TokenType::MINUS || tok.type == TokenType::NOT) {
            advance();
            return std::make_shared<UnaryOpNode>(UnaryOpNode(tok, expr()));
        } else if (tok.type == TokenType::LPAREN) {
            advance();
            std::shared_ptr<Node> my_expr = expr();
            if (tok.type != TokenType::RPAREN) {
                raise_error();
            }
            advance();
            return my_expr;
        } else if (tok.type == TokenType::NUMBER) {
            advance();
            return std::make_shared<NumberNode>(NumberNode(tok));
        } else if (tok.type == TokenType::IDENTIFIER) {
            advance();
            return std::make_shared<IdentifierNode>(IdentifierNode(tok));
        } else {
            raise_error();
        }
    }

    std::shared_ptr<Node> Parser::multiplicative_expr() {
        return bin_op([this]() {return this->factor(); }, { TokenType::MUL, TokenType::DIV, TokenType::MOD });
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

    std::shared_ptr<Node> Parser::assign_expr() {
        return bin_op([this]() {return this->or_expr(); }, { TokenType::EQ, TokenType::PLUS_EQ, TokenType::MINUS_EQ,
        TokenType::MUL_EQ, TokenType::DIV_EQ, TokenType::MOD_EQ,
        TokenType::AND_EQ, TokenType::OR_EQ, TokenType::XOR_EQ,
        TokenType::LSH_EQ, TokenType::RSH_EQ });
    }

    std::shared_ptr<Node> Parser::expr() {
        return assign_expr();
    }

    std::shared_ptr<Node> Parser::expr_stmt() {
        std::shared_ptr<Node> res = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<ExprStmtNode>(ExprStmtNode(res));
    }

    std::shared_ptr<Node> Parser::var_def_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "var")) {
            raise_error();
        }
        // std::cout << current_tok.to_string() << '\n';
        advance();
        if (current_tok.type != TokenType::EQ) {
            raise_error();
        }
        advance();
        Token name_tok = current_tok;
        if (current_tok.type != TokenType::IDENTIFIER) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> val = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<VarDefNode>(VarDefNode(name_tok, val));
    }

    
    std::shared_ptr<Node> Parser::block_stmt() {
        if(current_tok.type != TokenType::LCURLY) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> stmt_list_node = stmt_list(TokenType::RCURLY);
        advance();
        return std::make_shared<BlockStmtNode>(BlockStmtNode(stmt_list_node));
    }

    std::shared_ptr<Node> Parser::stmt() {
        if (current_tok.type == TokenType::LCURLY) {
            return block_stmt();
        } else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "var") {
            return var_def_stmt();
        } else {
            return expr_stmt();
        }
    }

    std::shared_ptr<Node> Parser::stmt_list(TokenType end) {
       std::vector<std::shared_ptr<Node> > stmts;
       while(current_tok.type != end) {
            stmts.push_back(stmt());
       }
       if(current_tok.type != end) {
            raise_error();
       }
       return std::make_shared<StmtListNode>(StmtListNode(stmts));
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