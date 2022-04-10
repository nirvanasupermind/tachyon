#ifndef INTERPRETER_H
#define INTERPRETER_H

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
        Number visit(const std::shared_ptr<Node> &node);
        void raise_error(int line, const std::string &message) const;
        Number visit(Node *node);
        Number visit(NumberNode *node);
        Number visit(AddNode *node);
        Number visit(SubtractNode *node);
        Number visit(MultiplyNode *node);
        Number visit(DivideNode *node);
        Number visit(PlusNode *node);
        Number visit(MinusNode *node);
    };

} // namespace eris

#endif // INTERPRETER_H