//
// Created by Akashic on 6/10/2025.
//

#ifndef AST_REBUILDER_HPP
#define AST_REBUILDER_HPP
#include "molang_preprocessor.hpp"

namespace molar::exec {
    class Rebuilder {
    public:
        Rebuilder(AstCollectorState& state, MolangAstGenerator::MolarAst& ast)
            : ast_information(state), ast(ast) {}

        void rebuild() const;

    private:
        void replace_in_expression(ast::RawExpressionPtr& expression) const;

    private:
        AstCollectorState&            ast_information;
        MolangAstGenerator::MolarAst& ast;
    };
} // namespace molar::exec

#endif // AST_REBUILDER_HPP
