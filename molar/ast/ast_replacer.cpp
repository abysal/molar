//
// Created by Akashic on 6/10/2025.
//

#include "ast_replacer.hpp"

#include <corecrt_startup.h>

#include "access_expression.hpp"
#include "ast_visitor.hpp"
#include "controll_flow.hpp"
#include "keyword.hpp"

namespace molar::ast {
    bool details::ReplaceVisitor::visit_parenthesized(class ParenthesizedExpression& expression
    ) {
        for (auto& expressions = expression.get_expressions();
             auto& raw_expression : expressions) {
            raw_expression = this->visitor.replace(std::move(raw_expression));
        }
        return true;
    }

    bool details::ReplaceVisitor::visit_block(class BlockExpression& expression) {
        for (auto& expressions = expression.get_expressions();
             auto& inner_expression : expressions) {
            inner_expression = std::move(this->visitor.replace(std::move(inner_expression)));
        }
        return true;
    }

    bool details::ReplaceVisitor::visit_binary(class BinaryExpression& expression) {
        expression.get_left() =
            std::move(this->visitor.replace(std::move(expression.get_left())));
        expression.get_right() =
            std::move(this->visitor.replace(std::move(expression.get_right())));
        return true;
    }

    bool details::ReplaceVisitor::visit_unary(class UnaryExpression& expression) {
        expression.get_expression() =
            std::move(this->visitor.replace(std::move(expression.get_expression())));
        return true;
    }

    bool details::ReplaceVisitor::visit_call(class CallExpression& expression) {
        for (auto& expressions = expression.get_arguments();
             auto& inner_expression : expressions) {
            inner_expression = std::move(this->visitor.replace(std::move(inner_expression)));
        }
        return true;
    }

    bool details::ReplaceVisitor::visit_array_access(class ArrayAccess& expression) {
        expression.get_index_expression() =
            std::move(this->visitor.replace(std::move(expression.get_index_expression())));
        return true;
    }

    bool details::ReplaceVisitor::visit_loop(class LoopExpression& expression) {
        expression.get_count_expression() =
            std::move(this->visitor.replace(std::move(expression.get_count_expression())));
        (void)this->visit_block(expression.get_loop_expression());
        return true;
    }

    bool details::ReplaceVisitor::visit_for_each(class ForEachExpression& expression) {
        expression.get_array_fetch_expression() =
            std::move(this->visitor.replace(std::move(expression.get_array_fetch_expression()))
            );
        (void)this->visit_block(expression.get_loop_expression());
        return true;
    }

    bool details::ReplaceVisitor::visit_conditional(class ConditionalExpression& expression) {
        expression.get_condition() =
            std::move(this->visitor.replace(std::move(expression.get_condition())));
        expression.get_if_expression() =
            std::move(this->visitor.replace(std::move(expression.get_if_expression())));
        return true;
    }

    bool details::ReplaceVisitor::visit_ternary(class TernaryExpression& expression) {
        (void)this->visit_conditional(expression);
        expression.get_else_expression() =
            std::move(this->visitor.replace(std::move(expression.get_else_expression())));
        return true;
    }

    bool details::ReplaceVisitor::visit_return(class ReturnNode& expression) {
        expression.get_value() =
            std::move(this->visitor.replace(std::move(expression.get_value())));
        return true;
    }

    bool details::ReplaceVisitor::visit_assignment(class VariableAssign& assignment) {
        assignment.get_expression() =
            std::move(this->visitor.replace(std::move(assignment.get_expression())));
        return true;
    }
} // namespace molar::ast
