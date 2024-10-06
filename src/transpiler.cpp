#include <string>
#include <memory>
#include <sstream>
#include <iostream>
#include "token.h"
#include "node.h"
#include "transpiler.h"

namespace tachyon {
    Transpiler::Transpiler() {
        this->code = std::ostringstream();
    }

    void Transpiler::visit(const std::shared_ptr<Node>& node) {
        switch (node->get_type()) {
        case NodeType::INT:
            visit_int_node(std::static_pointer_cast<IntNode>(node));
            break;
        case NodeType::FLOAT:
            visit_float_node(std::static_pointer_cast<FloatNode>(node));
            break;
        case NodeType::IDENTIFIER:
            visit_identifier_node(std::static_pointer_cast<IdentifierNode>(node));
            break;
        case NodeType::UNARY_OP:
            visit_unary_op_node(std::static_pointer_cast<UnaryOpNode>(node));
            break;
        case NodeType::BIN_OP:
            visit_bin_op_node(std::static_pointer_cast<BinOpNode>(node));
            break;
        case NodeType::EXPR_STMT:
            visit_expr_stmt_node(std::static_pointer_cast<ExprStmtNode>(node));
            break;
        case NodeType::VAR_DEF_STMT:
            visit_var_def_stmt_node(std::static_pointer_cast<VarDefStmtNode>(node));
            break;
        case NodeType::IF_STMT:
            visit_if_stmt_node(std::static_pointer_cast<IfStmtNode>(node));
            break;
        case NodeType::IF_ELSE_STMT:
            visit_if_else_stmt_node(std::static_pointer_cast<IfElseStmtNode>(node));
            break;
        case NodeType::WHILE_STMT:
            visit_while_stmt_node(std::static_pointer_cast<WhileStmtNode>(node));
            break;
        case NodeType::FOR_STMT:
            visit_for_stmt_node(std::static_pointer_cast<ForStmtNode>(node));
            break;
        case NodeType::BLOCK_STMT:
            visit_block_stmt_node(std::static_pointer_cast<BlockStmtNode>(node));
            break;
        case NodeType::STMT_LIST:
            visit_stmt_list_node(std::static_pointer_cast<StmtListNode>(node));
            break;
        }
    }

    void Transpiler::visit_int_node(const std::shared_ptr<IntNode>& node) {
        int32_t x = std::stol(node->tok.val);
        uint64_t result = 0;
        std::memcpy(&result, &x, sizeof(x));
        result = (result << 2) + 1;

        code << result << "ULL";
    }

    void Transpiler::visit_float_node(const std::shared_ptr<FloatNode>& node) {
        float x = std::stof(node->tok.val);
        uint64_t result = 0;
        std::memcpy(&result, &x, sizeof(x));
        result = (result << 2) + 1;
        code << result << "ULL";
    }

    void Transpiler::visit_bool_node(const std::shared_ptr<BoolNode>& node) {
        if(node->tok.val == "true") {
            code << "11ULL";
        } else {
            code << "3ULL";
        }
    }

    void Transpiler::visit_identifier_node(const std::shared_ptr<IdentifierNode>& node) {
        code << node->tok.val;
    }

    void Transpiler::visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node) {
        code << "tachyon_internal::";
        switch (node->op_tok.type) {
        case TokenType::MINUS:
            code << "minus";
            break;
        case TokenType::NOT:
            code << "not_";
            break;
        }
        code << "(";
        visit(node->right_node);
        code << ")";
    }

    void Transpiler::visit_bin_op_node(const std::shared_ptr<BinOpNode>& node) {
        if(node->op_tok.type == TokenType::EQ) {
        visit(node->left_node);
        code << "=";
        visit(node->right_node);
        } else {
        code << "tachyon_internal::";
        switch (node->op_tok.type) {
        case TokenType::PLUS:
            code << "add";
            break;
        case TokenType::MINUS:
            code << "sub";
            break;
        case TokenType::MUL:
            code << "mul";
            break;
        case TokenType::DIV:
            code << "div";
            break;
        case TokenType::MOD:
            code << "mod";
            break;
        case TokenType::AND:
            code << "and_";
            break;
        case TokenType::OR:
            code << "or_";
            break;
        case TokenType::XOR:
            code << "xor_";
            break;
        case TokenType::LOGICAL_AND:
            code << "logical_and";
            break;
        case TokenType::LOGICAL_OR:
            code << "logical_or";
            break;
        case TokenType::LSH:
            code << "lsh";
            break;
        case TokenType::RSH:
            code << "rsh";
            break;
        case TokenType::LT:
            code << "lt";
            break;
        case TokenType::LE:
            code << "le";
            break;
        case TokenType::GT:
            code << "gt";
            break;
        case TokenType::GE:
            code << "ge";
            break;
        }
        code << "(";
        visit(node->left_node);
        code << ",";
        visit(node->right_node);
        code << ")";
        }
    }

    void Transpiler::visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node) {
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_var_def_stmt_node(const std::shared_ptr<VarDefStmtNode>& node) {
        code << "uint64_t " << node->name_tok.val << " = ";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_if_stmt_node(const std::shared_ptr<IfStmtNode>& node) {
        code << "if((";
        visit(node->cond);
        code << ") != 3ULL)";
        visit(node->body);
    }
    
    void Transpiler::visit_if_else_stmt_node(const std::shared_ptr<IfElseStmtNode>& node) {
        code << "if((";
        visit(node->cond);
        code << ")!=3ULL)";
        visit(node->if_body);
        code << "else";
        visit(node->else_body);
    }
    
    void Transpiler::visit_while_stmt_node(const std::shared_ptr<WhileStmtNode>& node) {
        code << "while((";
        visit(node->cond);
        code << ")!=11ULL)";
        visit(node->body);
    }
        
    void Transpiler::visit_for_stmt_node(const std::shared_ptr<ForStmtNode>& node) {
        code << "for(";
        visit(node->init);
        code << "(";
        visit(node->cond);
        code << ")!=3;";
        visit(node->update);
        code << ")";
        visit(node->body);
    }

    void Transpiler::visit_block_stmt_node(const std::shared_ptr<BlockStmtNode>& node) {
        code << "{\n";
        visit(node->stmt_list_node);
        code << "\n}";
    }
    
    void Transpiler::visit_stmt_list_node(const std::shared_ptr<StmtListNode>& node) {
        for(int i = 0; i < node->stmts.size(); i++) {
            visit(node->stmts.at(i));
            code << '\n';
        }
    }

}