//
// Created by Akashic on 5/28/2025.
//

#ifndef KEYWORD_HPP
#define KEYWORD_HPP

#include "./ast_visitor.hpp"
#include "ast_visitor.hpp"
#include "expression.hpp"
#include "internal/variable_manager.hpp"

namespace molar::ast {
    class KeywordNode : public Expression {
    public:
        KeywordNode(const size_t position, const size_t size, const AstKind ast_type)
            : Expression(position, size, ast_type) {}
    };

    class BreakNode final : public KeywordNode {
    public:
        BreakNode(const size_t position, const size_t size)
            : KeywordNode(position, size, AstKind::Break) {}

        void visit_node(class AstVisitor& visitor) override {
            (void)visitor.visit_break(*this);
        }
    };

    class ContinueNode final : public KeywordNode {
    public:
        ContinueNode(const size_t position, const size_t size)
            : KeywordNode(position, size, AstKind::Continue) {}

        void visit_node(class AstVisitor& visitor) override {
            (void)visitor.visit_continue(*this);
        }
    };

    class ThisNode final : public KeywordNode {
    public:
        ThisNode(const size_t position, const size_t size)
            : KeywordNode(position, size, AstKind::This) {}

        void visit_node(class AstVisitor& visitor) override { (void)visitor.visit_this(*this); }
    };

    class ReturnNode final : public Expression, public VariableManager<RawExpressionPtr> {
    public:
        ReturnNode(
            const size_t position, const size_t size, RawExpressionPtr&& return_expression
        )
            : Expression(position, size, AstKind::Return) {
            this->value = std::move(return_expression);
        }

        void visit_node(class AstVisitor& visitor) override;

        void print(std::ostream& out, const uint32_t index) override;
    };
} // namespace molar::ast

#endif // KEYWORD_HPP
