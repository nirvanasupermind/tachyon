#include <string>
#include <vector>

#include "token.h"
#include "node.h"

namespace eris {
    EmptyNode::EmptyNode(std::size_t line) {
        this->line = line;
    }

    NodeType EmptyNode::type() const {
        return NodeType::EMPTY;
    }

    NumberNode::NumberNode(double val, std::size_t line)
        : val(val) {
        this->line = line;
    }

    NodeType NumberNode::type() const {
        return NodeType::NUMBER;
    }

    StringNode::StringNode(const std::string& val, std::size_t line)
        : val(val) {
        this->line = line;
    }

    NodeType StringNode::type() const {
        return NodeType::STRING;
    }

    IdentifierNode::IdentifierNode(const std::string& val, std::size_t line)
        : val(val) {
        this->line = line;
    }

    NodeType IdentifierNode::type() const {
        return NodeType::IDENTIFIER;
    }

    NullNode::NullNode(std::size_t line) {
        this->line = line;
    }

    NodeType NullNode::type() const {
        return NodeType::NULL_;
    }

    TrueNode::TrueNode(std::size_t line) {
        this->line = line;
    }

    NodeType TrueNode::type() const {
        return NodeType::TRUE;
    }

    FalseNode::FalseNode(std::size_t line) {
        this->line = line;
    }

    NodeType FalseNode::type() const {
        return NodeType::FALSE;
    }

    UnaryNode::UnaryNode(TokenType op, std::shared_ptr<Node> operand_node, std::size_t line)
        : op(op), operand_node(operand_node) {
        this->line = line;
    }

    NodeType UnaryNode::type() const {
        return NodeType::UNARY;
    }

    BinaryNode::BinaryNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line)
        : op(op), node_a(node_a), node_b(node_b) {
        this->line = line;
    }

    NodeType BinaryNode::type() const {
        return NodeType::BINARY;
    }

    VarDeclNode::VarDeclNode(const std::string& name, std::shared_ptr<Node> val, std::size_t line)
        : name(name), val(val) {
        this->line = line;
    }

    NodeType VarDeclNode::type() const {
        return NodeType::VAR_DECL;
    }
    
    ProgramNode::ProgramNode(const std::vector<std::shared_ptr<Node> >& stmts, std::size_t line)
        : stmts(stmts) {
        this->line = line;
    }

    NodeType ProgramNode::type() const {
        return NodeType::PROGRAM;
    }


} // namespace eri