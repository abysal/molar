//
// Created by Akashic on 5/28/2025.
//

#ifndef ACCESS_EXPRESSION_HPP
#define ACCESS_EXPRESSION_HPP
#include "expression.hpp"
#include "literal.hpp"

namespace molar::ast {
    class CallExpression final : public Expression {
    public:
        ~CallExpression() override = default;

        CallExpression(const size_t position, const size_t size, CallType call_type, IdentifierLiteral &&function_name,
                       RawExpressionList &&arguments) : Expression(position, size, AstKind::CallExpression),
                                                        arguments(std::move(arguments)),
                                                        call_type(call_type),
                                                        function_id(std::move(function_name)) {
        }


        [[nodiscard]] RawExpressionList &get_arguments() {
            return this->arguments;;
        }

        [[nodiscard]] CallType get_call_type() const {
            return this->call_type;
        }

        [[nodiscard]] const IdentifierLiteral &get_function_id() const {
            return this->function_id;
        }

        void print(std::ostream &out, uint32_t index) override;

        void visit_node(class AstVisitor &visitor) override;

    protected:
        RawExpressionList arguments{};
        CallType call_type{};
        IdentifierLiteral function_id;
    };

    class ResourceExpression final : public Expression {
    public:
        ResourceExpression(const size_t position, const size_t size, ResourceKind kind,
                           IdentifierLiteral &&resource_id) : Expression(position, size, AstKind::ResourceExpression),
                                                              resource_kind(kind), resource_id(std::move(resource_id)) {
        }

        ~ResourceExpression() override = default;

        void print(std::ostream &out, uint32_t index) override;

        std::string compile_id();

        [[nodiscard]] ResourceKind get_resource_kind() const {
            return this->resource_kind;
        }

        [[nodiscard]] IdentifierLiteral &get_resource_id() {
            return this->resource_id;
        }

        void visit_node(class AstVisitor &visitor) override;

    protected:
        ResourceKind resource_kind{};
        IdentifierLiteral resource_id;
    };

    class ArrayAccess final : public Expression {
    public:
        ArrayAccess(const size_t position, const size_t size, IdentifierLiteral &&array_id,
                    RawExpressionPtr &&index_expression) : Expression(position, size, AstKind::ArrayAccessExpression),
                                                           array_id(std::move(array_id)),
                                                           index_expression(std::move(index_expression)) {
        }

        ~ArrayAccess() override = default;

        void print(std::ostream &out, uint32_t index) override;

        [[nodiscard]] IdentifierLiteral &get_array_id() {
            return this->array_id;
        }

        [[nodiscard]] RawExpressionPtr &get_index_expression() {
            return this->index_expression;
        }

        void visit_node(class AstVisitor &visitor) override;

    protected:
        IdentifierLiteral array_id;
        RawExpressionPtr index_expression;
    };

    class ArrowAccess final : public Expression {
    public:
        ArrowAccess(const size_t position, const size_t size, RawExpressionPtr &&lhs_expression,
                    RawExpressionPtr &&rhs_expression) : Expression(position, size, AstKind::ArrowAccessExpression),
                                                         lhs(std::move(lhs_expression)),
                                                         rhs(std::move(rhs_expression)) {
        }

        [[nodiscard]] RawExpressionPtr &get_lhs() {
            return this->lhs;
        }

        [[nodiscard]] RawExpressionPtr &get_rhs() {
            return this->rhs;
        }

        void print(std::ostream &out, uint32_t index) override;

        void visit_node(class AstVisitor &visitor) override;

    protected:
        RawExpressionPtr lhs;
        RawExpressionPtr rhs;
    };
} // molar

#endif //ACCESS_EXPRESSION_HPP
