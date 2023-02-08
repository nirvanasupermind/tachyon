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
        case NodeKind::NUMBER:
            return visit(static_cast<NumberNode*>(node));
        case NodeKind::PAREN_EXPR:
            return visit(static_cast<ParenExprNode*>(node));
        case NodeKind::BINARY_EXPR:
            return visit(static_cast<BinaryExprNode*>(node));
        case NodeKind::EXPR_STMT:
            return visit(static_cast<ExprStmtNode*>(node));
        case NodeKind::STMT_LIST:
            return visit(static_cast<StmtListNode*>(node));
        default:
            throw std::string(filename + ":" + std::to_string(node->line) + ": unknown AST node type");
        }
    }

    void Transpiler::visit(NumberNode* node) {
        post_main_code << "ErisVal::make_num(";
        post_main_code << node->value;
        post_main_code << ')';
    }

    void Transpiler::visit(ParenExprNode* node) {
        post_main_code << '(';
        visit(node->node.get());
        post_main_code << ')';
    }

    void Transpiler::visit(UnaryExprNode* node) {
        post_main_code << node->op.val;
        visit(node->node.get());
    }

    void Transpiler::visit(BinaryExprNode* node) {
        visit(node->node_a.get());
        post_main_code << node->op.val;
        visit(node->node_b.get());
    }

    void Transpiler::visit(ExprStmtNode* node) {
        visit(node->node.get());
        post_main_code << ";\n";
    }

    void Transpiler::visit(StmtListNode* node) {
        for(std::shared_ptr<Node> stmt : node->stmts) {
            visit(stmt.get());
        }
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(node);
        std::string code = "";
        for (const std::string& header : included_headers) {
            code += "#include " + header + "\n";
        }
        code += "int main(int argc, char** argv) {\n";
        code += post_main_code.str();
        code += "    return 0;\n}";
        return code;
    }
}; // namespace eris