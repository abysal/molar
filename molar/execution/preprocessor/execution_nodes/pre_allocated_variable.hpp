//
// Created by Akashic on 6/8/2025.
//

#ifndef PRE_ALLOCATED_VARIABLE_HPP
#define PRE_ALLOCATED_VARIABLE_HPP
#include "ast/expression.hpp"
#include "ast/internal/variable_manager.hpp"

namespace molar::exec::ast {

    class PreAllocatedVariable : public molar::ast::RawExpression,
                                 public molar::ast::VariableManager<uint32_t> {
    public:
        ~PreAllocatedVariable() override = default;

        explicit PreAllocatedVariable(const uint32_t id)
            : RawExpression(molar::ast::AstKind::PreAllocatedVariableReference),
              VariableManager(id) {}

        void print(std::ostream& out, uint32_t index) override;

        void visit_node(class molar::ast::AstVisitor& visitor) override;
    };

    class PreAllocatedVariableAssign : public PreAllocatedVariable {
    public:
        ~PreAllocatedVariableAssign() override = default;

        PreAllocatedVariableAssign(
            const uint32_t id, molar::ast::RawExpressionPtr&& rawExpression
        )
            : PreAllocatedVariable(id), assignment(std::move(rawExpression)) {
            this->type = molar::ast::AstKind::PreAllocatedAssignment;
        }

        void print(std::ostream& out, uint32_t index) override;

        void visit_node(class molar::ast::AstVisitor& visitor) override;

        [[nodiscard]] molar::ast::RawExpressionPtr& get_assignment() {
            return this->assignment;
        }

    protected:
        molar::ast::RawExpressionPtr assignment;
    };
} // namespace molar::exec::ast

#endif // PRE_ALLOCATED_VARIABLE_HPP
