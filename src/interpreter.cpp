#include <string>
#include <memory>

#include "interpreter.h"
#include "values.h"
#include "nodes.h"

namespace eris
{
    Interpreter::Interpreter(const std::string &filename) : filename(filename) {}

    std::shared_ptr<Value> Interpreter::visit(const std::shared_ptr<Node> &node)
    {
        return visit(node.get());
    }

    void Interpreter::raise_error(int line, const std::string &message) const
    {
        std::ostringstream oss;
        oss << filename << ":" << line << ": " << message;
        throw Exception(oss.str());
    }

    std::shared_ptr<Value> Interpreter::visit(Node *node)
    {
        switch (node->kind())
        {
        case NodeKind::Int:
            return visit(dynamic_cast<IntNode *>(node));
        case NodeKind::Double:
            return visit(dynamic_cast<DoubleNode *>(node));
        case NodeKind::Atom:
            return visit(dynamic_cast<AtomNode *>(node));
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
            throw Exception("Invalid node");
            return std::shared_ptr<Value>();
        }
    }

    std::shared_ptr<Value> Interpreter::visit(IntNode *node)
    {
        return std::shared_ptr<Int>(new Int(node->value));
    }

    std::shared_ptr<Value> Interpreter::visit(DoubleNode *node)
    {
        return std::shared_ptr<Double>(new Double(node->value));
    }

    std::shared_ptr<Value> Interpreter::visit(AtomNode *node)
    {
        return std::shared_ptr<Atom>(new Atom(node->atom));
    }

    std::shared_ptr<Value> Interpreter::visit(AddNode *node)
    {
        std::shared_ptr<Number> a = std::dynamic_pointer_cast<Number>(visit(node->node_a));
        std::shared_ptr<Number> b = std::dynamic_pointer_cast<Number>(visit(node->node_b));

        if (!a)
        {
            raise_error(node->node_a->line, "bad argument #1 to binary operator '+'");
            return std::shared_ptr<Value>();
        }

        if (!b)
        {
            raise_error(node->node_a->line, "bad argument #2 to binary operator '+'");
            return std::shared_ptr<Value>();
        }

        if (a->kind() == ValueKind::IntVal && b->kind() == ValueKind::IntVal)
        {
            return std::shared_ptr<Int>(new Int(a->getval() + b->getval()));
        }

        return std::shared_ptr<Double>(new Double(a->getval() + b->getval()));
    }

    std::shared_ptr<Value> Interpreter::visit(SubtractNode *node)
    {
        std::shared_ptr<Number> a = std::dynamic_pointer_cast<Number>(visit(node->node_a));
        std::shared_ptr<Number> b = std::dynamic_pointer_cast<Number>(visit(node->node_b));

        if (!a)
        {
            raise_error(node->node_a->line, "bad argument #1 to binary operator '-'");
            return std::shared_ptr<Value>();
        }

        if (!b)
        {
            raise_error(node->node_a->line, "bad argument #2 to binary operator '-'");
            return std::shared_ptr<Value>();
        }

        if (a->kind() == ValueKind::IntVal && b->kind() == ValueKind::IntVal)
        {
            return std::shared_ptr<Int>(new Int(a->getval() - b->getval()));
        }

        return std::shared_ptr<Double>(new Double(a->getval() - b->getval()));
    }

    std::shared_ptr<Value> Interpreter::visit(MultiplyNode *node)
    {
        std::shared_ptr<Number> a = std::dynamic_pointer_cast<Number>(visit(node->node_a));
        std::shared_ptr<Number> b = std::dynamic_pointer_cast<Number>(visit(node->node_b));

        if (!a)
        {
            raise_error(node->node_a->line, "bad argument #1 to binary operator '*'");
            return std::shared_ptr<Value>();
        }

        if (!b)
        {
            raise_error(node->node_a->line, "bad argument #2 to binary operator '*'");
            return std::shared_ptr<Value>();
        }

        if (a->kind() == ValueKind::IntVal && b->kind() == ValueKind::IntVal)
        {
            return std::shared_ptr<Int>(new Int(a->getval() * b->getval()));
        }

        return std::shared_ptr<Double>(new Double(a->getval() * b->getval()));
    }

    std::shared_ptr<Value> Interpreter::visit(DivideNode *node)
    {
        std::shared_ptr<Number> a = std::dynamic_pointer_cast<Number>(visit(node->node_a));
        std::shared_ptr<Number> b = std::dynamic_pointer_cast<Number>(visit(node->node_b));

        if (!a)
        {
            raise_error(node->node_a->line, "bad argument #1 to binary operator '/'");
            return std::shared_ptr<Value>();
        }

        if (!b)
        {
            raise_error(node->node_a->line, "bad argument #2 to binary operator '/'");
            return std::shared_ptr<Value>();
        }

        if (b->getval() == 0)
        {
            raise_error(node->node_b->line, "attempt to divide by zero");
        }

        if (a->kind() == ValueKind::IntVal && b->kind() == ValueKind::IntVal)
        {
            return std::shared_ptr<Int>(new Int(a->getval() / b->getval()));
        }

        return std::shared_ptr<Double>(new Double(a->getval() / b->getval()));
    }

    std::shared_ptr<Value> Interpreter::visit(PlusNode *node)
    {
        std::shared_ptr<Number> a = std::dynamic_pointer_cast<Number>(visit(node->node));

        if (!a)
        {
            raise_error(node->node->line, "bad argument #1 to unary operator '+'");
            return std::shared_ptr<Value>();
        }

        if (a->kind() == ValueKind::IntVal)
        {
            return std::shared_ptr<Int>(new Int(-a->getval()));
        }

        return std::shared_ptr<Double>(new Double(-a->getval()));
    }

    std::shared_ptr<Value> Interpreter::visit(MinusNode *node)
    {
        std::shared_ptr<Number> a = std::dynamic_pointer_cast<Number>(visit(node->node));

        if (!a)
        {
            raise_error(node->node->line, "bad argument #1 to unary operator '-'");
            return std::shared_ptr<Value>();
        }

        if (a->kind() == ValueKind::IntVal)
        {
            return std::shared_ptr<Int>(new Int(-a->getval()));
        }

        return std::shared_ptr<Double>(new Double(-a->getval()));
    }

} // namespace eris