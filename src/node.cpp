#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace tachyon {
    NumberNode::NumberNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType NumberNode::get_type() const {
        return NodeType::NUMBER;
    }

    std::string NumberNode::to_string() const {
        return tok.to_string();
    }

    BinOpNode::BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node) {
        this->left_node = left_node;
        this->op_tok = op_tok;
        this->right_node = right_node;
    }

    NodeType BinOpNode::get_type() const {
        return NodeType::BIN_OP;
    }

    std::string BinOpNode::to_string() const {
        return "(" + this->left_node->to_string() + ", " + this->op_tok.to_string() + ", " + this->right_node->to_string() + ")";
    }

    



};

