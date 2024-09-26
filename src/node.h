#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include "token.h"

namespace tachyon {
    enum class NodeType {
        NUMBER,
        BIN_OP
    };

    class Node {
    public:
        virtual NodeType get_type() const = 0;
        virtual std::string to_string() const = 0;
    };

    class NumberNode: public Node {
    public:
        Token tok;
        NumberNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class BinOpNode: public Node {
    public:
        std::shared_ptr<Node> left_node;
        Token op_tok;
        std::shared_ptr<Node> right_node;
        BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node);
        NodeType get_type() const;
        std::string to_string() const;
    };
};

#endif