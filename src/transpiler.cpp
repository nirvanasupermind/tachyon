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
        case NodeType::NUMBER:
            visit_number_node(std::static_pointer_cast<NumberNode>(node));
            break;
        case NodeType::STRING:
            visit_string_node(std::static_pointer_cast<StringNode>(node));
            break;
        case NodeType::VECTOR:
            visit_vector_node(std::static_pointer_cast<VectorNode>(node));
            break;
        case NodeType::IDENTIFIER:
            visit_identifier_node(std::static_pointer_cast<IdentifierNode>(node));
            break;
        case NodeType::CALL_EXPR:
            visit_call_expr_node(std::static_pointer_cast<CallExprNode>(node));
            break;
        case NodeType::UNARY_OP:
            visit_unary_op_node(std::static_pointer_cast<UnaryOpNode>(node));
            break;
        case NodeType::BIN_OP:
            visit_bin_op_node(std::static_pointer_cast<BinOpNode>(node));
            break;
        case NodeType::VAR_DEF_STMT:
            visit_var_def_stmt_node(std::static_pointer_cast<VarDefStmtNode>(node));
            break;
        case NodeType::EXPR_STMT:
            visit_expr_stmt_node(std::static_pointer_cast<ExprStmtNode>(node));
            break;
        case NodeType::BLOCK_STMT:
            visit_block_stmt_node(std::static_pointer_cast<BlockStmtNode>(node));
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
        case NodeType::STMT_LIST:
            visit_stmt_list_node(std::static_pointer_cast<StmtListNode>(node));
            break;
        default:
            std::cout << "UNKNOWN NODE: " << node->to_string() << '\n';
            break;
        }
    }

    void Transpiler::visit_number_node(const std::shared_ptr<NumberNode>& node) {
        float x = std::stof(node->tok.val);
        uint64_t y = (((*(uint64_t*)(&x)) & 0xffffffff) << 1) + 1;
        code << y << "ULL";
    }

    void Transpiler::visit_string_node(const std::shared_ptr<StringNode>& node) {
        code << "pack_object(std::make_shared<Object>(Object(\"" << node->tok.val << "\", String)))";
    }

    void Transpiler::visit_vector_node(const std::shared_ptr<VectorNode>& node) {
        code << "pack_object(std::make_shared<Object>(Object({";
        if(node->elements.size() == 0) {
                code << "}, Vector)))";
        } else {

        for(int i = 0; i < node->elements.size(); i++) {
            visit(node->elements.at(i));
            if(i == node->elements.size() - 1) {
                code << "}, Vector)))";
            } else {
                code << ",";
            }
        }
        }
    }


    void Transpiler::visit_identifier_node(const std::shared_ptr<IdentifierNode>& node) {
        code << node->tok.val;
    }

    void Transpiler::visit_call_expr_node(const std::shared_ptr<CallExprNode>& node) {
        code << "unpack_object(";
        visit(node->callee);
        code << ")->func({";
        if(node->args.size() == 0) {
                code << "})";
        } else {

        for(int i = 0; i < node->args.size(); i++) {
            visit(node->args.at(i));
            if(i == node->args.size() - 1) {
                code << "})";
            } else {
                code << ",";
            }
        }
        }
    }

    void Transpiler::visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node) {
        code << "pack_number(";
        code << node->op_tok.val;
        code << "unpack_number(";
        visit(node->right_node);
        code << "))";
    }

    void Transpiler::visit_bin_op_node(const std::shared_ptr<BinOpNode>& node) {
        if (node->op_tok.type == TokenType::EQ) {
            visit(node->left_node);
            code << node->op_tok.val;
            visit(node->right_node);
        }
        else {
            code << "pack_number(unpack_number(";
            visit(node->left_node);
            code << ")" << node->op_tok.val << "unpack_number(";
            visit(node->right_node);
            code << "))";
        }
    }

    void Transpiler::visit_var_def_stmt_node(const std::shared_ptr<VarDefStmtNode>& node) {
        code << "uint64_t " << node->name_tok.val << "=";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node) {
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_block_stmt_node(const std::shared_ptr<BlockStmtNode>& node) {
        code << "{\n";
        visit(node->stmt_list_node);
        code << "}";
    }

    void Transpiler::visit_if_stmt_node(const std::shared_ptr<IfStmtNode>& node) {
        code << "if((";
        visit(node->cond);
        code << ") != 1ULL)";
        visit(node->body);
    }

    void Transpiler::visit_if_else_stmt_node(const std::shared_ptr<IfElseStmtNode>& node) {
        code << "if((";
        visit(node->cond);
        code << ") != 1ULL)";
        visit(node->if_body);
        code << "else";
        visit(node->else_body);
    }

    void Transpiler::visit_while_stmt_node(const std::shared_ptr<WhileStmtNode>& node) {
        code << "while((";
        visit(node->cond);
        code << ") != 1ULL)";
        visit(node->body);
    }

    void Transpiler::visit_for_stmt_node(const std::shared_ptr<ForStmtNode>& node) {
        code << "for(";
        visit(node->init);
        code << "(";
        visit(node->cond);
        code << ") != 1ULL;";
        visit(node->update);
        code << ")";
        visit(node->body);
    }

    void Transpiler::visit_stmt_list_node(const std::shared_ptr<StmtListNode>& node) {
        for (int i = 0; i < node->stmts.size(); i++) {
            visit(node->stmts.at(i));
            code << '\n';
        }
    }
}