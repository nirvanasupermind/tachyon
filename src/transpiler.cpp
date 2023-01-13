#include <string>
#include <vector>
#include <cmath>

#include "error.h"
#include "node.h"
#include "transpiler.h"

namespace eris {
    Transpiler::Transpiler(const std::string& filename)
        : filename(filename), include_val_t(false) {
    }

    void Transpiler::visit(Node* node) {
        switch (node->type()) {
        case NodeType::NUMBER:
            visit(dynamic_cast<NumberNode*>(node));
            break;
        case NodeType::STRING:
            visit(dynamic_cast<StringNode*>(node));
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
        case NodeType::PAREN_EXPR:
            visit(dynamic_cast<ParenExprNode*>(node));
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
        case NodeType::FUNC_DECL_STMT:
            visit(dynamic_cast<FuncDeclStmtNode*>(node));
            break;
        case NodeType::PROGRAM:
            visit(dynamic_cast<ProgramNode*>(node));
            break;
        default:
            raise_error(filename, node->line, "invalid node");
        }
    }

    void Transpiler::visit(NumberNode* node) {
        post_main_section << node->val;
        if(std::fmod(node->val, 1) == 0) {
            post_main_section << ".0";
        }
    }

    void Transpiler::visit(StringNode* node) {
        post_main_section << node->val;
    }

    void Transpiler::visit(IdentifierNode* node) {
        if (!include_val_t) {
            include_val_t = true;
        }

        post_main_section << node->val;
    }

    void Transpiler::visit(NilNode* node) {
        post_main_section << "val_t()";
    }

    void Transpiler::visit(TrueNode* node) {
        post_main_section << "true";
    }

    void Transpiler::visit(FalseNode* node) {
        post_main_section << "false";
    }

    void Transpiler::visit(ParenExprNode* node) {
        post_main_section << '(';
        visit(node->expr_node.get());
        post_main_section << ')';
    }

    void Transpiler::visit(UnaryExprNode* node) {
        post_main_section << node->op.val;
        visit(node->operand_node.get());
    }

    void Transpiler::visit(BinaryExprNode* node) {
        visit(node->node_a.get());
        post_main_section << node->op.val;
        visit(node->node_b.get());
    }

    void Transpiler::visit(AssignmentExprNode* node) {
        visit(node->node_a.get());
        if(node->op.type == TokenType::EQ) {
            post_main_section << '=';
            visit(node->node_b.get());
        } else {
            post_main_section << '=';
            visit(node->node_a.get());
            post_main_section << node->op.val.front();
            visit(node->node_b.get());            
        }
    }

    void Transpiler::visit(ExprStmtNode* node) {
        visit(node->expr_node.get());
        post_main_section << ";\n";
    }

    void Transpiler::visit(VarDeclStmtNode* node) {
        if (!include_val_t) {
            include_val_t = true;
        }

        post_main_section << "val_t " << node->name << " = ";
        visit(node->val.get());
        post_main_section << ";\n";
    }

    void Transpiler::visit(BlockStmtNode* node) {
        post_main_section << "{\n";
        visit(node->program_node.get());
        post_main_section << "}\n";
    }

    void Transpiler::visit(IfStmtNode* node) {
        post_main_section << "if(";
        visit(node->test.get());
        post_main_section << ")";
        visit(node->body.get());
    }

    void Transpiler::visit(IfElseStmtNode* node) {
        post_main_section << "if(";
        visit(node->test.get());
        post_main_section << ")";
        visit(node->body.get());
        post_main_section << "else";
        visit(node->alternate.get());
    }

    void Transpiler::visit(WhileStmtNode* node) {
        post_main_section << "while(";
        visit(node->test.get());
        post_main_section << ")";
        visit(node->body.get());
    }

    void Transpiler::visit(ForStmtNode* node) {
        post_main_section << "for(";
        visit(node->decl.get());
        visit(node->test.get());
        post_main_section << ';';
        visit(node->assignment.get());
        post_main_section << ")";
        visit(node->body.get());
    }

    void Transpiler::visit(FuncDeclStmtNode* node) {
        if (!include_val_t) {
            include_val_t = true;
        }

        post_main_section << "val_t " << node->name << "=[&" << node->name << "](const std::vector<val_t>& params) {\n";
        for(int i = 0; i < node->params.size(); i++) {
            post_main_section << "val_t " << node->params.at(i) << "=params.at(" << i << ");\n";
        }
        visit(node->body.get());
        post_main_section << "return val_t();";
        post_main_section << "};\n";
    }

    void Transpiler::visit(ProgramNode* node) {
        for (std::shared_ptr<Node> stmt : node->stmts) {
            visit(stmt.get());
        }
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(node);
        if (include_val_t) {
            include_section << "#include <functional>\n";
            include_section << "#include <string>\n";
            include_section << "#include <vector>\n";
            
            pre_main_section
                << "struct val_t;\n"
                << "using func_t = std::function<val_t(const std::vector<val_t>&)};\n"
                << "struct val_t {\n"
                << "    enum { NIL, DOUBLE, BOOL, STRING, FUNC } tag;\n"
                << "    void *ptr; \n"
                << "    val_t() { tag = NIL; ptr = nullptr; }\n" 
                << "    val_t(double d) { tag = DOUBLE; ptr = &d; }\n" 
                << "    val_t(bool b) { tag = BOOL; ptr = &b; } \n" 
                << "    val_t(std::string s) { tag = STRING; ptr = &s; }\n"
                << "    val_t(func_t f) { tag = FUNC; ptr = &f; }\n"
                << "    val_t(const val_t& v) {\n"
                << "        tag = v.tag;\n"
                << "        switch(tag) {\n"
                << "        case NIL: {\n"
                << "            break;\n"
                << "        }\n"
                << "        case DOUBLE: {\n"
                << "            double d = *(double *)v.ptr;\n"
                << "            ptr = &d;\n"
                << "            break;\n"
                << "        }\n"
                << "        case BOOL: {\n"
                << "            bool b = *(bool *)v.ptr;\n"
                << "            ptr = &b;\n"
                << "            break;\n"
                << "        }\n"
                << "        case STRING: {\n"
                << "            std::string s = *(std::string *)v.ptr;\n"
                << "            ptr = &s;\n"
                << "            break;\n"
                << "        }\n"
                << "        case FUNC: {\n"
                << "            func_t f = *(func_t *)v.ptr;\n"
                << "            ptr = &f;\n"
                << "            break;\n"
                << "        }\n"
                << "        }\n"
                << "    }\n"
                << "    operator double() const { return *(double *)ptr; }\n"
                << "    operator std::string() const { return *(std::string *)ptr; }\n"
                << "    operator func_t() const { return *(func_t *)ptr;}\n"
                << "};\n\n";
        }
        return include_section.str() + "\n" + pre_main_section.str() + "int main() {\n"
            + post_main_section.str() + "return 0;\n}";
    }

} // namespace eris