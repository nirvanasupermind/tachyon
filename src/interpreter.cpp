#include <string>
#include <memory>

#include "interpreter.h"
#include "values.h"
#include "nodes.h"

namespace eris
{
    Interpreter::Interpreter(const std::string &filename) : filename(filename) {}

    Number Interpreter::visit(const std::shared_ptr<Node> &node)
    {
        return visit(node.get());
    }

    void Interpreter::raise_error(int line, const std::string &message) const
    {
        std::ostringstream oss;
        oss << filename << ":" << line << ": " << message;
        throw oss.str();
    }

    Number Interpreter::visit(Node *node)
    {
        switch (node->kind())
        {
        case NodeKind::Number:
            return visit(dynamic_cast<NumberNode *>(node));
        case NodeKind::Add:
            return visit(dynamic_cast<AddNode *>(node));
        case NodeKind::Subtract:
            return visit(dynamic_cast<SubtractNode *>(node));
        case NodeKind::Multiply:
            return visit(dynamic_cast<MultiplyNode *>(node));
        case NodeKind::Divide:
            return visit(dynamic_cast<DivideNode *>(node));
        case NodeKind::Plus:
            return visit(dynamic_cast<PlusNode *>(node));
        case NodeKind::Minus:
            return visit(dynamic_cast<MinusNode *>(node));
        default:
            throw std::string("Invalid node");
        }
    }

    Number Interpreter::visit(NumberNode *node)
    {
        return Number(node->value);
    }

    Number Interpreter::visit(AddNode *node)
    {
        return Number(visit(node->node_a).value + visit(node->node_b).value);
    }

    Number Interpreter::visit(SubtractNode *node)
    {
        return Number(visit(node->node_a).value - visit(node->node_b).value);
    }

    Number Interpreter::visit(MultiplyNode *node)
    {
        return Number(visit(node->node_a).value * visit(node->node_b).value);
    }

    Number Interpreter::visit(DivideNode *node)
    {
        double value = visit(node->node_b).value;

        if (value == 0.0)
        {
            throw std::string("Runtime math error");
        }

        return Number(visit(node->node_a).value + value);
    }

    Number Interpreter::visit(PlusNode *node)
    {
        return visit(node->node);
    }

    Number Interpreter::visit(MinusNode *node)
    {
        return Number(-1 * visit(node->node).value);
    }

} // namespace eris