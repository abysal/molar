//
// Created by Akashic on 5/28/2025.
//

#include "controll_flow.hpp"
#include <ostream>

#include "ast_visitor.hpp"

namespace molar::ast {
    void LoopExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << "Count Expression: \n";
        this->count_expression->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "Loop Expression: \n";
        this->loop_expression.print(out, index + 1);
    }

    void LoopExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_loop(*this)) {
            this->count_expression->visit_node(visitor);
            this->loop_expression.visit_node(visitor);
        }
    }

    void ForEachExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << "Storage: \n";
        this->storage.print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "Array Fetch Expression: \n";
        this->array_fetch_expression->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "Loop Expression: \n";
        this->loop_expression.print(out, index + 1);
    }

    void ForEachExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_for_each(*this)) {
            this->storage.visit_node(visitor);
            this->array_fetch_expression->visit_node(visitor);
            this->loop_expression.visit_node(visitor);
        }
    }

    void ConditionalExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << "Conditional Expression: \n";
        this->condition->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "If Expression: \n";
        this->if_expression->print(out, index + 1);
    }

    void ConditionalExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_conditional(*this)) {
            this->condition->visit_node(visitor);
            this->if_expression->visit_node(visitor);
        }
    }

    void TernaryExpression::print(std::ostream& out, const uint32_t index) {
        ConditionalExpression::print(out, index);
        Expression::print_util_tab(out, index);
        out << "Else Expression: \n";
        this->else_expression->print(out, index + 1);
    }

    void TernaryExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_ternary(*this)) {
            this->condition->visit_node(visitor);
            this->if_expression->visit_node(visitor);
            this->else_expression->visit_node(visitor);
        }
    }
} // namespace molar::ast
