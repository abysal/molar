//
// Created by Akashic on 6/10/2025.
//

#ifndef AST_REPLACER_HPP
#define AST_REPLACER_HPP
#include "ast_visitor.hpp"
#include "expression.hpp"

namespace molar::ast {
    class AstReplaceVisitor;

    namespace details {
        // This is in the header to allow something to extend it to handle more types of ast
        // stuff
        class ReplaceVisitor : public virtual AstVisitor {
        public:
            explicit ReplaceVisitor(AstReplaceVisitor& visitor, RawExpression& expression)
                : AstVisitor(expression), visitor(visitor) {}

            ~ReplaceVisitor() override = default;

            bool visit_parenthesized(class ParenthesizedExpression& expression) override;

            bool visit_block(class BlockExpression& expression) override;

            bool visit_binary(class BinaryExpression& expression) override;

            bool visit_unary(class UnaryExpression& expression) override;

            bool visit_call(class CallExpression& expression) override;

            bool visit_array_access(class ArrayAccess& expression) override;

            bool visit_loop(class LoopExpression& expression) override;

            bool visit_for_each(class ForEachExpression& expression) override;

            bool visit_conditional(class ConditionalExpression& expression) override;

            bool visit_ternary(class TernaryExpression& expression) override;

            bool visit_return(class ReturnNode& expression) override;

            bool visit_assignment(class VariableAssign& assignment) override;

        protected:
            AstReplaceVisitor& visitor;
        };
    } // namespace details

    class AstReplaceVisitor {
    public:
        virtual ~AstReplaceVisitor() = default;

        virtual RawExpressionPtr replace(RawExpressionPtr&& expression) = 0;
    };

    class AstReplacer {
    public:
        explicit AstReplacer(RawExpressionPtr& expression) : expression(expression) {}

        template <typename Visitor, typename... Args>
            requires std::derived_from<Visitor, details::ReplaceVisitor>
        void visit_all(AstReplaceVisitor& visit, Args... args) {
            this->expression = std::move(visit.replace(std::move(this->expression)));
            auto visitor     = Visitor(visit, *this->expression, args...);
            visitor.visit();
        }

    private:
        RawExpressionPtr& expression;
    };
} // namespace molar::ast

#endif // AST_REPLACER_HPP
