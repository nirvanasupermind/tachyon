#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <string>
#include <vector>
#include "node.h"

namespace eris {
    class Transpiler {
    private:
        std::string filename;
        std::vector<std::string> body{};
        void visit(Node* node);
        void visit(NumberNode* node);
        void visit(IdentifierNode* node);
        void visit(NilNode* node);
        void visit(TrueNode* node);
        void visit(FalseNode* node);
        void visit(UnaryExprNode* node);
        void visit(BinaryExprNode* node);
        void visit(AssignmentExprNode* node);
        void visit(ExprStmtNode* node);
        void visit(VarDeclStmtNode* node);
        void visit(BlockStmtNode* node);
        void visit(IfStmtNode* node);
        void visit(IfElseStmtNode* node);
        void visit(WhileStmtNode* node);
        void visit(ForStmtNode* node);
        void visit(ProgramNode* node);
        std::string postprocess(std::string& code);
    public:
        static const std::string boilerplate;
        Transpiler(const std::string& filename);
        std::string generate_code(Node* node);
    };
} // namespace eris

#endif // TRANSPILER_H