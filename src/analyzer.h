#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

#include "node.h"
#include "env.h"

namespace eris {
    class Analyzer {
    private:
        std::string filename{};
        std::shared_ptr<Env> global_env;
        void visit(std::shared_ptr<Env> env, Node* node);
        void visit(std::shared_ptr<Env> env, IdentifierNode* node);
        void visit(std::shared_ptr<Env> env, ParenExprNode* node);
        void visit(std::shared_ptr<Env> env, CallExprNode* node);
        void visit(std::shared_ptr<Env> env, MemberExprNode* node);
        void visit(std::shared_ptr<Env> env, UnaryExprNode* node);
        void visit(std::shared_ptr<Env> env, BinaryExprNode* node);
        void visit(std::shared_ptr<Env> env, AssignmentExprNode* node);
        void visit(std::shared_ptr<Env> env, ExprStmtNode* node);
        void visit(std::shared_ptr<Env> env, VarDeclStmtNode* node);
        void visit(std::shared_ptr<Env> env, BlockStmtNode* node);
        void visit(std::shared_ptr<Env> env, IfStmtNode* node);
        void visit(std::shared_ptr<Env> env, IfElseStmtNode* node);
        void visit(std::shared_ptr<Env> env, WhileStmtNode* node);
        void visit(std::shared_ptr<Env> env, ForStmtNode* node);
        void visit(std::shared_ptr<Env> env, FuncDeclStmtNode* node);
        void visit(std::shared_ptr<Env> env, ReturnStmtNode* node);
        void visit(std::shared_ptr<Env> env, ProgramNode* node);
    public:
        Analyzer(const std::string& filename);
        void analyze(Node* node);
    };
} // namespace eris

#endif // TRANSPILER_H