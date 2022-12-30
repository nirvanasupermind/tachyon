#include <string>
#include <regex>
#include <vector>

#include "error.h"
#include "node.h"
#include "transpiler.h"

namespace eris {
    const std::string Transpiler::boilerplate =
        "#include <iostream>\n"
        "#include <string>\n"
        "#include <variant>\n"
        "class _Nil {};\n"
        "using _Val = std::variant<double, bool, std::string, _Nil>;\n"
        "int main() {";

    Transpiler::Transpiler(const std::string& filename)
        : filename(filename) {
    }

    void Transpiler::visit(Node* node) {
        switch (node->type()) {
        case NodeType::NUMBER:
            visit(dynamic_cast<NumberNode*>(node));
            break;
        case NodeType::IDENTIFIER:
            visit(dynamic_cast<IdentifierNode*>(node));
            break;
        case NodeType::NIL:
            visit(dynamic_cast<NilNode*>(node));
            break;
        case NodeType::TRUE:
            visit(dynamic_cast<TrueNode*>(node));
            break;
        case NodeType::FALSE:
            visit(dynamic_cast<FalseNode*>(node));
            break;
        case NodeType::UNARY_EXPR:
            visit(dynamic_cast<UnaryExprNode*>(node));
            break;
        case NodeType::BINARY_EXPR:
            visit(dynamic_cast<BinaryExprNode*>(node));
            break;
        case NodeType::ASSIGNMENT_EXPR:
            visit(dynamic_cast<AssignmentExprNode*>(node));
            break;
        case NodeType::EXPR_STMT:
            visit(dynamic_cast<ExprStmtNode*>(node));
            break;
        case NodeType::VAR_DECL_STMT:
            visit(dynamic_cast<VarDeclStmtNode*>(node));
            break;
        case NodeType::BLOCK_STMT:
            visit(dynamic_cast<BlockStmtNode*>(node));
            break;
        case NodeType::IF_STMT:
            visit(dynamic_cast<IfStmtNode*>(node));
            break;
        case NodeType::IF_ELSE_STMT:
            visit(dynamic_cast<IfElseStmtNode*>(node));
            break;
        case NodeType::WHILE_STMT:
            visit(dynamic_cast<WhileStmtNode*>(node));
            break;
        case NodeType::FOR_STMT:
            visit(dynamic_cast<ForStmtNode*>(node));
            break;
        case NodeType::PROGRAM:
            visit(dynamic_cast<ProgramNode*>(node));
            break;
        default:
            raise_error(filename, node->line, "invalid node");
        }
    }

    void Transpiler::visit(NumberNode* node) {
        body.push_back("_Val(");
        body.push_back(std::to_string(node->val));
        body.push_back(")");
    }

    void Transpiler::visit(IdentifierNode* node) {
        body.push_back(node->val);
    }

    void Transpiler::visit(NilNode* node) {
        body.push_back("_Val(_Nil())");
    }

    void Transpiler::visit(TrueNode* node) {
        body.push_back("_Val(true)");
    }

    void Transpiler::visit(FalseNode* node) {
        body.push_back("_Val(false)");
    }


    void Transpiler::visit(UnaryExprNode* node) {
        body.push_back("_Val(");

        std::string variant_cast_type;

        switch (node->op) {
        case TokenType::PLUS:
        case TokenType::MINUS:
            variant_cast_type = "double";
            break;
        }

        switch (node->op) {
        case TokenType::PLUS:
            body.push_back("+");
            break;
        case TokenType::MINUS:
            body.push_back("-");
            break;
        default:
            break;
        }

        body.push_back("std::get<");
        body.push_back(variant_cast_type);
        body.push_back(">(");
        visit(node->operand_node.get());
        body.push_back("))");
    }

    void Transpiler::visit(BinaryExprNode* node) {
        body.push_back("_Val(");

        std::string variant_cast_type;

        switch (node->op) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MUL:
        case TokenType::DIV:
        case TokenType::LT:
        case TokenType::LE:
        case TokenType::GT:
        case TokenType::GE:
        case TokenType::EE:
        case TokenType::NE:
            variant_cast_type = "double";
            break;
        default:
            break;
        }

