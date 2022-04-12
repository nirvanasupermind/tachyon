#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <memory>

#include "values.h"
#include "nodes.h"

namespace eris
{
    class Interpreter
    {
    private:
        std::string filename;
    public:
        Interpreter(const std::string& filename);
        std::shared_ptr<Value> visit(const std::shared_ptr<Node> &node);
        void raise_error(int line, const std::string &message) const;
        std::shared_ptr<Value> visit(Node *node);
        std::shared_ptr<Value> visit(IntNode *node);
        std::shared_ptr<Value> visit(DoubleNode *node);
        std::shared_ptr<Value> visit(SymbolNode *node);
        std::shared_ptr<Value> visit(AddNode *node);
        std::shared_ptr<Value> visit(SubtractNode *node);
        std::shared_ptr<Value> visit(MultiplyNode *node);
        std::shared_ptr<Value> visit(DivideNode *node);
        std::shared_ptr<Value> visit(PlusNode *node);
        std::shared_ptr<Value> visit(MinusNode *node);
    };

} // namespace eris

#endif // INTERPRETER_H