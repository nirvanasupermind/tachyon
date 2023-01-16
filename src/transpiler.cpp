#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <sstream>

#include "error.h"
#include "node.h"
#include "transpiler.h"

namespace eris {
    Transpiler::Transpiler(const std::string& filename)
        : filename(filename), include_val_t(false), global_env(std::shared_ptr<Env>(new Env())) {
    }

    void Transpiler::visit(std::shared_ptr<Env> env, Node* node) {
        switch (node->type()) {
        case NodeType::NUMBER:
            visit(env, dynamic_cast<NumberNode*>(node));
            break;
        case NodeType::STRING:
            visit(env, dynamic_cast<StringNode*>(node));
            break;
        case NodeType::IDENTIFIER:
            visit(env, dynamic_cast<IdentifierNode*>(node));
            break;
        case NodeType::NIL:
            visit(env, dynamic_cast<NilNode*>(node));
            break;
        case NodeType::TRUE:
            visit(env, dynamic_cast<TrueNode*>(node));
            break;
        case NodeType::FALSE:
            visit(env, dynamic_cast<FalseNode*>(node));
            break;
        case NodeType::PAREN_EXPR:
            visit(env, dynamic_cast<ParenExprNode*>(node));
            break;
        case NodeType::CALL_EXPR:
            visit(env, dynamic_cast<CallExprNode*>(node));
            break;
        case NodeType::UNARY_EXPR:
            visit(env, dynamic_cast<UnaryExprNode*>(node));
            break;
        case NodeType::BINARY_EXPR:
            visit(env, dynamic_cast<BinaryExprNode*>(node));
            break;
        case NodeType::ASSIGNMENT_EXPR:
            visit(env, dynamic_cast<AssignmentExprNode*>(node));
            break;
        case NodeType::EXPR_STMT:
            visit(env, dynamic_cast<ExprStmtNode*>(node));
            break;
        case NodeType::VAR_DECL_STMT:
            visit(env, dynamic_cast<VarDeclStmtNode*>(node));
            break;
        case NodeType::BLOCK_STMT:
            visit(env, dynamic_cast<BlockStmtNode*>(node));
            break;
        case NodeType::IF_STMT:
            visit(env, dynamic_cast<IfStmtNode*>(node));
            break;
        case NodeType::IF_ELSE_STMT:
            visit(env, dynamic_cast<IfElseStmtNode*>(node));
            break;
        case NodeType::WHILE_STMT:
            visit(env, dynamic_cast<WhileStmtNode*>(node));
            break;
        case NodeType::FOR_STMT:
            visit(env, dynamic_cast<ForStmtNode*>(node));
            break;
        case NodeType::FUNC_DECL_STMT:
            visit(env, dynamic_cast<FuncDeclStmtNode*>(node));
            break;
        case NodeType::RETURN_STMT:
            visit(env, dynamic_cast<ReturnStmtNode*>(node));
            break;
        case NodeType::PROGRAM:
            visit(env, dynamic_cast<ProgramNode*>(node));
            break;
        default:
            raise_error(filename, node->line, "invalid node");
        }
    }

    void Transpiler::visit(std::shared_ptr<Env> env, NumberNode* node) {
        post_main_section << "(double)" << node->val;
    }

    void Transpiler::visit(std::shared_ptr<Env> env, StringNode* node) {
        post_main_section << "std::string(\"" << escape(node->val) << "\")";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, IdentifierNode* node) {
        try {
        env->check_name(node->val, filename, node->line);
        } catch(const std::exception &e) {
            if(node->val == "print") {
                include_val_t = true;
                include_print = true;

            } else {
                throw e;
            }
        }
        post_main_section << node->val;
    }

    void Transpiler::visit(std::shared_ptr<Env> env, NilNode* node) {
        post_main_section << "val_t()";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, TrueNode* node) {
        post_main_section << "true";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, FalseNode* node) {
        post_main_section << "false";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, ParenExprNode* node) {
        post_main_section << '(';
        visit(env, node->expr_node.get());
        post_main_section << ')';
    }

