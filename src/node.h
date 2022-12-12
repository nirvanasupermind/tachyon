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
        NULL_,
        TRUE,
        FALSE,
        UNARY,
        BINARY,
        VAR_DECL,
        VAR_ASSIGN,
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

    class NullNode: public Node {
    public:
        NullNode(std::size_t line);
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

    class UnaryNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> operand_node;
        UnaryNode(TokenType op, std::shared_ptr<Node> operand_node, std::size_t line);
        NodeType type() const;
    };

    class BinaryNode: public Node {
    public:
        TokenType op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        BinaryNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line);
        NodeType type() const;
    };

    class VarDeclNode: public Node {
    public:
        std::string name;
        std::shared_ptr<Node> val;
        VarDeclNode(const std::string& name, std::shared_ptr<Node> val, std::size_t line);
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