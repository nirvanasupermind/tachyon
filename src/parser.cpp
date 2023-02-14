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

    std::shared_ptr<Node> Parser::stmt_list(TokenType end) {
        int line = current.line;
        std::vector<std::shared_ptr<Node> > stmts;
        while (current.type != end) {
            stmts.push_back(stmt());
        }
        return std::shared_ptr<StmtListNode>(new StmtListNode(stmts, line));
    }

    std::shared_ptr<Node> Parser::stmt() {
        if (current.type == TokenType::VAR) {
            return var_decl_stmt();
        }
        else if(current.type == TokenType::BLOCK) {
            advance();
            return block_stmt();
        }
        else if(current.type == TokenType::IF) {
            return if_stmt();
        }
        else if(current.type == TokenType::WHILE) {
            return while_stmt();
        }
        else if(current.type == TokenType::FOR) {
            return for_stmt();
        }        
        else {
            return expr_stmt();
        }
    }

    std::shared_ptr<Node> Parser::func_decl_stmt() {
        int line = current.line;
        eat(TokenType::DEF);
        std::string name = eat(TokenType::IDENTIFIER).val;
        std::vector<std::string> args;
        eat(TokenType::LPAREN);
        while(current.type != TokenType::EOF_ && current.type != TokenType::RPAREN) {
            args.push_back(eat(TokenType::IDENTIFIER).val);
            if(current.type == TokenType::RPAREN) {
                break; 
            } else {
                eat(TokenType::COMMA);            
            }
        }
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        return std::shared_ptr<FuncDeclStmtNode>(new FuncDeclStmtNode(name, args, body, line));
    }

    std::shared_ptr<Node> Parser::for_stmt() {
        int line = current.line;
        eat(TokenType::FOR);
        eat(TokenType::LPAREN);
        std::shared_ptr<Node> init = var_decl_stmt();
        std::shared_ptr<Node> test = expr();
        eat(TokenType::SEMICOLON);
        std::shared_ptr<Node> update = expr();
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        return std::shared_ptr<ForStmtNode>(new ForStmtNode(init, test, update, body, line));
    }

    std::shared_ptr<Node> Parser::while_stmt() {
        int line = current.line;
        eat(TokenType::WHILE);
        eat(TokenType::LPAREN);
        std::shared_ptr<Node> test = expr();
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        return std::shared_ptr<IfStmtNode>(new IfStmtNode(test, body, line));
    }

    std::shared_ptr<Node> Parser::if_stmt() {
        int line = current.line;
        eat(TokenType::IF);
        eat(TokenType::LPAREN);
        std::shared_ptr<Node> test = expr();
        eat(TokenType::RPAREN);
        std::shared_ptr<Node> body = block_stmt();
        if(current.type == TokenType::ELSE) {
            advance();
            std::shared_ptr<Node> alternate = block_stmt();            
            return std::shared_ptr<IfElseStmtNode>(new IfElseStmtNode(test, body, alternate, line));
        }
        return std::shared_ptr<IfStmtNode>(new IfStmtNode(test, body, line));
    }
    
    std::shared_ptr<Node> Parser::block_stmt() {
        int line = current.line;
        eat(TokenType::LCURLY);
        std::shared_ptr<Node> node = stmt_list(TokenType::RCURLY);
        eat(TokenType::RCURLY);
        return std::shared_ptr<BlockStmtNode>(new BlockStmtNode(node, line));
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
        return assignment_expr();
    }

    std::shared_ptr<Node> Parser::assignment_expr() {
        std::shared_ptr<Node> node_a = or_expr();

        if (current.type == TokenType::EQ) {
            Token op = current;
            advance();
            std::shared_ptr<Node> node_b = expr();
            node_a = std::shared_ptr<BinaryExprNode>(new BinaryExprNode(op, node_a, node_b, node_a->line));
        }

        return node_a;
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
        return binary_expr([this]() { return bcomp_expr(); }, { TokenType::EE, TokenType::NE });
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
        case TokenType::NUMBER: {
            advance();
            return std::shared_ptr<NumberNode>(new NumberNode(std::stod(token.val), token.line));
        };
        case TokenType::IDENTIFIER: {
            advance();
            return std::shared_ptr<IdentifierNode>(new IdentifierNode(token.val, token.line));
        };
        case TokenType::TRUE: {
            advance();
            return std::shared_ptr<TrueNode>(new TrueNode(token.line));
        };
        case TokenType::FALSE: {
            advance();
            return std::shared_ptr<FalseNode>(new FalseNode(token.line));
        };
        case TokenType::CHAR: {
            advance();
            return std::shared_ptr<CharNode>(new CharNode(token.val.at(1), token.line));
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