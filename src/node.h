#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <vector>

#include "token.h"

namespace eris {
    enum class NodeType {
        EMPTY,
        NUMBER,
        STRING,
        IDENTIFIER,
        NIL,
        TRUE,
        FALSE,
        CALL_EXPR,
        UNARY_EXPR,
        BINARY_EXPR,
        ASSIGNMENT_EXPR,
        EXPR_STMT,
        VAR_DECL_STMT,
        BLOCK_STMT,
        IF_STMT,
        IF_ELSE_STMT,
        WHILE_STMT,
        FOR_STMT,
        FUNC_DECL_STMT,
        RETURN_STMT,
        PROGRAM
    };

    class Node {
    public:
        std::size_t line;
        virtual NodeType type() const = 0;
    };

    class EmptyNode: public Node {
    public:
        EmptyNode(std::size_t line);
        NodeType type() const;
    };

    class NumberNode: public Node {
    public:
        double val;
        NumberNode(double val, std::size_t line);
        NodeType type() const;
    };

    class StringNode: public Node {
    public:
        std::string val;
        StringNode(const std::string& val, std::size_t line);
        NodeType type() const;
    };

    class IdentifierNode: public Node {
    public:
        std::string val;
        IdentifierNode(const std::string& val, std::size_t line);
        NodeType type() const;
    };

    class NilNode: public Node {
    public:
        NilNode(std::size_t line);
        NodeType type() const;
    };

    class TrueNode: public Node {
    public:
        TrueNode(std::size_t line);
        NodeType type() const;
    };

    class FalseNode: public Node {
    public:
        FalseNode(std::size_t line);
        NodeType type() const;
    };

    class CallExprNode: public Node {
    public:
        std::shared_ptr<Node> callee;
        std::vector<std::shared_ptr<Node> > params;
        CallExprNode(std::shared_ptr<Node> callee, const std::vector<std::shared_ptr<Node> >& params, std::size_t line);
        NodeType type() const;
    };

    class UnaryExprNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> operand_node;
        UnaryExprNode(TokenType op, std::shared_ptr<Node> operand_node, std::size_t line);
        NodeType type() const;
    };

    class BinaryExprNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        BinaryExprNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line);
        NodeType type() const;
    };

    class AssignmentExprNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        AssignmentExprNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line);
        NodeType type() const;
    };

    class ExprStmtNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> expr_node;
        ExprStmtNode(std::shared_ptr<Node> expr_node, std::size_t line);
        NodeType type() const;
    };

    class VarDeclStmtNode: public Node {
    public:
        std::string name;
        std::shared_ptr<Node> val;
        VarDeclStmtNode(const std::string& name, std::shared_ptr<Node> val, std::size_t line);
        NodeType type() const;
    };

    class BlockStmtNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> program_node;
        BlockStmtNode(std::shared_ptr<Node> program_node, std::size_t line);
        NodeType type() const;
    };

    class IfStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;
        std::shared_ptr<Node> body;
        IfStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::size_t line);
        NodeType type() const;
    };

    class IfElseStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;
        std::shared_ptr<Node> body;
        std::shared_ptr<Node> alternate;
        IfElseStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::shared_ptr<Node> alternate, std::size_t line);
        NodeType type() const;
    };

    class WhileStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;
        std::shared_ptr<Node> body;
        WhileStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::size_t line);
        NodeType type() const;
    };

    class ForStmtNode: public Node {
    public:
        std::shared_ptr<Node> decl;
        std::shared_ptr<Node> test;
        std::shared_ptr<Node> assignment;
        std::shared_ptr<Node> body;
        ForStmtNode(std::shared_ptr<Node> decl, std::shared_ptr<Node> test,
            std::shared_ptr<Node> assignment, std::shared_ptr<Node> body, std::size_t line);
        NodeType type() const;
    };

    class FuncDeclStmtNode: public Node {
    public:
        std::string name;
        std::vector<std::string> params;
        std::shared_ptr<Node> body;

        FuncDeclStmtNode(const std::string& name, const std::vector<std::string>& params, std::shared_ptr<Node> body, std::size_t line);
        NodeType type() const;
    };

    class ReturnStmtNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> expr_node;
        ReturnStmtNode(std::shared_ptr<Node> expr_node, std::size_t line);
        NodeType type() const;
    };

    class ProgramNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        ProgramNode(const std::vector<std::shared_ptr<Node> >& stmts, std::size_t line);
        NodeType type() const;
    };

} // namespace eris

#endif // NODE_H