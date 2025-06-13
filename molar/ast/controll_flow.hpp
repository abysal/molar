//
// Created by Akashic on 5/28/2025.
//

#ifndef CONTROLL_FLOW_HPP
#define CONTROLL_FLOW_HPP
#include "expression.hpp"
#include "variable.hpp"

namespace molar::ast {
    class LoopExpression final : public Expression {
    public:
        LoopExpression(
            const size_t position, const size_t size, RawExpressionPtr&& count_expression,
            BlockExpression&& loop_expression
        )
            : Expression(position, size, AstKind::LoopExpression),
              count_expression(std::move(count_expression)),
              loop_expression(std::move(loop_expression)) {}

        [[nodiscard]] RawExpressionPtr& get_count_expression() {
            return this->count_expression;
        }

        [[nodiscard]] BlockExpression& get_loop_expression() { return this->loop_expression; }

        ~LoopExpression() override = default;

        void print(std::ostream& out, const uint32_t index) override;

        void visit_node(class AstVisitor& visitor) override;

    private:
        RawExpressionPtr count_expression;
        BlockExpression  loop_expression;
    };

    class ForEachExpression final : public Expression {
    public:
        ~ForEachExpression() override = default;

        void print(std::ostream& out, const uint32_t index) override;

        ForEachExpression(
            const size_t position, const size_t size, VariableReference&& storage,
            RawExpressionPtr&& array_fetch_expression, BlockExpression&& loop_expression
        )
            : Expression(position, size, AstKind::ForEachExpression),
              storage(std::move(storage)),
              array_fetch_expression(std::move(array_fetch_expression)),
              loop_expression(std::move(loop_expression)) {};

        [[nodiscard]] VariableReference& get_storage() { return this->storage; }

        [[nodiscard]] RawExpressionPtr& get_array_fetch_expression() {
            return this->array_fetch_expression;
        }

        [[nodiscard]] BlockExpression& get_loop_expression() { return this->loop_expression; }

        void visit_node(class AstVisitor& visitor) override;

    private:
        VariableReference storage;
        RawExpressionPtr  array_fetch_expression;
        BlockExpression   loop_expression;
    };

    class ConditionalExpression : public Expression {
    public:
        void print(std::ostream& out, const uint32_t index) override;

        ConditionalExpression(
            const size_t position, const size_t size, RawExpressionPtr&& condition_expression,
            RawExpressionPtr&& if_expression
        )
            : Expression(position, size, AstKind::ConditionalExpression),
              condition(std::move(condition_expression)),
              if_expression(std::move(if_expression)) {}

        ~ConditionalExpression() override = default;

    protected:
        RawExpressionPtr condition;
        RawExpressionPtr if_expression;

    public:
        [[nodiscard]] RawExpressionPtr& get_condition() { return this->condition; }

        [[nodiscard]] RawExpressionPtr& get_if_expression() { return this->if_expression; }

        void visit_node(class AstVisitor& visitor) override;
    };

    class TernaryExpression final : public ConditionalExpression {
    public:
        void print(std::ostream& out, const uint32_t index) override;

        TernaryExpression(
            const size_t position, const size_t size, RawExpressionPtr&& condition_expression,
            RawExpressionPtr&& if_expression, RawExpressionPtr&& else_expression
        )
            : ConditionalExpression(
                  position, size, std::move(condition_expression), std::move(if_expression)
              ),
              else_expression(std::move(else_expression)) {
            this->type = AstKind::TernaryExpression;
        }

    protected:
        RawExpressionPtr else_expression;

    public:
        [[nodiscard]] RawExpressionPtr& get_else_expression() { return this->else_expression; }

        void visit_node(class AstVisitor& visitor) override;
    };
} // namespace molar::ast

#endif // CONTROLL_FLOW_HPP
