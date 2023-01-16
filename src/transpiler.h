#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include "node.h"
#include "env.h"

namespace eris {
    class Transpiler {
    private:
        std::string filename{};
        std::ostringstream include_section{};
        std::ostringstream pre_main_section{};
        std::ostringstream post_main_section{};
        bool include_val_t;
        bool include_print;
        std::shared_ptr<Env> global_env;
        std::set<std::string> obj_props;
        void visit(std::shared_ptr<Env> env, Node* node);
        void visit(std::shared_ptr<Env> env, NumberNode* node);
        void visit(std::shared_ptr<Env> env, StringNode* node);
        void visit(std::shared_ptr<Env> env, IdentifierNode* node);
        void visit(std::shared_ptr<Env> env, NilNode* node);
        void visit(std::shared_ptr<Env> env, TrueNode* node);
        void visit(std::shared_ptr<Env> env, FalseNode* node);
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
        std::string escape(const std::string& your_string) const;
    public:
        Transpiler(const std::string& filename);
        std::string generate_code(Node* node);
    };
} // namespace eris

#endif // TRANSPILER_H