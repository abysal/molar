//
// Created by Akashic on 6/10/2025.
//

#ifndef PRE_ALLOCATED_HPP
#define PRE_ALLOCATED_HPP
#include "ast/expression.hpp"
#include "pre_allocated_variable.hpp"

namespace molar::exec::ast {
    class PreAllocatedCall : public molar::ast::RawExpression,
                             public molar::ast::VariableManager<uint32_t> {
    public:
        PreAllocatedCall(const uint32_t call_index, molar::ast::RawExpressionList&& arguments)
            : VariableManager(call_index), arguments(std::move(arguments)) {
            this->type = molar::ast::AstKind::PreAllocatedCall;
        }

        ~PreAllocatedCall() override = default;

        void visit_node(class molar::ast::AstVisitor& visitor) override;

        [[nodiscard]] molar::ast::RawExpressionList& get_arguments() { return this->arguments; }

        void print(std::ostream& out, const uint32_t index) override;

    protected:
        molar::ast::RawExpressionList arguments;
    };

    class PreAllocatedForLoop : public molar::ast::RawExpression {
    public:
        PreAllocatedForLoop(
            const uint32_t                 variable_index,
            molar::ast::RawExpressionPtr&& array_fetch_expression,
            molar::ast::BlockExpression&&  loop
        )
            : RawExpression(molar::ast::AstKind::PreAllocatedForLoop),
              variable_index(variable_index),
              array_fetch_expression(
                  std::forward<molar::ast::RawExpressionPtr>(array_fetch_expression)
              ),
              loop(std::forward<molar::ast::BlockExpression>(loop)) {}

        ~PreAllocatedForLoop() override = default;

        void print(std::ostream& out, const uint32_t index) override;
        void visit_node(class molar::ast::AstVisitor& visitor) override;

        [[nodiscard]] PreAllocatedVariable& get_variable_index() {
            return this->variable_index;
        }
        [[nodiscard]] molar::ast::RawExpressionPtr& get_array_fetch_expression() {
            return this->array_fetch_expression;
        }
        [[nodiscard]] molar::ast::BlockExpression& get_loop() { return this->loop; }

    protected:
        PreAllocatedVariable         variable_index;
        molar::ast::RawExpressionPtr array_fetch_expression;
        molar::ast::BlockExpression  loop;
    };

} // namespace molar::exec::ast

#endif // PRE_ALLOCATED_HPP
