//
// Created by Akashic on 6/10/2025.
//

#include "processed_ast_visitor.hpp"

#include "execution_nodes/pre_allocated.hpp"
#include "execution_nodes/pre_allocated_variable.hpp"

namespace molar::exec::ast::details {
    bool ProcessedAstVisitorReplacer::visit_processed_variable_assign(
        class PreAllocatedVariableAssign& expression
    ) {
        expression.get_assignment() =
            std::move(this->visitor.replace(std::move(expression.get_assignment())));
        return true;
    }

    bool ProcessedAstVisitorReplacer::visit_processed_call(class PreAllocatedCall& expression) {
        for (auto& args = expression.get_arguments(); auto& arg : args) {
            arg = std::move(this->visitor.replace(std::move(arg)));
        }
        return true;
    }
} // namespace molar::exec::ast::details
