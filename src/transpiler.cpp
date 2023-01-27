#include <string>
#include <sstream>
#include <set>
#include "node.h"
#include "transpiler.h"

namespace eris {
    Transpiler::Transpiler(const std::string& filename)
        : filename(filename), included_headers({ "\"erislib.h\"" }) {
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
        default:
            throw std::string("invalid node");
        }
    }

    void Transpiler::visit(NumberNode* node) {
        post_main_code << "(new Number(";
        post_main_code << node->value;
        post_main_code << "))";
    }

    void Transpiler::visit(ParenExprNode* node) {
        post_main_code << '(';
        visit(node->node.get());
        post_main_code << ')';
    }

    void Transpiler::visit(UnaryExprNode* node) {
        if (node->op.type == TokenType::PLUS
            || node->op.type == TokenType::MINUS) {
            // Number operations

            post_main_code << "(new Number(";

            post_main_code << node->op.val;

            if (node->node->kind() == NodeKind::NUMBER) {
                NumberNode* number_node = static_cast<NumberNode*>(node->node.get());
                post_main_code << number_node->value;
            }
            else {
                post_main_code << "static_cast<Number*>(";
                visit(node->node.get());
                post_main_code << ")->val";
            }

            post_main_code << "))";
        }
    }

    void Transpiler::visit(BinaryExprNode* node) {
        if (node->op.type == TokenType::PLUS
            || node->op.type == TokenType::MINUS
            || node->op.type == TokenType::MUL
            || node->op.type == TokenType::DIV) {
            // Number operations

            post_main_code << "(new Number(";

            if (node->node_a->kind() == NodeKind::NUMBER) {
                NumberNode* number_node = static_cast<NumberNode*>(node->node_a.get());
                post_main_code << number_node->value;
            }
            else {
                post_main_code << "static_cast<Number*>(";
                visit(node->node_a.get());
                post_main_code << ")->val";
            }

            post_main_code << node->op.val;

            if (node->node_b->kind() == NodeKind::NUMBER) {
                NumberNode* number_node = static_cast<NumberNode*>(node->node_b.get());
                post_main_code << number_node->value;
            }
            else {
                post_main_code << "static_cast<Number*>(";
                visit(node->node_b.get());
                post_main_code << ")->val";
            }

            post_main_code << "))";
        }
    }

    void Transpiler::visit(ExprStmtNode* node) {
        visit(node->node.get());
        post_main_code << ";\n";
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(node);
        std::string code = "";
        for (const std::string& header : included_headers) {
            code += "#include " + header + "\n";
        }
        code += "int main(int argc, char** argv) {\n    eris_init();\n";
        code += post_main_code.str();
        code += "    return 0;\n}";
        return code;
    }
}; // namespace eris