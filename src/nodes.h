#ifndef NODES_H
#define NODES_H

#include <string>
#include <memory>
#include <cstdint>

namespace eris
{
    enum class NodeKind
    {
        Int,
        Double,
        Symbol,
        Identifier,
        Add,
        Subtract,
        Multiply,
        Divide,
        Plus,
        Minus
    };

    class Node
    {
    public:
        int line;

        virtual NodeKind kind() const = 0;
        virtual std::string str() const = 0;
        virtual ~Node() = default;
    };

    class IntNode : public Node
    {
    public:
        std::int32_t value;

        explicit IntNode(int line, std::int32_t value);
        NodeKind kind() const;
        std::string str() const;
    };

    class DoubleNode : public Node
    {
    public:
        double value;

        explicit DoubleNode(int line, double value);
        NodeKind kind() const;
        std::string str() const;
    };

    class IdentifierNode : public Node
    {
    public:
        std::string identifier;

        explicit IdentifierNode(int line, const std::string &identifier);
        NodeKind kind() const;
        std::string str() const;
    };

    class SymbolNode : public Node
    {
    public:
        std::string symbol;

        explicit SymbolNode(int line, const std::string &symbol);
        NodeKind kind() const;
        std::string str() const;
    };

    class AddNode : public Node
    {
    public:
        int line;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;

        explicit AddNode(int line, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class SubtractNode : public Node
    {
    public:
        int line;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;

        explicit SubtractNode(int line, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class MultiplyNode : public Node
    {
    public:
        int line;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;

        explicit MultiplyNode(int line, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class DivideNode : public Node
    {
    public:
        int line;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;

        explicit DivideNode(int line, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class PlusNode : public Node
    {
    public:
        int line;
        std::shared_ptr<Node> node;

        explicit PlusNode(int line, std::shared_ptr<Node> node);

        NodeKind kind() const;
        std::string str() const;
    };

    class MinusNode : public Node
    {
    public:
        int line;
        std::shared_ptr<Node> node;

        explicit MinusNode(int line, std::shared_ptr<Node> node);
        NodeKind kind() const;
        std::string str() const;
    };

} // namespace eris

#endif // NODES_H