    void Transpiler::visit(std::shared_ptr<Env> env, CallExprNode* node) {
        if (!include_val_t) {
            include_val_t = true;
        }

        std::shared_ptr<Env> func_env(new Env(env));

        post_main_section << "((func_t)";
        visit(env, node->callee.get());
        post_main_section << ")({";
        for (std::shared_ptr<Node> param : node->params) {
            visit(env, param.get());
            post_main_section << ',';
        }
        post_main_section.seekp(-1, std::ios_base::end);
        post_main_section << "})";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, UnaryExprNode* node) {
        post_main_section << node->op.val;
        visit(env, node->operand_node.get());
    }

    void Transpiler::visit(std::shared_ptr<Env> env, BinaryExprNode* node) {
        visit(env, node->node_a.get());
        post_main_section << node->op.val;
        visit(env, node->node_b.get());
    }

    void Transpiler::visit(std::shared_ptr<Env> env, AssignmentExprNode* node) {
        visit(env, node->node_a.get());
        if (node->op.type == TokenType::EQ) {
            post_main_section << '=';
            visit(env, node->node_b.get());
        }
        else {
            post_main_section << '=';
            visit(env, node->node_a.get());
            post_main_section << node->op.val.front();
            visit(env, node->node_b.get());
        }
    }

    void Transpiler::visit(std::shared_ptr<Env> env, ExprStmtNode* node) {
        visit(env, node->expr_node.get());
        post_main_section << ";\n";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, VarDeclStmtNode* node) {
        if (!include_val_t) {
            include_val_t = true;
        }

        env->scope.insert(node->name);
        post_main_section << "val_t " << node->name << '=';
        visit(env, node->val.get());
        post_main_section << ";\n";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, BlockStmtNode* node) {
        std::shared_ptr<Env> block_env(new Env(env));
        post_main_section << "{\n";
        visit(block_env, node->program_node.get());
        post_main_section << "}\n";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, IfStmtNode* node) {
        post_main_section << "if(";
        visit(env, node->test.get());
        post_main_section << ")";
        visit(env, node->body.get());
    }

    void Transpiler::visit(std::shared_ptr<Env> env, IfElseStmtNode* node) {
        post_main_section << "if(";
        visit(env, node->test.get());
        post_main_section << ")";
        visit(env, node->body.get());
        post_main_section << "else";
        visit(env, node->alternate.get());
    }

    void Transpiler::visit(std::shared_ptr<Env> env, WhileStmtNode* node) {
        post_main_section << "while(";
        visit(env, node->test.get());
        post_main_section << ")";
        visit(env, node->body.get());
    }

    void Transpiler::visit(std::shared_ptr<Env> env, ForStmtNode* node) {
        post_main_section << "for(";
        visit(env, node->decl.get());
        visit(env, node->test.get());
        post_main_section << ';';
        visit(env, node->assignment.get());
        post_main_section << ")";
        visit(env, node->body.get());
    }

    void Transpiler::visit(std::shared_ptr<Env> env, FuncDeclStmtNode* node) {
        if (!include_val_t) {
            include_val_t = true;
        }

        std::shared_ptr<Env> func_env(new Env(env));

        post_main_section << "val_t " << node->name << "=(func_t)([&" << node->name << ',';
        while (env->parent != nullptr) {
            for (const std::string& name : env->scope) {
                post_main_section << '&' << name << ',';
            }
            env = env->parent;
        }
        post_main_section.seekp(-1, std::ios_base::end);
        post_main_section << "](const std::vector<val_t>& params)->val_t {\n";
        for (int i = 0; i < node->params.size(); i++) {
            post_main_section << "val_t " << node->params.at(i) << "=params.at(" << i << ");\n";
            func_env->scope.insert(node->params.at(i));
        }
        visit(func_env, node->body.get());
        post_main_section << "return val_t();";
        post_main_section << "});\n";
    }

    void Transpiler::visit(std::shared_ptr<Env> env, ReturnStmtNode* node) {
        post_main_section << "return ";
        visit(env, node->expr_node.get());
        post_main_section << ';';
    }
    
    void Transpiler::visit(std::shared_ptr<Env> env, ProgramNode* node) {
        for (std::shared_ptr<Node> stmt : node->stmts) {
            visit(env, stmt.get());
        }
    }

