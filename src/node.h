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
        UNARY,
        BINARY,
        EXPR_STATEMENT,
        PROGRAM
    };

    std::string node_type_str(NodeType type);

    class Node {
    public:
        NodeType type;
        size_t line;
        std::string val{};
        TokenType op;
        std::vector<Node> children{};
        Node(NodeType type, size_t line, const std::string& val);
        Node(NodeType type, size_t line, const std::vector<Node>& children);
        Node(NodeType type, size_t line, TokenType op, const std::vector<Node>& children);
        std::string str() const;
    };

    // void free_node(Node *node);
} // namespace eris

#endif // NODE_H