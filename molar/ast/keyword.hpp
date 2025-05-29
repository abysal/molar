//
// Created by Akashic on 5/28/2025.
//

#ifndef KEYWORD_HPP
#define KEYWORD_HPP

#include "expression.hpp"
#include "internal/variable_manager.hpp"

namespace molar::ast {
    class KeywordNode : public Expression {
    public:
        KeywordNode(const size_t position, const size_t size, const AstKind ast_type
        ) : Expression(position, size, ast_type) {
        }
    };

    class BreakNode final : public KeywordNode {
    public:
        BreakNode(const size_t position, const size_t size) : KeywordNode(position, size, AstKind::Break) {
        }
    };

    class ContinueNode final : public KeywordNode {
    public:
        ContinueNode(const size_t position, const size_t size) : KeywordNode(position, size, AstKind::Continue) {
        }
    };

    class ThisNode final : public KeywordNode {
    public:
        ThisNode(const size_t position, const size_t size) : KeywordNode(position, size, AstKind::This) {
        }
    };

    class ReturnNode final : public Expression, public VariableManager<ExpressionPtr> {
    public:
        ReturnNode(const size_t position, const size_t size, ExpressionPtr &&return_expression) : Expression(
            position, size, AstKind::Return) {
            this->value = std::move(return_expression);
        }

        void print(std::ostream &out, const uint32_t index) override;
    };
} // molar

#endif //KEYWORD_HPP
