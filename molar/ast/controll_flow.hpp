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
        LoopExpression(const size_t position, const size_t size, ExpressionPtr &&count_expression,
                       BlockExpression &&loop_expression) : Expression(position, size,
                                                                       AstKind::LoopExpression),
                                                            count_expression(
                                                                std::move(count_expression)),
                                                            loop_expression(
                                                                std::move(loop_expression)) {
        }

        [[nodiscard]] ExpressionPtr &get_count_expression() {
            return this->count_expression;
        }

        [[nodiscard]] BlockExpression &get_loop_expression() {
            return this->loop_expression;
        }

        ~LoopExpression() override = default;

        void print(std::ostream &out, const uint32_t index) override;

    private:
        ExpressionPtr count_expression;
        BlockExpression loop_expression;
    };

    class ForEachExpression final : public Expression {
    public:
        ~ForEachExpression() override = default;

        void print(std::ostream &out, const uint32_t index) override;

        ForEachExpression(const size_t position, const size_t size, Variable &&storage,
                          ExpressionPtr &&array_fetch_expression,
                          BlockExpression &&loop_expression) : Expression(position, size,
                                                                          AstKind::ForEachExpression),
                                                               storage(std::move(storage)),
                                                               array_fetch_expression(
                                                                   std::move(array_fetch_expression)),
                                                               loop_expression(std::move(loop_expression)) {
        };

        [[nodiscard]] Variable &get_storage() {
            return this->storage;
        }

        [[nodiscard]] ExpressionPtr &get_array_fetch_expression() {
            return this->array_fetch_expression;
        }

        [[nodiscard]] BlockExpression &get_loop_expression() {
            return this->loop_expression;
        }

    private:
        Variable storage;
        ExpressionPtr array_fetch_expression;
        BlockExpression loop_expression;
    };

    class ConditionalExpression : public Expression {
    public:
        void print(std::ostream &out, const uint32_t index) override;

        ConditionalExpression(const size_t position, const size_t size, ExpressionPtr &&condition_expression,
                              ExpressionPtr &&if_expression) : Expression(position, size,
                                                                          AstKind::ConditionalExpression),
                                                               condition(std::move(condition_expression)),
                                                               if_expression(std::move(if_expression)) {
        }

        ~ConditionalExpression() override = default;

    protected:
        ExpressionPtr condition;
        ExpressionPtr if_expression;

    public:
        [[nodiscard]] ExpressionPtr &get_condition() {
            return this->condition;
        }

        [[nodiscard]] ExpressionPtr &get_if_expression() {
            return this->if_expression;
        }
    };

    class TernaryExpression final : public ConditionalExpression {
    public:
        void print(std::ostream &out, const uint32_t index) override;

        TernaryExpression(const size_t position, const size_t size, ExpressionPtr &&condition_expression,
                          ExpressionPtr &&if_expression,
                          ExpressionPtr &&else_expression) : ConditionalExpression(position, size,
                                                                 std::move(condition_expression),
                                                                 std::move(if_expression)),
                                                             else_expression(std::move(else_expression)) {
            this->type = AstKind::TernaryExpression;
        }

    protected:
        ExpressionPtr else_expression;

    public:
        [[nodiscard]] ExpressionPtr &get_else_expression() {
            return this->else_expression;
        }
    };
} // molar

#endif //CONTROLL_FLOW_HPP
