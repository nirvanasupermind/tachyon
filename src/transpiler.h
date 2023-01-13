#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <string>
#include <vector>
#include <sstream>
#include "node.h"

namespace eris {
    class Transpiler {
    private:
        std::string filename{};
        std::ostringstream include_section{};
        std::ostringstream pre_main_section{};
        std::ostringstream post_main_section{};
        bool include_val_t;
        void visit(Node* node);
        void visit(NumberNode* node);
        void visit(StringNode* node);
        void visit(IdentifierNode* node);
        void visit(NilNode* node);
        void visit(TrueNode* node);
        void visit(FalseNode* node);
        void visit(ParenExprNode* node);
        void visit(CallExprNode* node);
        void visit(MemberExprNode* node);
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
        void visit(FuncDeclStmtNode* node);
        void visit(ReturnStmtNode* node);
        void visit(ProgramNode* node);
    public:
        Transpiler(const std::string& filename);
        std::string generate_code(Node* node);
    };
} // namespace eris

#endif // TRANSPILER_H