#include <iostream>
#include <cmath>
#include <string>
#include <memory>

#include "error.h"
#include "token.h"
#include "node.h"
#include "object.h"
#include "value.h"
#include "interpreter.h"

namespace eris {
    Interpreter::Interpreter(const std::string& filename)
        : filename(filename) {
    }

    void Interpreter::check_operand_type(const Value& val, ValueType type, std::size_t line) const {
        if (val.type != type) {
            raise_error(filename, line, "illegal operation");
        }
    }

    Value Interpreter::visit(Node* node) {
        switch (node->type()) {
        case NodeType::NUMBER:
            return visit(dynamic_cast<NumberNode*>(node));
        case NodeType::STRING:
            return visit(dynamic_cast<StringNode*>(node));
        case NodeType::IDENTIFIER:
            return visit(dynamic_cast<IdentifierNode*>(node));
        case NodeType::NULL_:
            return visit(dynamic_cast<NullNode*>(node));
        case NodeType::TRUE:
            return visit(dynamic_cast<TrueNode*>(node));
        case NodeType::FALSE:
            return visit(dynamic_cast<FalseNode*>(node));
        case NodeType::UNARY:
            return visit(dynamic_cast<UnaryNode*>(node));
        case NodeType::BINARY:
            return visit(dynamic_cast<BinaryNode*>(node));
        case NodeType::VAR_DECL:
            return visit(dynamic_cast<VarDeclNode*>(node));
        case NodeType::PROGRAM:
            return visit(dynamic_cast<ProgramNode*>(node));
        default:
            raise_error(filename, node->line, "invalid node");
        }
    }

    Value Interpreter::visit(NumberNode* node) {
        return Value(ValueType::NUMBER, node->val);
    }

    Value Interpreter::visit(StringNode* node) {
        return Value(ValueType::OBJECT, new StringObject(node->val));
    }

    Value Interpreter::visit(IdentifierNode* node) {
        try {
            return global_env.get(node->val);
        } catch(const std::exception &e) {
            throw std::runtime_error("");
        }
    }

    Value Interpreter::visit(NullNode* node) {
        return Value(ValueType::NULL_);
    }

    Value Interpreter::visit(TrueNode* node) {
        return Value(ValueType::BOOL, true);
    }

    Value Interpreter::visit(FalseNode* node) {
        return Value(ValueType::BOOL, false);
    }

    Value Interpreter::visit(UnaryNode* node) {
        Value operand = visit(node->operand_node.get());

        switch (node->op) {
        case TokenType::PLUS: {
            check_operand_type(operand, ValueType::NUMBER, node->line);
            return operand;
        }

        case TokenType::MINUS: {
            check_operand_type(operand, ValueType::NUMBER, node->line);
            return Value(ValueType::NUMBER, -operand.number);
        }

        default:
            throw std::runtime_error("invalid node");
        }
    }

    Value Interpreter::visit(BinaryNode* node) {
        Value lhs = visit(node->node_a.get());
        Value rhs = visit(node->node_b.get());

        switch (node->op) {
        case TokenType::PLUS: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::NUMBER, lhs.number + rhs.number);
        }

        case TokenType::MINUS: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::NUMBER, lhs.number - rhs.number);
        }

        case TokenType::MUL: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::NUMBER, lhs.number * rhs.number);
        }

        case TokenType::DIV: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            if (rhs.number == 0) {
                raise_error(filename, node->line, "division by zero");
            }

            return Value(ValueType::NUMBER, lhs.number / rhs.number);
        }

        case TokenType::MOD: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            if (rhs.number == 0) {
                raise_error(filename, node->line, "modulo by zero");
            }

            return Value(ValueType::NUMBER, std::fmod(lhs.number, rhs.number));
        }

        case TokenType::LT: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::BOOL, lhs.number < rhs.number);
        }

        case TokenType::LE: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::BOOL, lhs.number <= rhs.number);
        }

        case TokenType::GT: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::BOOL, lhs.number > rhs.number);
        }

        case TokenType::GE: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::BOOL, lhs.number >= rhs.number);
        }

        case TokenType::EE: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);

            return Value(ValueType::BOOL, lhs.number == rhs.number);
        }

        case TokenType::NE: {
            check_operand_type(lhs, ValueType::NUMBER, node->line);
            check_operand_type(rhs, ValueType::NUMBER, node->line);
            
            return Value(ValueType::BOOL, lhs.number != rhs.number);
        }

        default:
            throw std::runtime_error("invalid node");
        }
    }


    Value Interpreter::visit(VarDeclNode* node) {
        global_env.def(node->name, visit(node->val.get()));
        return Value(ValueType::NULL_);
    }

    Value Interpreter::visit(ProgramNode* node) {
        for (std::size_t i = 0; i < node->stmts.size(); i++) {
            std::cout << visit(node->stmts.at(i).get()).str() << '\n';
        }

        return Value(ValueType::NULL_);
    }
}// namespace eris