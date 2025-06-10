//
// Created by Akashic on 5/25/2025.
//

#ifndef EXPRESSION_NODE_HPP
#define EXPRESSION_NODE_HPP
#include <format>
#include <memory>

#include "molang_token.hpp"

namespace molar {
    struct Token;
}

namespace molar::ast {
    enum class AstKind {
        BooleanLiteral,
        NumericLiteral,
        StringLiteral,
        IdentifierLiteral,
        VariableReference,
        ParenthesizedExpression,
        BlockExpression,
        BinaryExpression,
        UnaryExpression,
        TernaryExpression,
        ConditionalExpression,
        AssignmentExpression,
        ResourceExpression,
        ArrayAccessExpression,
        ArrowAccessExpression,
        CallExpression,
        LoopExpression,
        ForEachExpression,
        Break,
        Continue,
        This,
        Return,
    };

    std::string ast_kind_to_string(const AstKind kind);

    class RawExpression {
    public:
        virtual ~RawExpression() = default;

        static void print_util_tab(std::ostream &out, const uint32_t indent_level);

        virtual void print(std::ostream &out, const uint32_t index = 0) = 0;

        virtual void visit_node(class AstVisitor &visitor) = 0;

    protected:
    };

    class Expression : public RawExpression {
    public:
        ~Expression() override = default;

        void print(std::ostream &out, uint32_t index) override;

        Expression(const Expression &) = delete;

        Expression(Expression &&) noexcept = default;

        Expression &operator=(const Expression &) = delete;

        Expression &operator=(Expression &&) noexcept = default;

        Expression(const size_t position, const size_t size, const AstKind type) : position(position), size(size),
            type(type) {
        }

        explicit Expression(const size_t position, const AstKind type) : position(position), size(1), type(type) {
        }

        explicit Expression(const Token &token, const AstKind type) : position(token.position), size(token.size),
                                                                      type(type) {
        }

        void visit_node(class AstVisitor &visitor) override = 0;

        [[nodiscard]] size_t get_size() const { return this->size; }
        [[nodiscard]] size_t get_position() const { return this->position; }
        [[nodiscard]] AstKind get_type() const { return this->type; }

    protected:
        Expression() = default;

    protected:
        size_t position{};
        size_t size{};
        AstKind type{};
    };

    using RawExpressionPtr = std::unique_ptr<RawExpression>;
    using RawExpressionList = std::vector<RawExpressionPtr>;

    using ExpressionPtr = std::unique_ptr<Expression>;
    using ExpressionList = std::vector<ExpressionPtr>;

    class ParenthesizedExpression : public Expression {
    public:
        ParenthesizedExpression(const size_t position, const size_t size,
                                RawExpressionList &&expressions) : Expression(position, size,
                                                                              AstKind::ParenthesizedExpression),
                                                                   expressions(std::move(expressions)) {
        }

        [[nodiscard]] RawExpressionList &get_expressions() { return this->expressions; }
        [[nodiscard]] const RawExpressionList &get_expressions() const { return this->expressions; }

        ~ParenthesizedExpression() override = default;

        ParenthesizedExpression(const ParenthesizedExpression &) = delete;

        ParenthesizedExpression(ParenthesizedExpression &&) noexcept = default;

        void print(std::ostream &out, const uint32_t index) override;

        void visit_node(class AstVisitor &visitor) override;

    protected:
        RawExpressionList expressions{};
    };

    class BlockExpression final : public ParenthesizedExpression {
    public:
        BlockExpression(const size_t position, const size_t size,
                        RawExpressionList &&expressions) : ParenthesizedExpression(
            position, size, std::move(expressions)) {
            this->type = AstKind::BlockExpression;
        }

        ~BlockExpression() override = default;

        BlockExpression(const BlockExpression &) = delete;

        BlockExpression(BlockExpression &&) noexcept = default;


        void print(std::ostream &out, const uint32_t index) override;

        void visit_node(class AstVisitor &visitor) override;
    };

    class BinaryExpression final : public Expression {
    public:
        BinaryExpression(const size_t position, const size_t size, const BinaryOp operation, RawExpressionPtr &&left,
                         RawExpressionPtr &&right) : Expression(position, size, AstKind::BinaryExpression),
                                                     left(std::move(left)), right(std::move(right)),
                                                     operation(operation) {
        }

        [[nodiscard]] RawExpressionPtr &get_left() { return this->left; }
        [[nodiscard]] RawExpressionPtr &get_right() { return this->right; }
        [[nodiscard]] BinaryOp get_operation() const { return this->operation; }

        ~BinaryExpression() override = default;

        void print(std::ostream &out, const uint32_t index) override;

        void visit_node(class AstVisitor &visitor) override;

    protected:
        RawExpressionPtr left{};
        RawExpressionPtr right{};
        BinaryOp operation{};
    };

    class UnaryExpression final : public Expression {
    public:
        UnaryExpression(const size_t position, const size_t size, const UnaryOp operation,
                        RawExpressionPtr &&expression) : Expression(position, size, AstKind::UnaryExpression),
                                                         operation(operation), expression(std::move(expression)) {
        }

        ~UnaryExpression() override = default;

        void print(std::ostream &out, const uint32_t index) override;

        [[nodiscard]] RawExpressionPtr &get_expression() { return this->expression; }
        [[nodiscard]] UnaryOp get_operation() const { return this->operation; }

        void visit_node(class AstVisitor &visitor) override;

    protected:
        UnaryOp operation{};
        RawExpressionPtr expression{};
    };
} // namespace molar::ast

#endif //EXPRESSION_NODE_HPP
