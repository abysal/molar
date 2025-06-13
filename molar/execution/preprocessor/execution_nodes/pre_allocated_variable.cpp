//
// Created by Akashic on 6/8/2025.
//

#include "pre_allocated_variable.hpp"
#include <ostream>
#include <print>

#include "execution/preprocessor/processed_ast_visitor.hpp"
#include "internal/checked_down_cast.hpp"

using namespace molar::ast;

namespace molar::exec::ast {
    void PreAllocatedVariable::print(std::ostream& out, const uint32_t index) {
        Expression::print_util_tab(out, index);
        out << "PreAllocatedVariable: \n";
        Expression::print_util_tab(out, index);
        out << "Variable Id: " << this->get_value() << "\n";
    }

    void PreAllocatedVariable::visit_node(molar::ast::AstVisitor& visitor) {
        auto& visit = molar::details::type_asserted_cast<ProcessedAstVisitor>(visitor);
        (void)visit.visit_processed_variable(*this);
    }

    void PreAllocatedVariableAssign::print(std::ostream& out, uint32_t index) {
        Expression::print_util_tab(out, index);
        out << "PreAllocatedVariableAssign: \n";
        Expression::print_util_tab(out, index);
        out << "Variable Id: " << this->get_value() << "\n";
        Expression::print_util_tab(out, index);
        out << "Expression:\n";
        this->assignment->print(out, index + 1);
    }

    void PreAllocatedVariableAssign::visit_node(molar::ast::AstVisitor& visitor) {
        if (auto& visit = molar::details::type_asserted_cast<ProcessedAstVisitor>(visitor);
            visit.visit_processed_variable_assign(*this)) {
            visit.visit_processed_variable(*this);
            this->assignment->visit_node(visit);
        }
    }
} // namespace molar::exec::ast
