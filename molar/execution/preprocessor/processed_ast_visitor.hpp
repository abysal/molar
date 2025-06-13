//
// Created by Akashic on 6/10/2025.
//

#ifndef PROCESSED_AST_VISITOR_HPP
#define PROCESSED_AST_VISITOR_HPP
#include "ast/ast_replacer.hpp"
#include "ast/ast_visitor.hpp"

namespace molar::exec::ast {
    class ProcessedAstVisitor : public virtual molar::ast::AstVisitor {
    public:
        ~ProcessedAstVisitor() override = default;

        explicit ProcessedAstVisitor(molar::ast::RawExpression& expr) : AstVisitor(expr) {}

        virtual bool visit_processed_variable(class PreAllocatedVariable& expression) {
            return true;
        }
        virtual bool
        visit_processed_variable_assign(class PreAllocatedVariableAssign& expression) {
            return true;
        }
        virtual bool visit_processed_call(class PreAllocatedCall& expression) { return true; }
        virtual bool visit_processed_for_loop(class PreAllocatedForLoop& expression) {
            return true;
        }
    };

#pragma warning(push)
#pragma warning(disable : 4250)
    // The order of this is required to allow the overloading
    namespace details {
        class ProcessedAstVisitorReplacer : public ProcessedAstVisitor,
                                            public molar::ast::details::ReplaceVisitor {
        public:
            ProcessedAstVisitorReplacer(
                molar::ast::AstReplaceVisitor& visitor, molar::ast::RawExpression& expression
            )
                : AstVisitor(expression), ProcessedAstVisitor(expression),
                  ReplaceVisitor(visitor, expression) {}

            bool visit_processed_variable_assign(class PreAllocatedVariableAssign& expression
            ) override;

            bool visit_processed_call(class PreAllocatedCall& expression) override;
        };
    } // namespace details
#pragma warning(pop)
} // namespace molar::exec::ast

#endif // PROCESSED_AST_VISITOR_HPP
