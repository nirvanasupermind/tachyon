#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace tachyon {
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

    StringNode::StringNode(const std::string& val, int line)
        : val(val) {
        this->line = line;
    }

    NodeKind StringNode::kind() const {
        return NodeKind::STRING;
    }

    IdentifierNode::IdentifierNode(const std::string& val, int line)
        : val(val) {
        this->line = line;
    }

    NodeKind IdentifierNode::kind() const {
        return NodeKind::IDENTIFIER;
    }

    ParenExprNode::ParenExprNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind ParenExprNode::kind() const {
        return NodeKind::PAREN_EXPR;
    }

    LambdaExprNode::LambdaExprNode(const std::vector<std::string>& args, std::shared_ptr<Node> body, int line)
        : args(args), body(body) {
        this->line = line;
    }

    NodeKind LambdaExprNode::kind() const {
        return NodeKind::LAMBDA_EXPR;
    }

    CallExprNode::CallExprNode(std::shared_ptr<Node> callee, const std::vector<std::shared_ptr<Node> >& args, int line)
        : callee(callee), args(args) {
        this->line = line;
    }

    NodeKind CallExprNode::kind() const {
        return NodeKind::CALL_EXPR;
    }
    
    AttrExprNode::AttrExprNode(const std::shared_ptr<Node> object, const std::string& attr, int line)
        : object(object), attr(attr) {
        this->line = line;
    }
    
    NodeKind AttrExprNode::kind() const {
        return NodeKind::ATTR_EXPR;
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

    ObjectNode::ObjectNode(const std::vector<std::string>& keys, const std::vector<std::shared_ptr<Node> >& vals, int line)
        : keys(keys), vals(vals) {
        this->line = line;
    }

    NodeKind ObjectNode::kind() const {
        return NodeKind::OBJECT;
    }
    
    VecNode::VecNode(const std::vector<std::shared_ptr<Node> >& elems, int line)
        : elems(elems) {
        this->line = line;
    }

    NodeKind VecNode::kind() const {
        return NodeKind::VEC;
    }

    BlockStmtNode::BlockStmtNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind BlockStmtNode::kind() const {
        return NodeKind::BLOCK_STMT;
    }

    IfStmtNode::IfStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, int line)
        : test(test), body(body) {
        this->line = line;
    }

    NodeKind IfStmtNode::kind() const {
        return NodeKind::IF_STMT;
    }

    IfElseStmtNode::IfElseStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::shared_ptr<Node> alternate, int line)
        : test(test), body(body), alternate(alternate) {
        this->line = line;
    }

    NodeKind IfElseStmtNode::kind() const {
        return NodeKind::IF_ELSE_STMT;
    }

    ForStmtNode::ForStmtNode(std::shared_ptr<Node> init, std::shared_ptr<Node> test, std::shared_ptr<Node> update, std::shared_ptr<Node> body, int line)
        : init(init), test(test), update(update), body(body) {
        this->line = line;
    }

    NodeKind ForStmtNode::kind() const {
        return NodeKind::FOR_STMT;
    }

    WhileStmtNode::WhileStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, int line)
        : test(test), body(body) {
        this->line = line;
    }

    NodeKind WhileStmtNode::kind() const {
        return NodeKind::WHILE_STMT;
    }

    FuncDeclStmtNode::FuncDeclStmtNode(const std::string& name, const std::vector<std::string>& args, std::shared_ptr<Node> body, int line)
        : name(name), args(args), body(body) {
        this->line = line;
    }

    NodeKind FuncDeclStmtNode::kind() const {
        return NodeKind::FUNC_DECL_STMT;
    }

    ReturnStmtNode::ReturnStmtNode(std::shared_ptr<Node> node, int line)
        : node(node) {
        this->line = line;
    }

    NodeKind ReturnStmtNode::kind() const {
        return NodeKind::RETURN_STMT;
    }
    
    CImportStmtNode::CImportStmtNode(const std::string& path, int line)
        : path(path) {
        this->line = line;
    }

    NodeKind CImportStmtNode::kind() const {
        return NodeKind::CIMPORT_STMT;
    }

    TryCatchStmtNode::TryCatchStmtNode(std::shared_ptr<Node> try_body, const std::string& ex, std::shared_ptr<Node> catch_body, int line)
        : try_body(try_body), ex(ex), catch_body(catch_body) {
        this->line = line;
    }

    NodeKind TryCatchStmtNode::kind() const {
        return NodeKind::TRY_CATCH_STMT;
    }

    StmtListNode::StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts, int line)
        : stmts(stmts) {
        this->line = line;
    }

    NodeKind StmtListNode::kind() const {
        return NodeKind::STMT_LIST;
    }
} // namespace tachyon