//
// Created by Akashic on 5/28/2025.
//

#include "controll_flow.hpp"

namespace molar::ast {
    void LoopExpression::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << "Count Expression: \n";
        this->count_expression->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "Loop Expression: \n";
        this->loop_expression.print(out, index + 1);
    }

    void ForEachExpression::print(std::ostream &out, const uint32_t index) {
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

    void ConditionalExpression::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << "Conditional Expression: \n";
        this->condition->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "If Expression: \n";
        this->if_expression->print(out, index + 1);
    }

    void TernaryExpression::print(std::ostream &out, const uint32_t index) {
        ConditionalExpression::print(out, index);
        Expression::print_util_tab(out, index);
        out << "Else Expression: \n";
        this->else_expression->print(out, index + 1);
    }
} // molar
