#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <vector>

#include "token.h"

namespace eris {
    // AST node type
    enum class NodeType {
        EMPTY,
        NUMBER,
        NULL_,
        TRUE,
        FALSE,
        UNARY,
        BINARY,
        PROGRAM
    };

    // Base class for AST nodes
    class Node {
    public:
        std::size_t line;
        virtual NodeType type() const = 0;
    };

    class EmptyNode : public Node {
    public:
        EmptyNode(std::size_t line);
        NodeType type() const;
    };

    // Numeric literal
    class NumberNode : public Node {
    public:
        double val;
        NumberNode(double val, std::size_t line);
        NodeType type() const;
    };

    // Null literal
    class NullNode : public Node {
    public:
        NullNode(std::size_t line);
        NodeType type() const;
    };

    // True literal
    class TrueNode : public Node {
    public:
        TrueNode(std::size_t line);
        NodeType type() const;
    };

    // False literal
    class FalseNode : public Node {
    public:
        FalseNode(std::size_t line);
        NodeType type() const;
    };

    // Unary operation
    class UnaryNode : public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> operand_node;
        UnaryNode(TokenType op, std::shared_ptr<Node> operand_node, std::size_t line);
        NodeType type() const;
    };

   // Bnary operation
    class BinaryNode : public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        BinaryNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line);
        std::string str() const;
        NodeType type() const;
    };

    // Program or statement list
    class ProgramNode : public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        ProgramNode(const std::vector<std::shared_ptr<Node> >& stmts, std::size_t line);
        std::string str() const;
        NodeType type() const;
    };

} // namespace eris

#endif // NODE_H