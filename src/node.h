#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <vector>
#include "token.h"

namespace tachyon {
    enum class NodeType {
        NUMBER,
        IDENTIFIER,
        UNARY_OP,
        BIN_OP,
        VAR_DEF,
        EXPR_STMT,
        BLOCK_STMT,
        IF_STMT,
        IF_ELSE_STMT,
        WHILE_STMT,
        FOR_STMT,
        RETURN_STMT,
        FUNC_DEF_STMT,
        STMT_LIST
    };

    class Node {
    public:
        virtual NodeType get_type() const = 0;
        virtual std::string to_string() const = 0;
    };

    class NumberNode: public Node {
    public:
        Token tok;
        NumberNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
    };
  
    class IdentifierNode: public Node {
    public:
        Token tok;
        IdentifierNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class UnaryOpNode: public Node {
    public:
        Token op_tok;
        std::shared_ptr<Node> right_node;
        UnaryOpNode(const Token& op_tok, const std::shared_ptr<Node>& right_node);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class BinOpNode: public Node {
    public:
        std::shared_ptr<Node> left_node;
        Token op_tok;
        std::shared_ptr<Node> right_node;
        BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class VarDefNode: public Node {
    public:
        Token name_tok;
        std::shared_ptr<Node> val;
        VarDefNode(const Token& name_tok, const std::shared_ptr<Node>& val);
        NodeType get_type() const;
        std::string to_string() const;
    };

    // class VarAssignNode: public Node {
    // public:
    //     Token name_tok;
    //     std::shared_ptr<Node> val;
    //     VarAssignNode(const Token& name_tok, const std::shared_ptr<Node>& val);
    //     NodeType get_type() const;
    //     std::string to_string() const;
    // };

    class ExprStmtNode: public Node {
    public:
        std::shared_ptr<Node> expr_node;
        ExprStmtNode(const std::shared_ptr<Node>& expr_node);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class BlockStmtNode: public Node {
    public:
        std::shared_ptr<Node> stmt_list_node;
        BlockStmtNode(const std::shared_ptr<Node>& stmt_list_node);
        NodeType get_type() const;
        std::string to_string() const;
    };
    
    class IfStmtNode: public Node {
    public:
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> body;
        IfStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class IfElseStmtNode: public Node {
    public:
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> if_body;
        std::shared_ptr<Node> else_body;
        IfElseStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& if_body,  const std::shared_ptr<Node>& else_body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class WhileStmtNode: public Node {
    public:
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> body;
        WhileStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class ForStmtNode: public Node {
    public:
        std::shared_ptr<Node> init;
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> update;
        std::shared_ptr<Node> body;
        ForStmtNode(const std::shared_ptr<Node>& init, const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& update, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class ReturnStmtNode: public Node {
    public:
        std::shared_ptr<Node> expr_node;
        ReturnStmtNode(const std::shared_ptr<Node>& expr_node);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class StmtListNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts);
        NodeType get_type() const;
        std::string to_string() const;
    };
};

#endif