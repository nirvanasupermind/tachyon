#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace tachyon {
    NumberNode::NumberNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType NumberNode::get_type() const {
        return NodeType::NUMBER;
    }

    std::string NumberNode::to_string() const {
        return "(NumberNode " + tok.to_string() + ")";
    }

    IdentifierNode::IdentifierNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType IdentifierNode::get_type() const {
        return NodeType::IDENTIFIER;
    }

    std::string IdentifierNode::to_string() const {
        return "(IdentifierNode " + tok.to_string() + ")";
    }

    UnaryOpNode::UnaryOpNode(const Token& op_tok, const std::shared_ptr<Node>& right_node) {
        this->op_tok = op_tok;
        this->right_node = right_node;
    }

    NodeType UnaryOpNode::get_type() const {
        return NodeType::UNARY_OP;
    }

    std::string UnaryOpNode::to_string() const {
        return "(UnaryOpNode " + this->op_tok.to_string() + " " + this->right_node->to_string() + ")";
    }

    BinOpNode::BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node) {
        this->left_node = left_node;
        this->op_tok = op_tok;
        this->right_node = right_node;
    }

    NodeType BinOpNode::get_type() const {
        return NodeType::BIN_OP;
    }

    std::string BinOpNode::to_string() const {
        return "(BinOpNode " + this->op_tok.to_string() + " " + this->left_node->to_string() + " " + this->right_node->to_string() + ")";
    }

    VarDefNode::VarDefNode(const Token& name_tok, const std::shared_ptr<Node>& val) {
        this->name_tok = name_tok;
        this->val = val;
    }

    NodeType VarDefNode::get_type() const {
        return NodeType::VAR_DEF;
    }

    std::string VarDefNode::to_string() const {
        return "(VarDefNode " + name_tok.to_string() + " " + val->to_string() + ")";
    }

    // VarAssignNode::VarAssignNode(const Token& name_tok, const std::shared_ptr<Node>& val) {
    //     this->name_tok = name_tok;
    //     this->val = val;
    // }

    // NodeType VarAssignNode::get_type() const {
    //     return NodeType::VAR_ASSIGN;
    // }

    // std::string VarAssignNode::to_string() const {
    //     return "(VarAssignNode " + name_tok.to_string() + " " + val->to_string() + ")";
    // }


    ExprStmtNode::ExprStmtNode(const std::shared_ptr<Node>& expr_node) {
        this->expr_node = expr_node;
    }

    NodeType ExprStmtNode::get_type() const {
        return NodeType::EXPR_STMT;
    }

    std::string ExprStmtNode::to_string() const {
        return "(ExprStmtNode " + expr_node->to_string() + ")";
    }

    BlockStmtNode::BlockStmtNode(const std::shared_ptr<Node>& stmt_list_node) {
        this->stmt_list_node = stmt_list_node;
    }

    NodeType BlockStmtNode::get_type() const {
        return NodeType::BLOCK_STMT;
    }

    std::string BlockStmtNode::to_string() const {
        return "(BlockStmtNode " + stmt_list_node->to_string() + ")";
    }

    StmtListNode::StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts) {
        this->stmts = stmts;
    }

    NodeType StmtListNode::get_type() const {
        return NodeType::STMT_LIST;
    }

    std::string StmtListNode::to_string() const {
        std::string result = "(StmtListNode ";
        for(int i = 0; i < stmts.size(); i++) {
            result += stmts.at(i)->to_string() + " ";
        }
        result = result.substr(0, result.size() - 1);
        result += ")";
        return result;
    }

};

