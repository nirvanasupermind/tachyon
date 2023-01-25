#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include "token.h"

namespace eris {

    enum class NodeKind {
        NUMBER,
        PAREN_EXPR,
        UNARY_EXPR,
        BINARY_EXPR,
        EXPR_STMT
    };

    class Node {
    public:
        int line;
        virtual NodeKind kind() const = 0;
        virtual ~Node() = default;
    };

    class NumberNode: public Node {
    public:
        double value;
        explicit NumberNode(double value, int line);
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
} // namespace eris

#endif // NODE_H