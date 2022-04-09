#include <string>
#include <memory>
#include <sstream>

#include "nodes.h"

namespace eris {

NumberNode::NumberNode(double value)
    : value(value)
{}

NodeKind NumberNode::kind() const {
    return NodeKind::Number;
}

std::string NumberNode::str() const {
    return std::to_string(value);
}

AddNode::AddNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b)
    : node_a(std::move(node_a)),
    node_b(std::move(node_b))
{}

NodeKind AddNode::kind() const {
    return NodeKind::Add;
}

std::string AddNode::str() const {
    std::ostringstream oss;

    oss << "(" << node_a->str() << "+" << node_b->str() << ")";

    return oss.str();
}

SubtractNode::SubtractNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b)
    : node_a(std::move(node_a)),
    node_b(std::move(node_b))
{}

NodeKind SubtractNode::kind() const {
    return NodeKind::Subtract;
}

std::string SubtractNode::str() const {
    std::ostringstream oss;

    oss << "(" << node_a->str() << "-" << node_b->str() << ")";

    return oss.str();
}

MultiplyNode::MultiplyNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b)
    : node_a(std::move(node_a)),
    node_b(std::move(node_b))
{}

NodeKind MultiplyNode::kind() const {
    return NodeKind::Multiply;
}

std::string MultiplyNode::str() const {
    std::ostringstream oss;

    oss << "(" << node_a->str() << "*" << node_b->str() << ")";

    return oss.str();
}

DivideNode::DivideNode(std::unique_ptr<Node> node_a, std::unique_ptr<Node> node_b)
    : node_a(std::move(node_a)),
    node_b(std::move(node_b))
{}

NodeKind DivideNode::kind() const {
    return NodeKind::Divide;
}

std::string DivideNode::str() const {
    std::ostringstream oss;

    oss << "(" << node_a->str() << "/" << node_b->str() << ")";

    return oss.str();
}

PlusNode::PlusNode(std::unique_ptr<Node> node)
    : node(std::move(node))
{}

NodeKind PlusNode::kind() const {
    return NodeKind::Plus;
}

std::string PlusNode::str() const {
    std::ostringstream oss;

    oss << "(+" << node->str() << ")";

    return oss.str();
}

MinusNode::MinusNode(std::unique_ptr<Node> node)
    : node(std::move(node))
{}

NodeKind MinusNode::kind() const {
    return NodeKind::Minus;
}

std::string MinusNode::str() const {
    std::ostringstream oss;

    oss << "(-" << node->str() << ")";

    return oss.str();
}

} // namespace eris