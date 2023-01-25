#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <string>
#include <sstream>
#include <set>
#include "node.h"

namespace eris {
    class Transpiler {
    private:
        std::string filename{};
        std::ostringstream post_main_code{};
        std::set<std::string> included_headers{};
        void visit(Node* node);
        void visit(NumberNode* node);
        void visit(ParenExprNode* node);
        void visit(UnaryExprNode* node);
        void visit(BinaryExprNode* node);
        void visit(ExprStmtNode* node);
    public:
        static std::string boilerplate;
        Transpiler(const std::string& filename);
        std::string generate_code(Node* node);
    };
} // namespace eris


#endif // TOKEN_H