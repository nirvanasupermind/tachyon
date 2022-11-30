#include <string>
#include <vector>
#include <functional>

#include "token.h"
#include "node.h"
#include "parser.h"
#include "error.h"

namespace eris {
    Parser::Parser(const std::string& filename, const std::vector<Token>& tokens)
        : filename(filename), tokens(tokens), pos(0), current(Token(TokenType::EOF_, 1)) {
        advance();
    }

    void Parser::advance() {
        current = tokens.at(pos++);
    }

    Token Parser::eat(TokenType type) {
        if (current.type == type) {
            Token token = current;
            advance();
            return token;
        }
        else {
            raise_error(filename, current.line, "invalid syntax (unexpected token " + current.str() + ")");
        }
    }

    Node Parser::basic_expr() {
        Token token = current;
        switch (token.type) {
        case TokenType::NUMBER: {
            advance();
            return Node(NodeType::NUMBER, token.line, token.val);
        };
        case TokenType::LPAREN: {
            advance();
            Node inner_expr = expr();
            eat(TokenType::RPAREN);
            return inner_expr;
        };
        default:
            raise_error(filename, token.line, "invalid syntax (unexpected token " + token.str() + ")");
        }
    }

    Node Parser::unary_expr() {
        Token op = current;
        if (op.type == TokenType::PLUS || op.type == TokenType::MINUS) {
            advance();
            return Node(NodeType::UNARY, op.line, op.type, { unary_expr() });
        }
        else {
            return basic_expr();
        }
    }

    Node Parser::binary_expr(const std::function<Node()>& operand, TokenType op) {
        Node result = operand();

        while (current.type == op && current.type != TokenType::EOF_) {
            advance();
            Node temp = operand();
            result = Node(NodeType::BINARY, result.line, op, { result, temp });
        }

        return result;
    }

    Node Parser::mul_expr() {
        return binary_expr([this]() { return unary_expr(); }, TokenType::MUL);
    }

    Node Parser::div_expr() {
        return binary_expr([this]() { return mul_expr(); }, TokenType::DIV);
    }

    Node Parser::mod_expr() {
        return binary_expr([this]() { return div_expr(); }, TokenType::MOD);
    }

    Node Parser::add_expr() {
        return binary_expr([this]() { return mod_expr(); }, TokenType::PLUS);
    }

    Node Parser::sub_expr() {
        return binary_expr([this]() { return add_expr(); }, TokenType::MINUS);
    }

    Node Parser::expr() {
        return sub_expr();
    }

    Node Parser::expr_statement() {
        Node inner_expr = expr();
        eat(TokenType::SEMICOLON);
        return Node(NodeType::EXPR_STATEMENT, inner_expr.line, { inner_expr });
    }

    Node Parser::program() {
        std::vector<Node> statements;

        while (current.type != TokenType::EOF_) {
            statements.push_back(expr_statement());
        }

        return Node(NodeType::PROGRAM, statements.at(0).line, statements);
    }

    Node Parser::parse() {
        return program();
    }
} // namespace eris