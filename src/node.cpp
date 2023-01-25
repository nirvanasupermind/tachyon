#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace eris {
    NumberNode::NumberNode(double value, int line)
        : value(value) {
        this->line = line;
    }

    NodeKind NumberNode::kind() const {
        return NodeKind::NUMBER;
    }

    ParenExprNode::ParenExprNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind ParenExprNode::kind() const {
        return NodeKind::PAREN_EXPR;
    }

    UnaryExprNode::UnaryExprNode(Token op, std::shared_ptr<Node> node, int line)
        : op(op), node(node) {
        this->line = line;
    }

    NodeKind UnaryExprNode::kind() const {
        return NodeKind::UNARY_EXPR;
    }

    BinaryExprNode::BinaryExprNode(Token op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, int line)
        : op(op), node_a(node_a), node_b(node_b) {
        this->line = line;
    }

    NodeKind BinaryExprNode::kind() const {
        return NodeKind::BINARY_EXPR;
    }  


    ExprStmtNode::ExprStmtNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind ExprStmtNode::kind() const {
        return NodeKind::EXPR_STMT;
    }    
} // namespace eris