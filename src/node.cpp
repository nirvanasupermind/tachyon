#include <string>
#include <vector>

#include "token.h"
#include "node.h"

namespace eris {
    EmptyNode::EmptyNode(std::size_t line) {
        this->line = line;
    }

    NodeType EmptyNode::type() const {
        return NodeType::EMPTY;
    }

    NumberNode::NumberNode(double val, std::size_t line)
        : val(val) {
        this->line = line;
    }

    NodeType NumberNode::type() const {
        return NodeType::NUMBER;
    }

    StringNode::StringNode(const std::string& val, std::size_t line)
        : val(val) {
        this->line = line;
    }

    NodeType StringNode::type() const {
        return NodeType::STRING;
    }

    IdentifierNode::IdentifierNode(const std::string& val, std::size_t line)
        : val(val) {
        this->line = line;
    }

    NodeType IdentifierNode::type() const {
        return NodeType::IDENTIFIER;
    }

    NilNode::NilNode(std::size_t line) {
        this->line = line;
    }

    NodeType NilNode::type() const {
        return NodeType::NIL;
    }

    TrueNode::TrueNode(std::size_t line) {
        this->line = line;
    }

    NodeType TrueNode::type() const {
        return NodeType::TRUE;
    }

    FalseNode::FalseNode(std::size_t line) {
        this->line = line;
    }

    NodeType FalseNode::type() const {
        return NodeType::FALSE;
    }

    CallExprNode::CallExprNode(std::shared_ptr<Node> callee, const std::vector<std::shared_ptr<Node> >& params, std::size_t line)
        : callee(callee), params(params) {
        this->line = line;
    }

    NodeType CallExprNode::type() const {
        return NodeType::CALL_EXPR;
    }

    MemberExprNode::MemberExprNode(TokenType op, std::shared_ptr<Node> object, const std::string& member, std::size_t line)
        : op(op), object(object), member(member) {
        this->line = line;
    }

    NodeType MemberExprNode::type() const {
        return NodeType::MEMBER_EXPR;
    }

    UnaryExprNode::UnaryExprNode(TokenType op, std::shared_ptr<Node> operand_node, std::size_t line)
        : op(op), operand_node(operand_node) {
        this->line = line;
    }

    NodeType UnaryExprNode::type() const {
        return NodeType::UNARY_EXPR;
    }

    BinaryExprNode::BinaryExprNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line)
        : op(op), node_a(node_a), node_b(node_b) {
        this->line = line;
    }

    NodeType BinaryExprNode::type() const {
        return NodeType::BINARY_EXPR;
    }

    AssignmentExprNode::AssignmentExprNode(TokenType op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, std::size_t line)
        : op(op), node_a(node_a), node_b(node_b) {
        this->line = line;
    }

    NodeType AssignmentExprNode::type() const {
        return NodeType::ASSIGNMENT_EXPR;
    }

    ExprStmtNode::ExprStmtNode(std::shared_ptr<Node> expr_node, std::size_t line)
        : expr_node(expr_node) {
        this->line = line;
    }

    NodeType ExprStmtNode::type() const {
        return NodeType::EXPR_STMT;
    }

    VarDeclStmtNode::VarDeclStmtNode(const std::string& name, std::shared_ptr<Node> val, std::size_t line)
        : name(name), val(val) {
        this->line = line;
    }

    NodeType VarDeclStmtNode::type() const {
        return NodeType::VAR_DECL_STMT;
    }

    BlockStmtNode::BlockStmtNode(std::shared_ptr<Node> program_node, std::size_t line)
        : program_node(program_node) {
        this->line = line;
    }

    NodeType BlockStmtNode::type() const {
        return NodeType::BLOCK_STMT;
    }

    IfStmtNode::IfStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::size_t line)
        : test(test), body(body) {
        this->line = line;
    }

    NodeType IfStmtNode::type() const {
        return NodeType::IF_STMT;
    }

    IfElseStmtNode::IfElseStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::shared_ptr<Node> alternate, std::size_t line)
        : test(test), body(body), alternate(alternate) {
        this->line = line;
    }

    NodeType IfElseStmtNode::type() const {
        return NodeType::IF_ELSE_STMT;
    }

    WhileStmtNode::WhileStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::size_t line)
        : test(test), body(body) {
        this->line = line;
    }

    NodeType WhileStmtNode::type() const {
        return NodeType::WHILE_STMT;
    }

    ForStmtNode::ForStmtNode(std::shared_ptr<Node> decl, std::shared_ptr<Node> test,
        std::shared_ptr<Node> assignment, std::shared_ptr<Node> body, std::size_t line)
        : decl(decl), test(test), assignment(assignment), body(body) {
        this->line = line;
    }

    NodeType ForStmtNode::type() const {
        return NodeType::FOR_STMT;
    }

    FuncDeclStmtNode::FuncDeclStmtNode(const std::string& name, const std::vector<std::string>& params,
        std::shared_ptr<Node> body, std::size_t line)
        : name(name), params(params), body(body) {
        this->line = line;
    }

    NodeType FuncDeclStmtNode::type() const {
        return NodeType::FUNC_DECL_STMT;
    }

    ReturnStmtNode::ReturnStmtNode(std::shared_ptr<Node> expr_node, std::size_t line)
        : expr_node(expr_node) {
        this->line = line;
    }

    NodeType ReturnStmtNode::type() const {
        return NodeType::RETURN_STMT;
    }

    ProgramNode::ProgramNode(const std::vector<std::shared_ptr<Node> >& stmts, std::size_t line)
        : stmts(stmts) {
        this->line = line;
    }

    NodeType ProgramNode::type() const {
        return NodeType::PROGRAM;
    }

} // namespace eris