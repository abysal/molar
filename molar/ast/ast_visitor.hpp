//
// Created by Akashic on 5/30/2025.
//

#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP
#include <functional>


namespace molar::ast {
    class RawExpression;
    class ForEachExpression;
    class LoopExpression;
    class ConditionalExpression;
    class TernaryExpression;

    class AstVisitor {
    public:
        virtual ~AstVisitor() = default;

        void visit();

        explicit AstVisitor(RawExpression &expression) : root_expression(expression) {
        }

        virtual bool visit_variable(class VariableReference &variable) { return true; }
        virtual bool visit_string(class StringLiteral &string) { return true; }
        virtual bool visit_bool(class BoolLiteral &boolean) { return true; }
        virtual bool visit_number(class NumericLiteral &number) { return true; }
        virtual bool visit_resource(class ResourceExpression &expression) { return true; };
        virtual bool visit_break(class BreakNode &expression) { return true; };
        virtual bool visit_continue(class ContinueNode &expression) { return true; };
        virtual bool visit_this(class ThisNode &expression) { return true; };
        virtual bool visit_assignment(class VariableAssign &assignment) { return true; };
        virtual bool visit_block(class BlockExpression &expression) { return true; };
        virtual bool visit_parenthesized(class ParenthesizedExpression &expression) { return true; };
        virtual bool visit_binary(class BinaryExpression &expression) { return true; };
        virtual bool visit_unary(class UnaryExpression &expression) { return true; };
        virtual bool visit_ternary(class TernaryExpression &expression) { return true; };
        virtual bool visit_conditional(class ConditionalExpression &expression) { return true; };
        virtual bool visit_array_access(class ArrayAccess &expression) { return true; };
        virtual bool visit_arrow_access(class ArrowAccess &expression) { return true; };
        virtual bool visit_call(class CallExpression &expression) { return true; };
        virtual bool visit_loop(class LoopExpression &expression) { return true; };
        virtual bool visit_for_each(class ForEachExpression &expression) { return true; };
        virtual bool visit_return(class ReturnNode &expression) { return true; };

    private:
        RawExpression &root_expression;
    };
} // molar

#endif //AST_VISITOR_HPP
