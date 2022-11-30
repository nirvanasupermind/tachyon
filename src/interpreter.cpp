#include <iostream>
#include <cmath>
#include <string>

#include "error.h"
#include "token.h"
#include "node.h"
#include "value.h"
#include "interpreter.h"

namespace eris {
    Interpreter::Interpreter(const std::string& filename)
        : filename(filename) {
    }

    void Interpreter::expect_type(size_t line, const ErisValue &val, ErisValueType type) const {
        if (val.type != type) {
            raise_error(filename, line, "illegal operation");
        }
    }

    ErisValue Interpreter::visit(const Node& node) {
        switch (node.type) {
        case NodeType::NUMBER:
            return visit_number_node(node);
        case NodeType::UNARY:
            return visit_unary_node(node);
        case NodeType::BINARY:
            return visit_binary_node(node);
        case NodeType::EXPR_STATEMENT:
            return visit_expr_statement_node(node);
        case NodeType::PROGRAM:
            return visit_program_node(node);
        default:
            throw std::runtime_error("invalid node");
        }
    }
    
    ErisValue Interpreter::visit_number_node(const Node& node) {
        return ErisValue(ErisValueType::NUMBER, std::stod(node.val));
    }

    ErisValue Interpreter::visit_unary_node(const Node& node) {
        ErisValue operand = visit(node.children.at(0));

        switch (node.op) {
            case TokenType::PLUS: {
                expect_type(node.line, operand, ErisValueType::NUMBER);
                return operand;
            }
                
            case TokenType::MINUS: {
                expect_type(node.line, operand, ErisValueType::NUMBER);
                return ErisValue(ErisValueType::NUMBER, -operand.number);
            }

            default:
                throw std::runtime_error("invalid node");
        }
    }
    
    ErisValue Interpreter::visit_binary_node(const Node& node) {
        ErisValue lhs = visit(node.children.at(0));
        ErisValue rhs = visit(node.children.at(1));

        switch (node.op) {
            case TokenType::PLUS: {
                expect_type(node.line, lhs, ErisValueType::NUMBER);
                expect_type(node.line, rhs, ErisValueType::NUMBER);
                return ErisValue(ErisValueType::NUMBER, lhs.number + rhs.number);
            }
                
            case TokenType::MINUS: {
                expect_type(node.line, lhs, ErisValueType::NUMBER);
                expect_type(node.line, rhs, ErisValueType::NUMBER);
                return ErisValue(ErisValueType::NUMBER, lhs.number - rhs.number);
            }

            case TokenType::MUL: {
                expect_type(node.line, lhs, ErisValueType::NUMBER);
                expect_type(node.line, rhs, ErisValueType::NUMBER);
                return ErisValue(ErisValueType::NUMBER, lhs.number * rhs.number);
            }

            case TokenType::DIV: {
                expect_type(node.line, lhs, ErisValueType::NUMBER);
                expect_type(node.line, rhs, ErisValueType::NUMBER);
                
                if(rhs.number == 0) {
                    raise_error(filename, node.line, "division by zero");
                }

                return ErisValue(ErisValueType::NUMBER, lhs.number / rhs.number);
            }

            case TokenType::MOD: {
                expect_type(node.line, lhs, ErisValueType::NUMBER);
                expect_type(node.line, rhs, ErisValueType::NUMBER);
                return ErisValue(ErisValueType::NUMBER, std::fmod(lhs.number, rhs.number));
            }

            default:
                throw std::runtime_error("invalid node");
        }
    }

    ErisValue Interpreter::visit_expr_statement_node(const Node& node) {
        return visit(node.children.at(0));
    }

    ErisValue Interpreter::visit_program_node(const Node& node) {
        for(const Node &child : node.children) {
            std::cout << visit(child).str() << '\n';
        }

        return ErisValue(ErisValueType::NULL_);
    }
}// namespace eris