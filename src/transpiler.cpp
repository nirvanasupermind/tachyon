#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include "node.h"
#include "transpiler.h"

namespace eris {
    Transpiler::Transpiler(const std::string& filename)
        : filename(filename), included_headers({ "\"../src/eris.h\"" }) {
    }

    void Transpiler::visit(Node* node) {
        switch (node->kind()) {
        case NodeKind::NIL:
            return visit(static_cast<NilNode*>(node));
        case NodeKind::NUMBER:
            return visit(static_cast<NumberNode*>(node));
        case NodeKind::IDENTIFIER:
            return visit(static_cast<IdentifierNode*>(node));
        case NodeKind::TRUE:
            return visit(static_cast<TrueNode*>(node));
        case NodeKind::FALSE:
            return visit(static_cast<FalseNode*>(node));
        case NodeKind::CHAR:
            return visit(static_cast<CharNode*>(node));
        case NodeKind::PAREN_EXPR:
            return visit(static_cast<ParenExprNode*>(node));
        case NodeKind::CALL_EXPR:
            return visit(static_cast<CallExprNode*>(node));
        case NodeKind::LAMBDA_EXPR:
            return visit(static_cast<LambdaExprNode*>(node));
        case NodeKind::UNARY_EXPR:
            return visit(static_cast<UnaryExprNode*>(node));
        case NodeKind::BINARY_EXPR:
            return visit(static_cast<BinaryExprNode*>(node));
        case NodeKind::EXPR_STMT:
            return visit(static_cast<ExprStmtNode*>(node));
        case NodeKind::VAR_DECL_STMT:
            return visit(static_cast<VarDeclStmtNode*>(node));
        case NodeKind::BLOCK_STMT:
            return visit(static_cast<BlockStmtNode*>(node));
        case NodeKind::IF_STMT:
            return visit(static_cast<IfStmtNode*>(node));
        case NodeKind::IF_ELSE_STMT:
            return visit(static_cast<IfElseStmtNode*>(node));
        case NodeKind::WHILE_STMT:
            return visit(static_cast<WhileStmtNode*>(node));
        case NodeKind::FOR_STMT:
            return visit(static_cast<ForStmtNode*>(node));
        case NodeKind::STMT_LIST:
            return visit(static_cast<StmtListNode*>(node));
        case NodeKind::FUNC_DECL_STMT:
            return visit(static_cast<FuncDeclStmtNode*>(node));
        default:
            throw std::string(filename + ":" + std::to_string(node->line) + ": unknown AST node type");
        }
    }

    void Transpiler::visit(NilNode* node) {
        post_main_code << "ErisVal::make_nil()";
    }

    void Transpiler::visit(NumberNode* node) {
        post_main_code << "ErisVal::make_num(";
        post_main_code << node->val;
        post_main_code << ')';
    }

    void Transpiler::visit(IdentifierNode* node) {
        post_main_code << node->val;
    }

    void Transpiler::visit(TrueNode* node) {
        post_main_code << "ErisVal::make_bool(true)";
    }

    void Transpiler::visit(FalseNode* node) {
        post_main_code << "ErisVal::make_bool(false)";
    }

    void Transpiler::visit(CharNode* node) {
        post_main_code << "ErisVal::make_char('";
        post_main_code << node->val;
        post_main_code << "')";
    }

    void Transpiler::visit(CallExprNode* node) {
        visit(node->callee.get());
        post_main_code << "({";
        for(int i = 0; i < node->args.size(); i++) {
            visit(node->args.at(i).get());
            if(i < node->args.size() - 1) {
                post_main_code << ',';
            }
        }
        post_main_code << "})";
    }

    void Transpiler::visit(ParenExprNode* node) {
        post_main_code << '(';
        visit(node->node.get());
        post_main_code << ')';
    }

    void Transpiler::visit(UnaryExprNode* node) {
        post_main_code << '(';
        post_main_code << node->op.val;
        visit(node->node.get());
        post_main_code << ')';
    }

    void Transpiler::visit(BinaryExprNode* node) {
        post_main_code << '(';
        visit(node->node_a.get());
        post_main_code << ' ';
        if(node->op.val == "^^") {
            post_main_code << "!=";
        } else {
        post_main_code << node->op.val;
        }
        post_main_code << ' ';
        visit(node->node_b.get());
        post_main_code << ')';
    }

    void Transpiler::visit(ExprStmtNode* node) {
        visit(node->node.get());
        post_main_code << ";";
    }

    void Transpiler::visit(LambdaExprNode* node) {
        post_main_code << "(ErisVal::make_func([&](const std::vector<ErisVal>& args) {\n";
        for(int i = 0; i < node->args.size(); i++) {
            post_main_code << "ErisVal " << node->args.at(i) << " = args.at(" << i << ");\n";
        }
        if(node->body->kind() == NodeKind::BLOCK_STMT) {
        visit(node->body.get());
        post_main_code << "\nreturn ErisVal::make_nil();\n}))";
        } else {
        post_main_code << "return ";
        visit(node->body.get());
        post_main_code << ";\n}))";
        }
    }
    
    void Transpiler::visit(VarDeclStmtNode* node) {
        post_main_code << "ErisVal ";
        post_main_code << node->name;
        post_main_code << " = ";
        visit(node->val.get());
        post_main_code << ';';
    }

    void Transpiler::visit(BlockStmtNode* node) {
        post_main_code << "{\n";
        visit(node->node.get());
        post_main_code << "}";
    }

    void Transpiler::visit(IfStmtNode* node) {
        post_main_code << "if((";
        visit(node->test.get());
        post_main_code << ").b) ";
        visit(node->body.get());
    }

    void Transpiler::visit(IfElseStmtNode* node) {
        post_main_code << "if((";
        visit(node->test.get());
        post_main_code << ").b) ";
        visit(node->body.get());
        post_main_code << " else ";
        visit(node->alternate.get());
    }

    void Transpiler::visit(WhileStmtNode* node) {
        post_main_code << "while((";
        visit(node->test.get());
        post_main_code << ").b)";
        visit(node->body.get());
    }

     void Transpiler::visit(ForStmtNode* node) {
        post_main_code << "for(";
        visit(node->init.get());
        post_main_code << ' ';
        post_main_code << "(";
        visit(node->test.get());
        post_main_code << ").b; ";
        visit(node->update.get());
        post_main_code << ") ";
        visit(node->body.get());
    }
    
    void Transpiler::visit(FuncDeclStmtNode* node) {
        post_main_code << "ErisVal " << node->name << " = ErisVal::make_func([&](const std::vector<ErisVal>& args) {\n";
        for(int i = 0; i < node->args.size(); i++) {
            post_main_code << "ErisVal " << node->args.at(i) << " = args.at(" << i << ");\n";
        }
        visit(node->body.get());
        post_main_code << "\nreturn ErisVal::make_nil();\n});";
    }
    
    void Transpiler::visit(StmtListNode* node) {
        for(int i = 0; i < node->stmts.size(); i++) {
            std::shared_ptr<Node> stmt = node->stmts.at(i);
            visit(stmt.get());
            post_main_code << '\n';
        }
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(node);
        std::string code = "// Generated by Eris\n";
        for (const std::string& header : included_headers) {
            code += "#include " + header + "\n";
            
        }
        code += "int main(int argc, char** argv) {\n";
        code += post_main_code.str();
        code += "    return 0;\n}";
        return code;
    }
}; // namespace eris