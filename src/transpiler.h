#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <string>
#include <sstream>
#include <set>
#include "node.h"

namespace tachyon {
    class Transpiler {
    private:
        static const std::string boilerplate;
        std::string filename{};
        std::string imported_code{};
        std::ostringstream post_main_code{};
        std::set<std::string> included_headers{};
        void visit(Node* node);
        void visit(NilNode* node);
        void visit(NumberNode* node);
        void visit(TrueNode* node);
        void visit(FalseNode* node);
        void visit(CharNode* node);
        void visit(StringNode* node);
        void visit(IdentifierNode* node);
        void visit(ParenExprNode* node);
        void visit(LambdaExprNode* node);
        void visit(ObjectNode* node);
        void visit(VecNode* node);
        void visit(CallExprNode* node);
        void visit(AttrExprNode* node);
        void visit(UnaryExprNode* node);
        void visit(BinaryExprNode* node);
        void visit(ExprStmtNode* node);
        void visit(VarDeclStmtNode* node);
        void visit(BlockStmtNode* node);
        void visit(StmtListNode* node);
        void visit(IfStmtNode* node);
        void visit(IfElseStmtNode* node);
        void visit(WhileStmtNode* node);
        void visit(ForStmtNode* node);
        void visit(FuncDeclStmtNode* node);
        void visit(ReturnStmtNode* node);
        void visit(TryCatchStmtNode* node);
        void visit(CImportStmtNode* node);
    public:
        Transpiler(const std::string& filename);
        std::string generate_code(Node* node);
    };
} // namespace tachyon

#endif // TRANSPILER_H