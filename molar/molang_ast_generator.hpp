//
// Created by Akashic on 5/25/2025.
//

#ifndef MOLANG_AST_GENERATOR_HPP
#define MOLANG_AST_GENERATOR_HPP
#include "ast/expression.hpp"
#include "ast/variable.hpp"

namespace molar {
    class MolangAstGenerator {
    public:
        class MolarAst {
        public:
            MolarAst() = default;

            [[nodiscard]] ast::ExpressionList &get_expressions() {
                return this->contained_expressions;
            }

        private:
            ast::ExpressionList contained_expressions{};
            bool contains_multi_expression{false};
            // This is true if we have a program which is (1 + 1; 2 + 2) since it has a ';' that makes it a multi expression

            friend class MolangAstGenerator;
        };

    public:
        explicit MolangAstGenerator(TokenBuffer &&tokens, molar_impl::SourceBuffer &&source) : tokens(tokens),
            source(source) {
        }

        MolarAst build_ast();

    private:
        ast::ExpressionPtr build_expression_recursive(uint8_t min_binding_power);

        bool try_var(ast::ExpressionPtr &returns);

        bool try_bool(ast::ExpressionPtr &returns);

        bool try_number(ast::ExpressionPtr &returns);

        bool try_string(ast::ExpressionPtr &returns);

        bool try_parse_paren(ast::ExpressionPtr &returns);

        bool try_parse_block(ast::ExpressionPtr &returns);

        bool try_parse_unary(ast::ExpressionPtr &returns);

        bool try_parse_call(ast::ExpressionPtr &returns);

        bool try_parse_resource(ast::ExpressionPtr &returns);

        bool try_parse_array(ast::ExpressionPtr &returns);

        bool try_parse_loop(ast::ExpressionPtr &returns);

        bool try_parse_for_each(ast::ExpressionPtr &returns);

        bool try_parse_keyword(ast::ExpressionPtr &returns);

        bool try_parse_return(ast::ExpressionPtr &returns);


        std::optional<ast::BlockExpression> build_block();

        std::optional<ast::Variable> build_variable();


        std::pair<ast::ExpressionPtr, bool> next_expression(ast::ExpressionPtr &&lhs, uint8_t min_binding_power);

        ast::ExpressionPtr next_expression();

        bool try_parse_binary(ast::ExpressionPtr &&lhs, std::pair<ast::ExpressionPtr, bool> &value,
                              uint8_t min_binding_power);

        bool try_parse_arrow(ast::ExpressionPtr &&lhs, ast::ExpressionPtr &value);

        bool try_parse_branch(ast::ExpressionPtr &&condition, ast::ExpressionPtr &value);

        static bool token_is_bin_op(const Token &token);

        static bool token_is_unary(const Token &token);

        static bool token_is_call(const Token &token);

        static bool token_is_resource(const Token &token);

    private:
        TokenBuffer tokens;
        molar_impl::SourceBuffer source;
    };
} // molar

#endif //MOLANG_AST_GENERATOR_HPP
