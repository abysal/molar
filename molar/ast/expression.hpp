//
// Created by Akashic on 5/25/2025.
//

#ifndef EXPRESSION_NODE_HPP
#define EXPRESSION_NODE_HPP
#include "molang_token.hpp"
#include "internal/source_buffer.hpp"

namespace molar {
    struct Token;
}

namespace molar::ast {
    enum class AstKind {
        BooleanLiteral,
        NumericLiteral,
        StringLiteral,
        IdentifierLiteral,
        VariableDeclaration,
        VariableMember,
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


    class ExpressionNode {
    public:
        ExpressionNode(const size_t position, const size_t size, const AstKind type) : position(position), size(size),
            type(type) {
        }

        explicit ExpressionNode(const size_t position, const AstKind type) : position(position), size(1), type(type) {
        }

        explicit ExpressionNode(const Token &token, const AstKind type) : position(token.position), size(token.size),
                                                                          type(type) {
        }


        [[nodiscard]] size_t get_size() const { return this->size; }
        [[nodiscard]] size_t get_position() const { return this->position; }
        [[nodiscard]] AstKind get_type() const { return this->type; }

    protected:
        size_t position{};
        size_t size{};
        AstKind type{};
    };
} // namespace molar::ast

#endif //EXPRESSION_NODE_HPP