        body.push_back("std::get<");
        body.push_back(variant_cast_type);
        body.push_back(">");
        body.push_back("(");
        visit(node->node_a.get());
        body.push_back(")");

        switch (node->op) {
        case TokenType::PLUS:
            body.push_back("+");
            break;
        case TokenType::MINUS:
            body.push_back("-");
            break;
        case TokenType::MUL:
            body.push_back("*");
            break;
        case TokenType::DIV:
            body.push_back("/");
            break;
        case TokenType::LT:
            body.push_back("<");
            break;
        case TokenType::LE:
            body.push_back("<=");
            break;
        case TokenType::GT:
            body.push_back(">");
            break;
        case TokenType::GE:
            body.push_back(">=");
            break;
        case TokenType::EE:
            body.push_back("==");
            break;
        case TokenType::NE:
            body.push_back("!=");
            break;
        default:
            break;
        }

        body.push_back("std::get<");
        body.push_back(variant_cast_type);
        body.push_back(">(");
        visit(node->node_b.get());
        body.push_back("))");
    }


    void Transpiler::visit(AssignmentExprNode* node) {
        if (node->node_a->type() != NodeType::IDENTIFIER) {
            raise_error(filename, node->line, "invalid left-hand side in assignment");
        }

        visit(node->node_a.get());

        switch (node->op) {
        case TokenType::EQ: {
            body.push_back("=");
            visit(node->node_b.get());
            break;
        }
        case TokenType::PE: {
            body.push_back("=");
            body.push_back("_Val(std::get<double>(");
            visit(node->node_a.get());
            body.push_back(")+std::get<double>(");
            visit(node->node_b.get());
            body.push_back("))");
            break;
        }
        default:
            break;
        }
    }

    void Transpiler::visit(ExprStmtNode* node) {
        visit(node->expr_node.get());
        body.push_back(";");
    }

    void Transpiler::visit(VarDeclStmtNode* node) {
        body.push_back("_Val ");
        body.push_back(node->name);
        body.push_back("=");
        visit(node->val.get());
        body.push_back(";");
    }

    void Transpiler::visit(BlockStmtNode* node) {
        body.push_back("{");
        visit(node->program_node.get());
        body.push_back("}");
    }

    void Transpiler::visit(IfStmtNode* node) {
        body.push_back("if (std::get<bool>(");
        visit(node->test.get());
        body.push_back("))");
        visit(node->body.get());
    }

    void Transpiler::visit(IfElseStmtNode* node) {
        body.push_back("if (std::get<bool>(");
        visit(node->test.get());
        body.push_back("))");
        visit(node->body.get());
        body.push_back("else");
        visit(node->alternate.get());
    }

    void Transpiler::visit(WhileStmtNode* node) {
        body.push_back("while (std::get<bool>(");
        visit(node->test.get());
        body.push_back("))");
        visit(node->body.get());
    }

    void Transpiler::visit(ForStmtNode* node) {
        body.push_back("for (");
        visit(node->decl.get());
        body.push_back("std::get<bool>(");
        visit(node->test.get());
        body.push_back(");");
        visit(node->assignment.get());
        body.push_back(")");
        visit(node->body.get());
    }

    void Transpiler::visit(ProgramNode* node) {
        for (std::shared_ptr<Node> stmt : node->stmts) {
            visit(stmt.get());
        }
    }

    std::string Transpiler::postprocess(std::string& code) {
        std::regex rx1("std::get<double>\\(_Val\\(([0-9]+\\.[0-9]+)\\)\\)");

        code = Transpiler::boilerplate + code;
        code = std::regex_replace(code, rx1, "$1");
        code = code + " return 0; }";
        return code;
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(node);

        std::string code = "";

        for (const std::string& str : body) {
            code = code + str;
        }

        code = postprocess(code);

        return code;
    }

} // namespace eriss