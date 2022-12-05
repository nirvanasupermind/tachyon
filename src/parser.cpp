#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "token.h"
#include "node.h"
#include "parser.h"
#include "error.h"

namespace eris {
    Parser::Parser(const std::vector<Token>& tokens, const std::string& filename)
        : tokens(tokens), pos(0), current(Token(TokenType::EOF_, 1)), filename(filename) {
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

    std::shared_ptr<Node> Parser::basic_expr() {
        Token token = current;
        switch (token.type) {
        case TokenType::NUMBER: {
            advance();
            return std::shared_ptr<NumberNode>(new NumberNode(std::stod(token.val), token.line));
        };
        case TokenType::NULL_: {
            advance();
            return std::shared_ptr<NullNode>(new NullNode(token.line));
        };
        case TokenType::TRUE: {
            advance();
            return std::shared_ptr<TrueNode>(new TrueNode(token.line));
        };
        case TokenType::FALSE: {
            advance();
            return std::shared_ptr<FalseNode>(new FalseNode(token.line));
        };
        case TokenType::LPAREN: {
            advance();
            std::shared_ptr<Node> inner_expr = expr();
            eat(TokenType::RPAREN);
            return inner_expr;
        };
        default:
            raise_error(filename, token.line, "invalid syntax (unexpected token " + token.str() + ")");
        }
    }

    std::shared_ptr<Node> Parser::unary_expr() {
        Token op = current;    
        if (op.type == TokenType::PLUS || op.type == TokenType::MINUS) {
            advance();
            return std::shared_ptr<UnaryNode>(new UnaryNode(op.type, unary_expr(), op.line));
        }
        else {
            return basic_expr();
        }
    }

    std::shared_ptr<Node> Parser::binary_expr(const std::function<std::shared_ptr<Node>()>& operand, const std::vector<TokenType>& op_types) {
        std::shared_ptr<Node> node_a = operand();

        while (std::find(op_types.begin(), op_types.end(), current.type) != op_types.end()
            && current.type != TokenType::EOF_) {
            TokenType op = current.type;
            advance();
            std::shared_ptr<Node> node_b = operand();
            node_a = std::shared_ptr<BinaryNode>(new BinaryNode(op, node_a, node_b, node_a->line));
        }

        return node_a;
    }

    std::shared_ptr<Node> Parser::multiplicative_expr() {
        return binary_expr([this]() { return unary_expr(); }, { TokenType::MUL, TokenType::DIV, TokenType::MOD });
    }

    std::shared_ptr<Node> Parser::additive_expr() {
        return binary_expr([this]() { return multiplicative_expr(); }, { TokenType::PLUS, TokenType::MINUS });
    }

    std::shared_ptr<Node> Parser::expr() {
        return additive_expr();
    }

    std::shared_ptr<Node> Parser::expr_stmt() {
        std::shared_ptr<Node> inner_expr = expr();
        eat(TokenType::SEMICOLON);
        return inner_expr;
    }

    std::shared_ptr<Node> Parser::program() {
        std::size_t line = current.line;
        std::vector<std::shared_ptr<Node> > stmts;

        while (current.type != TokenType::EOF_) {
            stmts.push_back(expr_stmt());
        }

        return std::shared_ptr<ProgramNode>(new ProgramNode(stmts, line));
    }

    std::shared_ptr<Node> Parser::parse() {
        return program();
    }
} // namespace eris