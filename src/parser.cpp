#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>

#include "token.h"
#include "node.h"
#include "parser.h"

namespace eris {
    Parser::Parser(const std::vector<Token>& tokens, const std::string& filename)
        : tokens(tokens), pos(0), current(Token(TokenType::EOF_, "", 1)), filename(filename) {
        advance();
    }

    void Parser::raise_error() const {
        throw std::string(filename + ":" + std::to_string(current.line) + ": invalid syntax (unexpected '" + current.val + "')");
    }

    void Parser::advance() {
        current = tokens.at(pos);
        pos++;
    }


    Token Parser::eat(TokenType type) {
        if (current.type == type) {
            Token token = current;
            advance();
            return token;
        }
        else {
            raise_error();
        }
    }

    std::shared_ptr<Node> Parser::parse() {
        return stmt_list();
    }

    std::shared_ptr<Node> Parser::stmt_list() {
        int line = current.line;
        std::vector<std::shared_ptr<Node> > stmts;
        while (current.type != TokenType::EOF_) {
            stmts.push_back(stmt());
            // std::cout << "ejq" << '\n';
        }
        return std::shared_ptr<StmtListNode>(new StmtListNode(stmts, line));
    }

    std::shared_ptr<Node> Parser::stmt() {
        if(current.type == TokenType::VAR) {
            return var_decl_stmt();
        } else {
            return expr_stmt();
        }
    }

    std::shared_ptr<Node> Parser::var_decl_stmt() {
        int line = current.line;
        eat(TokenType::VAR);
        std::string name = eat(TokenType::IDENTIFIER).val;
        eat(TokenType::EQ);
        std::shared_ptr<Node> val = expr();
        eat(TokenType::SEMICOLON);
        return std::shared_ptr<VarDeclStmtNode>(new VarDeclStmtNode(name, val, line));
    }

    std::shared_ptr<Node> Parser::expr_stmt() {
        std::shared_ptr<Node> expr_node = expr();
        eat(TokenType::SEMICOLON);
        return std::shared_ptr<ExprStmtNode>(new ExprStmtNode(expr_node, expr_node->line));
    }

    std::shared_ptr<Node> Parser::expr() {
        return or_expr();
    }

    std::shared_ptr<Node> Parser::binary_expr(const std::function<std::shared_ptr<Node>()>& operand, const std::set<TokenType>& op_types) {
        std::shared_ptr<Node> node_a = operand();

        while (std::find(op_types.begin(), op_types.end(), current.type) != op_types.end() && current.type != TokenType::EOF_) {
            Token op = current;
            advance();
            std::shared_ptr<Node> node_b = operand();
            node_a = std::shared_ptr<BinaryExprNode>(new BinaryExprNode(op, node_a, node_b, node_a->line));
        }

        return node_a;
    }

    std::shared_ptr<Node> Parser::or_expr() {
        return binary_expr([this]() { return xor_expr(); }, { TokenType::OR });
    }

    std::shared_ptr<Node> Parser::xor_expr() {
        return binary_expr([this]() { return and_expr(); }, { TokenType::XOR });
    }

    std::shared_ptr<Node> Parser::and_expr() {
        return binary_expr([this]() { return bitor_expr(); }, { TokenType::AND });
    }

    std::shared_ptr<Node> Parser::bitor_expr() {
        return binary_expr([this]() { return bitxor_expr(); }, { TokenType::BITOR });
    }

    std::shared_ptr<Node> Parser::bitxor_expr() {
        return binary_expr([this]() { return bitand_expr(); }, { TokenType::BITXOR });
    }

    std::shared_ptr<Node> Parser::bitand_expr() {
        return binary_expr([this]() { return equality_expr(); }, { TokenType::BITAND });
    }

    std::shared_ptr<Node> Parser::equality_expr() {
        return binary_expr([this]() { return comp_expr(); }, { TokenType::EE, TokenType::NE });
    }

    std::shared_ptr<Node> Parser::comp_expr() {
        return binary_expr([this]() { return shift_expr(); }, { TokenType::LT, TokenType::LE, TokenType::GT, TokenType::GE });
    }

    std::shared_ptr<Node> Parser::shift_expr() {
        return binary_expr([this]() { return additive_expr(); }, { TokenType::SL, TokenType::SR });
    }

    std::shared_ptr<Node> Parser::additive_expr() {
        return binary_expr([this]() { return multiplicative_expr(); }, { TokenType::PLUS, TokenType::MINUS });
    }

    std::shared_ptr<Node> Parser::multiplicative_expr() {
        return binary_expr([this]() { return unary_expr(); }, { TokenType::MUL, TokenType::DIV, TokenType::MOD });
    }

    std::shared_ptr<Node> Parser::unary_expr() {
        Token op = current;
        if (op.type == TokenType::PLUS || op.type == TokenType::MINUS) {
            advance();
            return std::shared_ptr<UnaryExprNode>(new UnaryExprNode(op, unary_expr(), op.line));
        }
        else {
            return primary_expr();
        }
    }

    std::shared_ptr<Node> Parser::primary_expr() {
        Token token = current;
        switch (token.type) {
        case TokenType::NIL: {
            advance();
            return std::shared_ptr<NilNode>(new NilNode(token.line));
        };
        case TokenType::CHAR: {
            advance();
            return std::shared_ptr<CharNode>(new CharNode(token.val.at(1), token.line));
        };
        case TokenType::TRUE: {
            advance();
            return std::shared_ptr<TrueNode>(new TrueNode(token.line));
        };
        case TokenType::FALSE: {
            advance();
            return std::shared_ptr<FalseNode>(new FalseNode(token.line));
        };
        case TokenType::NUMBER: {
            advance();
            return std::shared_ptr<NumberNode>(new NumberNode(std::stod(token.val), token.line));
        };
        case TokenType::LPAREN: {
            advance();
            std::shared_ptr<Node> expr_node = expr();
            eat(TokenType::RPAREN);
            return std::shared_ptr<ParenExprNode>(new ParenExprNode(expr_node, token.line));
        };
        default:
            raise_error();
        }
    }

} // namespace eris