    // Taken from StackOverflow
    std::string Transpiler::escape(const std::string& your_string) const {
        // s is our escaped output string
        std::string s = "";
        // loop through all characters
        for (char c : your_string) {
            // check if a given character is printable
            // the cast is necessary to avoid undefined behaviour
            if (isprint((unsigned char)c))
                s += c;
            else {
                std::stringstream stream;
                // if the character is not printable
                // we'll convert it to a hex string using a stringstream
                // note that since char is signed we have to cast it to unsigned first
                stream << std::hex << (unsigned int)(unsigned char)(c);
                std::string code = stream.str();
                s += std::string("\\x") + (code.size() < 2 ? "0" : "") + code;
                // alternatively for URL encodings:
                //s += std::string("%")+(code.size()<2?"0":"")+code;
            }
        }
        return s;
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(global_env, node);
        if (include_val_t) {
            include_section << "#include <functional>\n";
            include_section << "#include <string>\n";
            include_section << "#include <vector>\n";

            pre_main_section
                << "struct val_t;\n"
                << "using func_t = std::function<val_t(const std::vector<val_t>&)>;\n"
                << "struct val_t {\n"
                << "    enum { NIL, NUMBER, BOOL, STRING, FUNC } tag;\n"
                << "    void *ptr; \n"
                << "    val_t() { tag = NIL; ptr = nullptr; }\n"
                << "    val_t(double d) {\n"
                << "        tag = NUMBER;\n"
                << "        ptr = malloc(sizeof(double));\n"
                << "        *(double *)ptr = d;\n"
                << "    }\n"
                << "    val_t(bool b) {\n"
                << "        tag = BOOL;\n"
                << "        ptr = malloc(sizeof(bool));\n"
                << "        *(bool *)ptr = b;\n"
                << "    }\n"
                << "    val_t(const std::string& s) {\n"
                << "        tag = STRING;\n"
                << "        ptr = malloc(sizeof(std::string));\n"
                << "        *(std::string *)ptr = s;\n"
                << "    }\n"
                << "    val_t(const func_t& f) {\n"
                << "        tag = FUNC;\n"
                << "        ptr = malloc(sizeof(func_t));\n"
                << "        *(func_t *)ptr = f;\n"
                << "    }\n"
                << "    val_t(const val_t& v) {\n"
                << "        tag = v.tag;\n"
                << "        switch(tag) {\n"
                << "        case NIL: {\n"
                << "            break;\n"
                << "        }\n"
                << "        case NUMBER: {\n"
                << "            double d = *(double *)v.ptr;\n"
                << "            ptr = malloc(sizeof(double));\n"
                << "            *(double *)ptr = d;\n"
                << "            break;\n"
                << "        }\n"
                << "        case BOOL: {\n"
                << "            bool b = *(bool *)v.ptr;\n"
                << "            ptr = malloc(sizeof(bool));\n"
                << "            *(bool *)ptr = b;\n"
                << "            break;\n"
                << "        }\n"
                << "        case STRING: {\n"
                << "            std::string s = *(std::string *)v.ptr;\n"
                << "            ptr = malloc(sizeof(std::string));\n"
                << "            *(std::string *)ptr = s;\n"\
                << "            ptr = &s;\n"
                << "            break;\n"
                << "        }\n"
                << "        case FUNC: {\n"
                << "            func_t f = *(func_t *)v.ptr;\n"
                << "            ptr = malloc(sizeof(func_t));\n"
                << "            *(func_t *)ptr = f;\n"\
                << "            break;\n"
                << "        }\n"
                << "        }\n"
                << "    }\n"
                << "    ~val_t() {\n"
                << "    }\n"
                << "    operator double() const { return *(double *)ptr; }\n"
                << "    operator std::string() const { return *(std::string *)ptr; }\n"
                << "    operator func_t() const { return *(func_t *)ptr;}\n"
                << "};\n\n";
        }

        if (include_print) {
 
        }

        return include_section.str() + "\n" + pre_main_section.str() + "int main() {\n"
            + post_main_section.str() + "return 0;\n}";
    }

} // namespace eris