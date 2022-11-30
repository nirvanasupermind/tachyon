#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

#include "value.h"
#include "node.h"

namespace eris {
    class Interpreter {
    private:
        std::string filename{};
        void expect_type(size_t line, const ErisValue &val, ErisValueType type) const;
    public:
        Interpreter(const std::string &filename);
        ErisValue visit(const Node& node);
        ErisValue visit_number_node(const Node& node);
        ErisValue visit_unary_node(const Node& node);
        ErisValue visit_binary_node(const Node& node);
        ErisValue visit_expr_statement_node(const Node& node);
        ErisValue visit_program_node(const Node& node);
    };
} // namespace eris

#endif // INTERPRETER_H