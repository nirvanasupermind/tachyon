#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace eris {
    NilNode::NilNode(int line) {
        this->line = line;
    }

    NodeKind NilNode::kind() const {
        return NodeKind::NIL;
    }

    NumberNode::NumberNode(double val, int line)
        : val(val) {
        this->line = line;
    }

    NodeKind NumberNode::kind() const {
        return NodeKind::NUMBER;
    }

    TrueNode::TrueNode(int line) {
        this->line = line;
    }

    NodeKind TrueNode::kind() const {
        return NodeKind::TRUE;
    }

    FalseNode::FalseNode(int line) {
        this->line = line;
    }

    NodeKind FalseNode::kind() const {
        return NodeKind::FALSE;
    }

    CharNode::CharNode(char val, int line)
        : val(val) {
        this->line = line;
    }

    NodeKind CharNode::kind() const {
        return NodeKind::CHAR;
    }

    ParenExprNode::ParenExprNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind ParenExprNode::kind() const {
        return NodeKind::PAREN_EXPR;
    }

    UnaryExprNode::UnaryExprNode(Token op, std::shared_ptr<Node> node, int line)
        : op(op), node(node) {
        this->line = line;
    }

    NodeKind UnaryExprNode::kind() const {
        return NodeKind::UNARY_EXPR;
    }

    BinaryExprNode::BinaryExprNode(Token op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, int line)
        : op(op), node_a(node_a), node_b(node_b) {
        this->line = line;
    }

    NodeKind BinaryExprNode::kind() const {
        return NodeKind::BINARY_EXPR;
    }  

    ExprStmtNode::ExprStmtNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind ExprStmtNode::kind() const {
        return NodeKind::EXPR_STMT;
    }   

    VarDeclStmtNode::VarDeclStmtNode(const std::string& name, std::shared_ptr<Node> val, int line)
        : name(name), val(val) {
        this->line = line;
    }
    
    NodeKind VarDeclStmtNode::kind() const {
        return NodeKind::VAR_DECL_STMT;
    }

    StmtListNode::StmtListNode(std::vector<std::shared_ptr<Node> > stmts, int line)
        : stmts(stmts) {
        this->line = line;
    }

    NodeKind StmtListNode::kind() const {
        return NodeKind::STMT_LIST;
    }
     
} // namespace eris