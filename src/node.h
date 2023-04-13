#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>
#include "token.h"

namespace tachyon {
    enum class NodeKind {
        NIL,
        NUMBER,
        TRUE,
        FALSE,
        CHAR,
        STRING,
        IDENTIFIER,
        PAREN_EXPR,
        LAMBDA_EXPR,
        OBJECT,
        VEC,
        CALL_EXPR,
        ATTR_EXPR,
        UNARY_EXPR,
        BINARY_EXPR,
        EXPR_STMT,
        VAR_DECL_STMT,
        BLOCK_STMT,
        IF_STMT,
        IF_ELSE_STMT,
        WHILE_STMT,
        FOR_STMT,
        FUNC_DECL_STMT,
        RETURN_STMT,
        CIMPORT_STMT,
        TRY_CATCH_STMT,
        STMT_LIST
    };

    class Node {
    public:
        int line;
        virtual NodeKind kind() const = 0;
        virtual ~Node() = default;
    };

    class NilNode: public Node {
    public:
        explicit NilNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class NumberNode: public Node {
    public:
        double val;
        explicit NumberNode(double val, int line);
        NodeKind kind() const;
        std::string str() const;
    };
    class TrueNode: public Node {
    public:
        explicit TrueNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };
    
    class FalseNode: public Node {
    public:
        explicit FalseNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class CharNode: public Node {
    public:
        char val;
        explicit CharNode(char val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class StringNode: public Node {
    public:
        std::string val;
        explicit StringNode(const std::string& val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class IdentifierNode: public Node {
    public:
        std::string val;
        explicit IdentifierNode(const std::string& val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ParenExprNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ParenExprNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class LambdaExprNode: public Node {
    public:
        std::vector<std::string> args;
        std::shared_ptr<Node> body;
        explicit LambdaExprNode(const std::vector<std::string>& args, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ObjectNode: public Node {
    public:
        std::vector<std::string> keys;
        std::vector<std::shared_ptr<Node> > vals;
        explicit ObjectNode(const std::vector<std::string>& keys, const std::vector<std::shared_ptr<Node> >& vals, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class VecNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > elems;
        explicit VecNode(const std::vector<std::shared_ptr<Node> >& elems, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class CallExprNode: public Node {
    public:
        std::shared_ptr<Node> callee;
        std::vector<std::shared_ptr<Node> > args;
        explicit CallExprNode(std::shared_ptr<Node> callee, const std::vector<std::shared_ptr<Node> >& args, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class AttrExprNode: public Node {
    public:
        std::shared_ptr<Node> object;
        std::string attr;
        explicit AttrExprNode(std::shared_ptr<Node> object, const std::string& attr, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class UnaryExprNode: public Node {
    public:
        Token op;
        std::shared_ptr<Node> node;
        explicit UnaryExprNode(Token op, std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class BinaryExprNode: public Node {
    public:
        Token op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        explicit BinaryExprNode(Token op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ExprStmtNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ExprStmtNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class VarDeclStmtNode: public Node {
    public:
        std::string name;
        std::shared_ptr<Node> val;
        explicit VarDeclStmtNode(const std::string& name, std::shared_ptr<Node> val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class BlockStmtNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit BlockStmtNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class IfStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> body;
        explicit IfStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class IfElseStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> body;
        std::shared_ptr<Node> alternate;
        explicit IfElseStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, std::shared_ptr<Node> alternate, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class WhileStmtNode: public Node {
    public:
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> body;
        explicit WhileStmtNode(std::shared_ptr<Node> test, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };
    
    class ForStmtNode: public Node {
    public:
        std::shared_ptr<Node> init;    
        std::shared_ptr<Node> test;    
        std::shared_ptr<Node> update;    
        std::shared_ptr<Node> body;
        explicit ForStmtNode(std::shared_ptr<Node> init, std::shared_ptr<Node> test, std::shared_ptr<Node> update, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class FuncDeclStmtNode: public Node {
    public:
        std::string name;
        std::vector<std::string> args;
        std::shared_ptr<Node> body;
        explicit FuncDeclStmtNode(const std::string& name, const std::vector<std::string>& args, std::shared_ptr<Node> body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ReturnStmtNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ReturnStmtNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class CImportStmtNode: public Node {
    public:
        std::string path;
        explicit CImportStmtNode(const std::string& path, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class TryCatchStmtNode: public Node {
    public:
        std::shared_ptr<Node> try_body;
        std::string ex;
        std::shared_ptr<Node> catch_body;
        explicit TryCatchStmtNode(std::shared_ptr<Node> try_body, const std::string& ex, std::shared_ptr<Node> catch_body, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class StmtListNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        explicit StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts, int line);
        NodeKind kind() const;
        std::string str() const;
    };
} // namespace tachyon

#endif // NODE_H