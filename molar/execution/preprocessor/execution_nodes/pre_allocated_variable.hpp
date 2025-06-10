//
// Created by Akashic on 6/8/2025.
//

#ifndef PRE_ALLOCATED_VARIABLE_HPP
#define PRE_ALLOCATED_VARIABLE_HPP
#include "ast/expression.hpp"
#include "ast/internal/variable_manager.hpp"

namespace molar::exec::ast {
    class PreAllocatedVariable : public molar::ast::RawExpression, public molar::ast::VariableManager<uint32_t> {
    public:
    };
} // molar

#endif //PRE_ALLOCATED_VARIABLE_HPP
