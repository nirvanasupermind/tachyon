#include <iostream>
#include <cmath>
#include <string>
#include <memory>

#include "error.h"
#include "token.h"
#include "node.h"
#include "erisval.h"
#include "interpreter.h"

namespace eris {
    Interpreter::Interpreter(const std::string& filename)
        : filename(filename) {
    }

    void Interpreter::check_operand_type(const ErisVal& val, ErisValType type, std::size_t line) const {
        if (val.type != type) {
            raise_error(filename, line, "illegal operation");
        }
    }

    ErisVal Interpreter::visit(Node* node) {
        switch (node->type()) {
        case NodeType::NUMBER:
            return visit(dynamic_cast<NumberNode*>(node));
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
        case NodeType::PROGRAM:
            return visit(dynamic_cast<ProgramNode*>(node));
        default:
            raise_error(filename, node->line, "invalid node");
        }
    }

    ErisVal Interpreter::visit(NumberNode* node) {
        return ErisVal(ErisValType::NUMBER, node->val);
    }

    ErisVal Interpreter::visit(NullNode* node) {
        return ErisVal(ErisValType::NULL_);
    }

    ErisVal Interpreter::visit(TrueNode* node) {
        return ErisVal(ErisValType::BOOL, true);
    }

    ErisVal Interpreter::visit(FalseNode* node) {
        return ErisVal(ErisValType::BOOL, false);
    }

    ErisVal Interpreter::visit(UnaryNode* node) {
        ErisVal operand = visit(node->operand_node.get());

        switch (node->op) {
        case TokenType::PLUS: {
            check_operand_type(operand, ErisValType::NUMBER, node->line);
            return operand;
        }

        case TokenType::MINUS: {
            check_operand_type(operand, ErisValType::NUMBER, node->line);
            return ErisVal(ErisValType::NUMBER, -operand.number);
        }

        default:
            throw std::runtime_error("invalid node");
        }
    }

    ErisVal Interpreter::visit(BinaryNode* node) {
        ErisVal lhs = visit(node->node_a.get());
        ErisVal rhs = visit(node->node_b.get());

        switch (node->op) {
        case TokenType::PLUS: {
            check_operand_type(lhs, ErisValType::NUMBER, node->line);
            check_operand_type(rhs, ErisValType::NUMBER, node->line);

            return ErisVal(ErisValType::NUMBER, lhs.number + rhs.number);
        }

        case TokenType::MINUS: {
            check_operand_type(lhs, ErisValType::NUMBER, node->line);
            check_operand_type(rhs, ErisValType::NUMBER, node->line);

            return ErisVal(ErisValType::NUMBER, lhs.number - rhs.number);
        }

        case TokenType::MUL: {
            check_operand_type(lhs, ErisValType::NUMBER, node->line);
            check_operand_type(rhs, ErisValType::NUMBER, node->line);
            
            return ErisVal(ErisValType::NUMBER, lhs.number * rhs.number);
        }

        case TokenType::DIV: {
            check_operand_type(lhs, ErisValType::NUMBER, node->line);
            check_operand_type(rhs, ErisValType::NUMBER, node->line);

            if (rhs.number == 0) {
                raise_error(filename, node->line, "division by zero");
            }

            return ErisVal(ErisValType::NUMBER, lhs.number / rhs.number);
        }

        case TokenType::MOD: {
            check_operand_type(lhs, ErisValType::NUMBER, node->line);
            check_operand_type(rhs, ErisValType::NUMBER, node->line);

            if (rhs.number == 0) {
                raise_error(filename, node->line, "modulo by zero");
            }

            return ErisVal(ErisValType::NUMBER, std::fmod(lhs.number, rhs.number));
        }

        default:
            throw std::runtime_error("invalid node");
        }
    }

    ErisVal Interpreter::visit(ProgramNode* node) {
        for (std::size_t i = 0; i < node->stmts.size(); i++) {
            std::cout << visit(node->stmts.at(i).get()).str() << '\n';
        }

        return ErisVal(ErisValType::NULL_);
    }
}// namespace eris