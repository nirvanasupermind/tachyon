#include <iostream>
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
            raise_error(filename, current.line, "invalid syntax (unexpected token " + current.str() + ", expected " + token_type_str(type) + ")");
        }
    }

    std::shared_ptr<Node> Parser::basic_expr() {
        Token token = current;
        switch (token.type) {
        case TokenType::NUMBER: {
            advance();
            return std::shared_ptr<NumberNode>(new NumberNode(std::stod(token.val), token.line));
        };
        case TokenType::STRING: {
            advance();
            return std::shared_ptr<StringNode>(new StringNode(token.val, token.line));
        };
        case TokenType::IDENTIFIER: {
            advance();
            return std::shared_ptr<IdentifierNode>(new IdentifierNode(token.val, token.line));
        };
        case TokenType::NIL: {
            advance();
            return std::shared_ptr<NilNode>(new NilNode(token.line));
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
            std::shared_ptr<Node> expr_node = expr();
            eat(TokenType::RPAREN);
            return expr_node;
        };
        default:
            raise_error(filename, token.line, "invalid syntax (unexpected token " + token.str() + ")");
        }
    }

    std::shared_ptr<Node> Parser::call_member_expr() {
        std::shared_ptr<Node> result = basic_expr();
        while (current.type == TokenType::LPAREN || current.type == TokenType::PERIOD
            || current.type == TokenType::DOUBLE_COLON) {
            if (current.type == TokenType::LPAREN) {
                std::vector<std::shared_ptr<Node> > params;
                if (tokens.at(pos).type == TokenType::RPAREN) {
                    advance();
                    advance();
                }
                else {
                    while (current.type != TokenType::RPAREN) {
                        advance();
                        params.push_back(expr());
                        if (current.type != TokenType::COMMA) {
                            break;
                        }
                    }
                    advance();
                }
                result = std::shared_ptr<CallExprNode>(new CallExprNode(result, params, result->line));
            }
            else {
                TokenType op = current.type;
                advance();
                std::string member = eat(TokenType::IDENTIFIER).val;
                result = std::shared_ptr<MemberExprNode>(new MemberExprNode(op, result, member, result->line));
            }
        }
        return result;
    }

    std::shared_ptr<Node> Parser::unary_expr() {
        Token op = current;

        if (op.type == TokenType::PLUS || op.type == TokenType::MINUS) {
            advance();
            return std::shared_ptr<UnaryExprNode>(new UnaryExprNode(op.type, unary_expr(), op.line));
        }
        else {
            return call_member_expr();
        }
    }


    std::shared_ptr<Node> Parser::binary_expr(const std::function<std::shared_ptr<Node>()>& operand, const std::vector<TokenType>& op_types) {
        std::shared_ptr<Node> node_a = operand();

        while (std::find(op_types.begin(), op_types.end(), current.type) != op_types.end()
            && current.type != TokenType::EOF_) {
            TokenType op = current.type;
            advance();
            std::shared_ptr<Node> node_b = operand();
            node_a = std::shared_ptr<BinaryExprNode>(new BinaryExprNode(op, node_a, node_b, node_a->line));
        }

        return node_a;
    }

    std::shared_ptr<Node> Parser::multiplicative_expr() {
        return binary_expr([this]() { return unary_expr(); }, { TokenType::MUL, TokenType::DIV, TokenType::MOD });
    }

    std::shared_ptr<Node> Parser::additive_expr() {
        return binary_expr([this]() { return multiplicative_expr(); }, { TokenType::PLUS, TokenType::MINUS });
    }

    std::shared_ptr<Node> Parser::comparison_expr() {
        return binary_expr([this]() { return additive_expr(); }, { TokenType::LT, TokenType::LE, TokenType::GT, TokenType::GE });
    }

    std::shared_ptr<Node> Parser::equality_expr() {
        return binary_expr([this]() { return comparison_expr(); }, { TokenType::EE, TokenType::NE });
    }

    std::shared_ptr<Node> Parser::assignment_expr() {
        std::shared_ptr<Node> node_a = equality_expr();

        if (current.type == TokenType::EQ || current.type == TokenType::PE) {
            TokenType op = current.type;
            advance();
            std::shared_ptr<Node> node_b = assignment_expr();
            node_a = std::shared_ptr<AssignmentExprNode>(new AssignmentExprNode(op, node_a, node_b, node_a->line));
        }

        return node_a;
    }

    std::shared_ptr<Node> Parser::expr() {
        return assignment_expr();
    }

    std::shared_ptr<Node> Parser::expr_stmt() {
        std::shared_ptr<Node> expr_node = expr();
        eat(TokenType::SEMICOLON);
        return std::shared_ptr<ExprStmtNode>(new ExprStmtNode(expr_node, expr_node->line));
    }

    std::shared_ptr<Node> Parser::var_decl_stmt() {
        std::size_t line = current.line;
        eat(TokenType::VAR);
        std::string name = eat(TokenType::IDENTIFIER).val;
        eat(TokenType::EQ);
        std::shared_ptr<Node> val = expr();
        eat(TokenType::SEMICOLON);
        return std::shared_ptr<VarDeclStmtNode>(new VarDeclStmtNode(name, val, line));
    }

    std::shared_ptr<Node> Parser::block_stmt() {
        std::size_t line = current.line;
        eat(TokenType::LCURLY);
        std::shared_ptr<Node> program_node = program(TokenType::RCURLY);
        eat(TokenType::RCURLY);
        return std::shared_ptr<BlockStmtNode>(new BlockStmtNode(program_node, line));
    }

    std::shared_ptr<Node> Parser::if_stmt() {
        std::size_t line = current.line;
        eat(TokenType::IF);
        eat(TokenType::LPAREN);
        std::shared_ptr<Node> test = expr();
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        if (current.type == TokenType::ELSE) {
            advance();
            std::shared_ptr<Node> alternate = block_stmt();
            return std::shared_ptr<IfElseStmtNode>(new IfElseStmtNode(test, body, alternate, line));
        }
        return std::shared_ptr<IfStmtNode>(new IfStmtNode(test, body, line));
    }

    std::shared_ptr<Node> Parser::while_stmt() {
        std::size_t line = current.line;
        eat(TokenType::WHILE);
        eat(TokenType::LPAREN);
        std::shared_ptr<Node> test = expr();
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        return std::shared_ptr<WhileStmtNode>(new WhileStmtNode(test, body, line));
    }

    std::shared_ptr<Node> Parser::for_stmt() {
        std::size_t line = current.line;
        eat(TokenType::FOR);
        eat(TokenType::LPAREN);
        std::shared_ptr<Node> decl = var_decl_stmt();
        std::shared_ptr<Node> test = expr();
        eat(TokenType::SEMICOLON);
        std::shared_ptr<Node> assignment = expr();
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        return std::shared_ptr<ForStmtNode>(new ForStmtNode(decl, test, assignment, body, line));
    }

    std::shared_ptr<Node> Parser::func_decl_stmt() {
        std::size_t line = current.line;
        eat(TokenType::FUNC);
        std::string name = eat(TokenType::IDENTIFIER).val;
        eat(TokenType::LPAREN);
        std::vector<std::string> params;

        while (current.type != TokenType::RPAREN) {
            params.push_back(eat(TokenType::IDENTIFIER).val);
            if (current.type != TokenType::COMMA) {
                break;
            }
            advance();
        }

        advance();

        std::shared_ptr<Node> body = block_stmt();
        return std::shared_ptr<FuncDeclStmtNode>(new FuncDeclStmtNode(name, params, body, line));
    }

    std::shared_ptr<Node> Parser::return_stmt() {
        eat(TokenType::RETURN);
        std::shared_ptr<Node> expr_node = expr();
        eat(TokenType::SEMICOLON);
        return std::shared_ptr<ReturnStmtNode>(new ReturnStmtNode(expr_node, expr_node->line));
    }

    std::shared_ptr<Node> Parser::stmt() {
        if (current.type == TokenType::VAR) {
            return var_decl_stmt();
        }
        else if (current.type == TokenType::LCURLY) {
            return block_stmt();
        }
        else if (current.type == TokenType::IF) {
            return if_stmt();
        }
        else if (current.type == TokenType::WHILE) {
            return while_stmt();
        }
        else if (current.type == TokenType::FOR) {
            return for_stmt();
        }
        else if (current.type == TokenType::FUNC) {
            return func_decl_stmt();
        }
        else if (current.type == TokenType::RETURN) {
            return return_stmt();
        }
        else {
            return expr_stmt();
        }
    }

    std::shared_ptr<Node> Parser::program(TokenType end) {
        std::size_t line = current.line;
        std::vector<std::shared_ptr<Node> > stmts;

        while (current.type != end) {
            stmts.push_back(stmt());
        }

        return std::shared_ptr<ProgramNode>(new ProgramNode(stmts, line));
    }

    std::shared_ptr<Node> Parser::parse() {
        return program();
    }
} // namespace eris