#include <string>

#include "node.h"
#include "env.h"
#include "analyzer.h"

namespace eris {
    Analyzer::Analyzer(const std::string& filename)
        : global_env(std::shared_ptr<Env>(new Env())), filename(filename) {
    }

    void Analyzer::visit(std::shared_ptr<Env> env, Node* node) {
        switch (node->type()) {
        case NodeType::IDENTIFIER:
            visit(env, dynamic_cast<IdentifierNode*>(node));
            break;
        case NodeType::PAREN_EXPR:
            visit(env, dynamic_cast<ParenExprNode*>(node));
            break;
        case NodeType::UNARY_EXPR:
            visit(env, dynamic_cast<UnaryExprNode*>(node));
            break;
        case NodeType::BINARY_EXPR:
            visit(env, dynamic_cast<BinaryExprNode*>(node));
            break;
        case NodeType::ASSIGNMENT_EXPR:
            visit(env, dynamic_cast<AssignmentExprNode*>(node));
            break;
        case NodeType::EXPR_STMT:
            visit(env, dynamic_cast<ExprStmtNode*>(node));
            break;
        case NodeType::VAR_DECL_STMT:
            visit(env, dynamic_cast<VarDeclStmtNode*>(node));
            break;
        case NodeType::IF_STMT:
            visit(env, dynamic_cast<IfStmtNode*>(node));
            break;
        case NodeType::IF_ELSE_STMT:
            visit(env, dynamic_cast<IfElseStmtNode*>(node));
            break;
        case NodeType::WHILE_STMT:
            visit(env, dynamic_cast<WhileStmtNode*>(node));
            break;
        case NodeType::FOR_STMT:
            visit(env, dynamic_cast<ForStmtNode*>(node));
           break;
        case NodeType::FUNC_DECL_STMT:
            visit(env, dynamic_cast<FuncDeclStmtNode*>(node));
            break;
        case NodeType::PROGRAM:
            visit(env, dynamic_cast<ProgramNode*>(node));
            break;
        default:
            break;
        }
    }

    void Analyzer::visit(std::shared_ptr<Env> env, IdentifierNode* node) {
        env->check_name(node->val, filename, node->line); 
    }

    void Analyzer::visit(std::shared_ptr<Env> env, ParenExprNode* node) {
        visit(env, node->expr_node.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, UnaryExprNode* node) {
        visit(env, node->operand_node.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, BinaryExprNode* node) {
        visit(env, node->node_a.get());
        visit(env, node->node_b.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, AssignmentExprNode* node) {
        visit(env, node->node_a.get());
        visit(env, node->node_b.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, ExprStmtNode* node) {
        visit(env, node->expr_node.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, VarDeclStmtNode* node) {
        visit(env, node->val.get());
        env->add_name(node->name);
    }

    void Analyzer::visit(std::shared_ptr<Env> env, BlockStmtNode* node) {
        std::shared_ptr<Env> block_env(new Env(env));
        visit(block_env, node->program_node.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, IfStmtNode* node) {
        visit(env, node->test.get());
        visit(env, node->body.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, IfElseStmtNode* node) {
        visit(env, node->test.get());
        visit(env, node->body.get());
        visit(env, node->alternate.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, WhileStmtNode* node) {
        visit(env, node->test.get());
        visit(env, node->body.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, ForStmtNode* node) {
        visit(env, node->decl.get());
        visit(env, node->test.get());
        visit(env, node->assignment.get());
        visit(env, node->body.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, FuncDeclStmtNode* node) {
        std::shared_ptr<Env> func_env(new Env(env));
        for(const std::string& name : node->params) {
            func_env->add_name(name);
        }
        visit(func_env, node->body.get());
    }

    void Analyzer::visit(std::shared_ptr<Env> env, ProgramNode* node) {
        for (std::shared_ptr<Node> stmt : node->stmts) {
            visit(env, stmt.get());
        }
    }

    void Analyzer::analyze(Node* node) {
        visit(global_env, node);
    }

} // namespace eris