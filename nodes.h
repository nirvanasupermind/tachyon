#ifndef NODES_H
#define NODES_H

#include <string>
#include <memory>

namespace eris
{

    enum class NodeKind
    {
        Number,
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

    class NumberNode : public Node
    {
    public:
        int line;
        double value;

        explicit NumberNode(int line, double value);
        NodeKind kind() const;
        std::string str() const;
    };

    class AddNode : public Node
    {
    public:
        int line;
        std::unique_ptr<Node> node_a;
        std::unique_ptr<Node> node_b;

        explicit AddNode(int line, std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class SubtractNode : public Node
    {
    public:
        int line;
        std::unique_ptr<Node> node_a;
        std::unique_ptr<Node> node_b;

        explicit SubtractNode(int line, std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class MultiplyNode : public Node
    {
    public:
        int line;
        std::unique_ptr<Node> node_a;
        std::unique_ptr<Node> node_b;

        explicit MultiplyNode(int line, std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class DivideNode : public Node
    {
    public:
        int line;
        std::unique_ptr<Node> node_a;
        std::unique_ptr<Node> node_b;

        explicit DivideNode(int line, std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
        NodeKind kind() const;
        std::string str() const;
    };

    class PlusNode : public Node
    {
    public:
        int line;
        std::unique_ptr<Node> node;

        explicit PlusNode(int line, std::unique_ptr<Node> node);

        NodeKind kind() const;
        std::string str() const;
    };

    class MinusNode : public Node
    {
    public:
        int line;
        std::unique_ptr<Node> node;

        explicit MinusNode(int line, std::unique_ptr<Node> node);
        NodeKind kind() const;
        std::string str() const;
    };

} // namespace eris

#endif // NODES_H