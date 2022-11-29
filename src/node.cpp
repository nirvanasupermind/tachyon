#include <iostream>
#include <string>
#include <vector>

#include "token.h"
#include "node.h"

namespace eris {
    std::string node_type_str(NodeType type) {
        switch(type) {
            case NodeType::EMPTY:
                return "EMPTY";
            case NodeType::NUMBER:
                return "NUMBER";
            case NodeType::UNARY:
                return "UNARY";
            case NodeType::BINARY:
                return "BINARY";
            case NodeType::EXPR_STATEMENT:
                return "EXPR_STATEMENT";
            case NodeType::PROGRAM:
                return "PROGRAM";
            default:
                return "DEFAULT";
        }
    }
    
    Node::Node(NodeType type, size_t line, const std::string &val) 
        : type(type), line(line), op(TokenType::EOF_), val(val), children{} {
    }

    Node::Node(NodeType type, size_t line, const std::vector<Node>& children)
        : type(type), line(line), op(TokenType::EOF_), val(""), children(children) {
    }

    Node::Node(NodeType type, size_t line, TokenType op, const std::vector<Node>& children)
        : type(type), line(line), op(op), children(children) {
    }

    std::string Node::str() const {
        std::string s = node_type_str(type) + '(';

        if(children.empty()) {
            s += val + ')';
            return s;
        } else {
            if(op != TokenType::EOF_) {
                s += token_type_str(op) + ',';
            }

            for(const Node& child : children) {
                s += child.str() + ',';
            }

            s.pop_back();
            s += ')';
            return s;
        }
    }

    // void Node::free_node(Node *node) {
    //     for(const Node &child : node->children) {
    //         free(&child);
    //     }

    //     delete node;
    // }
} // namespace eris