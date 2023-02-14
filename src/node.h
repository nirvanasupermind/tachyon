#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>
#include "token.h"

namespace eris {
    enum class NodeKind {
        NIL,
        NUMBER,
        IDENTIFIER,
        TRUE,
        FALSE,
        CHAR,
        PAREN_EXPR,
        UNARY_EXPR,
        BINARY_EXPR,
        EXPR_STMT,
        VAR_DECL_STMT,
        BLOCK_STMT,
        IF_STMT,
        IF_ELSE_STMT,
        WHILE_STMT,
        FOR_STMT,
        FUNC_DECL_STMT,
        STMT_LIST
    };

    class Node {
    public:
        int line;
        virtual NodeKind kind() const = 0;
        virtual ~Node() = default;
    };

    class NilNode: public Node {
    public:
        explicit NilNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class NumberNode: public Node {
    public:
        double val;
        explicit NumberNode(double val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class IdentifierNode: public Node {
    public:
        std::string val;
        explicit IdentifierNode(const std::string& val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class TrueNode: public Node {
    public:
        explicit TrueNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };
    
    class FalseNode: public Node {
    public:
        explicit FalseNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class CharNode: public Node {
    public:
        char val;
        explicit CharNode(char val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ParenExprNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ParenExprNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class UnaryExprNode: public Node {
    public:
        Token op;
        std::shared_ptr<Node> node;
        explicit UnaryExprNode(Token op, std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class BinaryExprNode: public Node {
    public:
        Token op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        explicit BinaryExprNode(Token op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ExprStmtNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ExprStmtNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class VarDeclStmtNode: public Node {
    public:
        std::string name;
        std::shared_ptr<Node> val;
        explicit VarDeclStmtNode(const std::string& name, std::shared_ptr<Node> val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class BlockStmtNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit BlockStmtNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class IfStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> body;
        explicit IfStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class IfElseStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> body;
        std::shared_ptr<Node> alternate;
        explicit IfElseStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::shared_ptr<Node> alternate, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class WhileStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> body;
        explicit WhileStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ForStmtNode: public Node {
    public:
        std::shared_ptr<Node> init;    
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> update;    
        std::shared_ptr<Node> body;
        explicit ForStmtNode(std::shared_ptr<Node> init, std::shared_ptr<Node> test, std::shared_ptr<Node> update, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class FuncDeclStmtNode: public Node {
    public:
        std::string name;
        std::vector<std::string> args;
        std::shared_ptr<Node> body;
        explicit FuncDeclStmtNode(const std::string& name, const std::vector<std::string>& args, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class StmtListNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        explicit StmtListNode(std::vector<std::shared_ptr<Node> > stmts, int line);
        NodeKind kind() const;
        std::string str() const;
    };
} // namespace eris

#endif // NODE_H