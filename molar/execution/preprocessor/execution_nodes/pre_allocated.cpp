//
// Created by Akashic on 6/10/2025.
//

#include "pre_allocated.hpp"

#include "execution/preprocessor/processed_ast_visitor.hpp"
#include "internal/checked_down_cast.hpp"
#include <ostream>

namespace molar::exec::ast {
    void PreAllocatedCall::visit_node(molar::ast::AstVisitor& visitor) {
        auto& visit = molar::details::type_asserted_cast<ProcessedAstVisitor>(visitor);
        if (visit.visit_processed_call(*this)) {
            for (auto& arg : this->arguments) {
                arg->visit_node(visitor);
            }
        }
    }

    void PreAllocatedCall::print(std::ostream& out, const uint32_t index) {
        molar::ast::Expression::print_util_tab(out, index);
        out << "PreAllocatedCall: \n";
        molar::ast::Expression::print_util_tab(out, index);
        out << "Id: " << this->value << "\n";
        molar::ast::Expression::print_util_tab(out, index);
        out << "Arguments: \n";
        for (const auto& arg : this->arguments) {
            arg->print(out, index + 1);
        }
    }

    void PreAllocatedForLoop::print(std::ostream& out, const uint32_t index) {
        molar::ast::Expression::print_util_tab(out, index);
        out << "PreAllocatedForLoop: \n";
        molar::ast::Expression::print_util_tab(out, index);
        out << "Variable Id: " << this->variable_index.get_value() << "\n";
        molar::ast::Expression::print_util_tab(out, index);
        out << "Array Fetch Expression: \n";
        this->array_fetch_expression->print(out, index + 1);
        molar::ast::Expression::print_util_tab(out, index);
        out << "Loop Expression: \n";
        this->loop.print(out, index + 1);
    }

    void PreAllocatedForLoop::visit_node(molar::ast::AstVisitor& visitor) {
        if (auto& visit = molar::details::type_asserted_cast<ProcessedAstVisitor>(visitor);
            !visit.visit_processed_for_loop(*this)) {
            return;
        }

        this->variable_index.visit_node(visitor);
        this->array_fetch_expression->visit_node(visitor);
        this->loop.visit_node(visitor);
    }
} // namespace molar::exec::ast
