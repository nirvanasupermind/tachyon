#ifndef NODES_H
#define NODES_H

#include <string>
#include <memory>

namespace volcano {

enum class NodeKind {
    Number,
    Add,
    Subtract,
    Multiply,
    Divide,
    Plus,
    Minus
};

class Node {
public:
    virtual NodeKind kind() const = 0;
    virtual std::string str() const = 0;
    virtual ~Node() = default;
};

class NumberNode : public Node {
public:
    double value;

    explicit NumberNode(double value);
    NodeKind kind() const;
    std::string str() const;
};

class AddNode : public Node {
public:
    std::unique_ptr<Node> node_a;
    std::unique_ptr<Node> node_b;
    
    explicit AddNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
    NodeKind kind() const;
    std::string str() const;
};

class SubtractNode : public Node {
public:
    std::unique_ptr<Node> node_a;
    std::unique_ptr<Node> node_b;
    
    explicit SubtractNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
    NodeKind kind() const;
    std::string str() const;
};

class MultiplyNode : public Node {
public:
    std::unique_ptr<Node> node_a;
    std::unique_ptr<Node> node_b;
    
    explicit MultiplyNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
    NodeKind kind() const;
    std::string str() const;
};

class DivideNode : public Node {
public:
    std::unique_ptr<Node> node_a;
    std::unique_ptr<Node> node_b;
    
    explicit DivideNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b);
    NodeKind kind() const;
    std::string str() const;
};

class PlusNode : public Node {
public:
    std::unique_ptr<Node> node;
    
    explicit PlusNode(std::unique_ptr<Node> node);
    
    NodeKind kind() const;
    std::string str() const;
};

class MinusNode : public Node {
public:
    std::unique_ptr<Node> node;
    
    explicit MinusNode(std::unique_ptr<Node> node);
    NodeKind kind() const;
    std::string str() const;
};

} // namespace volcano


#endif // NODES_H