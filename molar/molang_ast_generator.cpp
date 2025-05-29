//
// Created by Akashic on 5/25/2025.
//

#include "molang_ast_generator.hpp"

#include "molang_error.hpp"
#include "ast/access_expression.hpp"
#include "ast/controll_flow.hpp"
#include "ast/keyword.hpp"
#include "ast/variable.hpp"

namespace molar {
    constexpr auto resource_tokens = std::array{
        TokenType::Geometry,
        TokenType::Material,
        TokenType::Texture
    };

    constexpr auto call_ops = std::array{
        CallType::Math,
        CallType::Query
    };

    constexpr auto unary_ops = std::array{
        UnaryOp::Negate,
        UnaryOp::Not,
    };

    constexpr auto tokens_which_could_be_names_constructed = std::array{
        Token{TokenType::Math},
        Token{TokenType::Query},
        Token{TokenType::Variable},
        Token{TokenType::Geometry},
        Token{TokenType::Material},
        Token{TokenType::Texture},
        Token{TokenType::Array},
        Token{TokenType::True},
        Token{TokenType::False},
        Token{TokenType::This},
        Token{TokenType::Break},
        Token{TokenType::Continue},
        Token{TokenType::ForEach},
        Token{TokenType::Loop},
        Token{TokenType::Return},
        Token{TokenType::Temporary},
        Token{TokenType::Context},
        Token{TokenType::Identifier},
    };

    MolangAstGenerator::MolarAst MolangAstGenerator::build_ast() {
        MolarAst ast{};

        while (this->tokens.has_any()) {
            auto ast_node = this->build_expression_recursive(0);
            if (!ast_node) {
                ast.contains_multi_expression = true; // Means we found something like a `;`
                continue;
            }

            ast.contained_expressions.emplace_back(std::move(ast_node));
        }


        return ast;
    }

    // Recursively constructs an AST node
    // NOLINTNEXTLINE
    ast::ExpressionPtr MolangAstGenerator::build_expression_recursive(uint8_t min_binding_power) {
        if (this->tokens.next(Token{TokenType::Semi})) {
            return nullptr;
        }

        auto lhs_expression = this->next_expression();

        if (!lhs_expression) {
            return nullptr;
        }

        while (true) {
            if (ast::ExpressionPtr ptr; this->try_parse_arrow(std::move(lhs_expression), ptr)) {
                lhs_expression = std::move(ptr);
                break;
            } else {
                lhs_expression = std::move(ptr); // ptr always gets set to a valid value, in this cause our expression
            }

            uint8_t right_bind_power = 0xFF;
            try {
                if (const auto bind_power = token_bind_power(Token(this->tokens.peek_value()).type); bind_power) {
                    if (const auto [left_bp, right_bp] = bind_power.value(); left_bp < min_binding_power) {
                        break;
                    } else {
                        right_bind_power = right_bp;
                    }
                } else {
                    break; // Means there is nothing to chain
                }
            } catch (std::out_of_range &) {
                break;
                // Swallow the error since it means we have finished the buffer (the last expression was the last one)
            }

            auto [next_expression, matched] = this->next_expression(std::move(lhs_expression), right_bind_power);

            lhs_expression = std::move(next_expression);
            if (!matched) {
                break;
            }
        }

        return lhs_expression;
    }

    bool MolangAstGenerator::try_var(ast::ExpressionPtr &returns) {
        if (auto value = this->build_variable()) {
            if (this->tokens.next(Token{TokenType::Assign})) {
                auto expression = this->build_expression_recursive(0);
                if (!expression) {
                    throw MolangSyntaxError("Expected expression", value.value().get_position());
                }

                returns = std::make_unique<ast::VariableAssign>(std::move(value.value()), std::move(expression));
            } else {
                returns = std::make_unique<ast::Variable>(std::move(value.value()));
            }

            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_bool(ast::ExpressionPtr &returns) {
        if (const auto token = this->tokens.next_value(std::array{Token{TokenType::True}, Token{TokenType::False}});
            token.has_value()) {
            returns = std::make_unique<ast::BoolLiteral>(token.value(), this->source);
            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_number(ast::ExpressionPtr &returns) {
        if (const auto token = this->tokens.next_value(Token{TokenType::Number}); token.has_value()) {
            returns = std::move(std::make_unique<ast::NumericLiteral>(token.value(), this->source));
            return true;
        }


        return false;
    }

    bool MolangAstGenerator::try_string(ast::ExpressionPtr &returns) {
        if (const auto token = this->tokens.next_value(Token{TokenType::String}); token.has_value()) {
            returns = std::move(std::make_unique<ast::StringLiteral>(token.value(), this->source));
            return true;
        }
        return false;
    }

    ast::ExpressionPtr MolangAstGenerator::next_expression() {
        if (ast::ExpressionPtr value; this->try_var(value)) return value;
        if (ast::ExpressionPtr value; this->try_number(value)) return value;
        if (ast::ExpressionPtr value; this->try_bool(value)) return value;
        if (ast::ExpressionPtr value; this->try_string(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_paren(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_block(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_unary(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_call(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_resource(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_array(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_loop(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_for_each(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_keyword(value)) return value;
        if (ast::ExpressionPtr value; this->try_parse_return(value)) return value;

        // TODO: Add a function which just checks if any invalid tokens are next. Basically meaning that if there is something like an = next we throw. Since there is no valid state where this could happen

        return nullptr;
    }

    // NOLINTNEXTLINE
    bool MolangAstGenerator::try_parse_binary(ast::ExpressionPtr &&lhs, std::pair<ast::ExpressionPtr, bool> &value,
                                              const uint8_t min_binding_power) {
        const auto next_tk = this->tokens.peek_value();
        if (const Token next_token = next_tk; token_is_bin_op(next_token)) {
            const auto op = static_cast<BinaryOp>(next_token.type);
            next_tk.apply(); // We don't need to bind this peek since the expression building consumes it

            auto rhs = this->build_expression_recursive(min_binding_power);
            value = {
                std::make_unique<ast::BinaryExpression>(next_token.position, next_token.size, op, std::move(lhs),
                                                        std::move(rhs)),
                true
            };
            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_parse_arrow(ast::ExpressionPtr &&lhs, ast::ExpressionPtr &value) {
        if (const auto token_temp = this->tokens.next_value(Token{TokenType::Arrow}); token_temp.has_value()) {
            const auto token = token_temp.value();
            auto expr = this->build_expression_recursive(0);

            if (!expr) {
                throw MolangSyntaxError("Expected expression after arrow", token.position);
            }

            value = std::move(
                std::make_unique<ast::ArrowAccess>(token.position, token.size, std::move(lhs), std::move(expr))
            );

            return true;
        }
        value = std::move(lhs);
        return false;
    }

    bool MolangAstGenerator::try_parse_branch(ast::ExpressionPtr &&condition, ast::ExpressionPtr &value) {
        if (const auto token_temp = this->tokens.next_value(Token{TokenType::Conditional}); token_temp) {
            const auto token = token_temp.value();

            auto if_expression = this->build_expression_recursive(0);

            if (!if_expression) {
                throw MolangSyntaxError("Expected expression after conditional", token.position);
            }

            if (const auto temp_colon = this->tokens.next_value(Token{TokenType::Colon}); temp_colon.has_value()) {
                auto else_expression = this->build_expression_recursive(0);

                if (!else_expression) {
                    throw MolangSyntaxError("Expected expression after conditional", token.position);
                }

                value = std::move(std::make_unique<ast::TernaryExpression>(
                    token.position, token.size, std::move(condition), std::move(if_expression),
                    std::move(else_expression)));
            } else {
                value = std::move(std::make_unique<ast::ConditionalExpression>(
                    token.position, token.size, std::move(condition), std::move(if_expression)));
            }

            return true;
        }

        value = std::move(condition);
        return false;
    }

    bool MolangAstGenerator::try_parse_paren(ast::ExpressionPtr &returns) {
        size_t position{};
        if (const auto p = this->tokens.next(Token{TokenType::LeftParen}); !p.has_value()) return false;
        else position = p.value();

        ast::ExpressionList exprs{};

        while (true) {
            if (this->tokens.next(Token{TokenType::RightParen})) break;
            auto expr = this->build_expression_recursive(0);
            if (!expr) continue;
            exprs.emplace_back(std::move(expr));
        }

        returns = std::move(std::make_unique<ast::ParenthesizedExpression>(position, exprs.size(), std::move(exprs)));

        return true;
    }

    bool MolangAstGenerator::try_parse_block(ast::ExpressionPtr &returns) {
        if (auto block = this->build_block()) {
            returns = std::make_unique<ast::BlockExpression>(std::move(block.value()));
            return true;
        }

        return false;
    }

    std::optional<ast::BlockExpression> MolangAstGenerator::build_block() {
        size_t our_position{};

        if (const auto position = this->tokens.next(Token{TokenType::LeftBrace}); !position.has_value())
            return
                    std::nullopt;
        else our_position = position.value();

        ast::ExpressionList expressions{};

        while (!this->tokens.next(Token{TokenType::RightBrace})) {
            auto expr = this->build_expression_recursive(0);
            if (!expr) {
                continue;
            }

            expressions.emplace_back(std::move(expr));
            //this->tokens.skip(-1);
            if (!this->tokens.next({Token{TokenType::Semi}})) {
                throw MolangSyntaxError("Expected semi colon after expression in block", our_position);
            }
        }

        return
                ast::BlockExpression(our_position, expressions.size(), std::move(expressions));
    }

    std::optional<ast::Variable> MolangAstGenerator::build_variable() {
        if (const auto token = this->tokens.next_value(std::array{
            Token{TokenType::Variable}, Token{TokenType::Context}, Token{TokenType::Temporary}
        }); token.has_value()) {
            const Token tkn = token.value();
            return ast::Variable(tkn, this->source, this->tokens);
        }
        return std::nullopt;
    }

    bool MolangAstGenerator::try_parse_unary(ast::ExpressionPtr &returns) {
        if (const auto pos = this->tokens.next_value(std::array{Token{TokenType::Minus}, Token{TokenType::Not}}); pos.
            has_value()) {
            ast::ExpressionPtr expr{this->build_expression_recursive(0)};

            if (!expr) {
                throw MolangSyntaxError("Expected expression after a Unary Operator", pos.value().position);
            }

            returns = std::make_unique<ast::UnaryExpression>(pos.value().position, pos.value().size,
                                                             static_cast<UnaryOp>(pos.value().type), std::move(expr));
            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_parse_call(ast::ExpressionPtr &returns) {
        if (const auto current_token = this->tokens.next_value(std::array{
            Token{TokenType::Math}, Token{TokenType::Query}
        }); current_token.has_value()) {
            const auto kind = static_cast<CallType>(current_token.value().type);

            if (!this->tokens.next(Token{TokenType::Dot}))
                throw MolangSyntaxError("Expected Dot after Call Type (math, or query)",
                                        current_token.value().position);

            const auto ident_token = this->tokens.next_value();

            if (!ident_token.check_types(tokens_which_could_be_names)) {
                throw MolangSyntaxError("Expected identifier after Call Type", ident_token.position);
            }

            auto identifier = ast::IdentifierLiteral(ident_token, this->source);

            ast::ExpressionList params{};

            if (this->tokens.next(Token{TokenType::LeftParen})) {
                try {
                    while (true) {
                        if (this->tokens.next(Token{TokenType::RightParen})) {
                            break;
                        }

                        if (this->tokens.next(Token{TokenType::Comma})) {
                            continue;
                        }

                        auto expr = this->build_expression_recursive(0);

                        if (!expr) {
                            throw MolangSyntaxError("Invalid expression in call list", ident_token.position);
                        }

                        params.emplace_back(std::move(expr));
                    }
                } catch (std::out_of_range &) {
                    // This means we EOF
                    throw MolangSyntaxError("Unexpected End Of File at call expression", ident_token.position);
                }
            }

            returns = std::move(std::make_unique<ast::CallExpression>(current_token.value().position,
                                                                      current_token.value().size, kind,
                                                                      std::move(identifier), std::move(params)));
            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_parse_resource(ast::ExpressionPtr &returns) {
        const auto token_temp = this->tokens.next_value(std::array{
            Token{TokenType::Texture}, Token{TokenType::Geometry}, Token{TokenType::Material}
        });
        if (!token_temp) {
            return false;
        }

        const auto token = token_temp.value();

        if (!this->tokens.next(Token{TokenType::Dot})) {
            throw MolangSyntaxError("Expected dot after resource", token.position);
        }
        const auto ident_token = this->tokens.next_value();

        if (!ident_token.check_types(tokens_which_could_be_names)) {
            throw MolangSyntaxError("Expected identifier after resource Type", ident_token.position);
        }

        auto identifier = ast::IdentifierLiteral(ident_token, this->source);

        returns = std::move(std::make_unique<ast::ResourceExpression>(token.position, token.size,
                                                                      static_cast<ResourceKind>(token.type),
                                                                      std::move(identifier)));

        return true;
    }

    bool MolangAstGenerator::try_parse_array(ast::ExpressionPtr &returns) {
        if (const auto token_temp = this->tokens.next_value(Token{TokenType::Array}); token_temp.has_value()) {
            const auto token = token_temp.value();
            if (!this->tokens.next(Token{TokenType::Dot})) {
                throw MolangSyntaxError("Expected dot after array", token.position);
            }

            ast::IdentifierLiteral ident{};
            if (const auto identifier_temp = this->tokens.next_value(tokens_which_could_be_names_constructed);
                identifier_temp.has_value()) {
                ident = std::move(ast::IdentifierLiteral(identifier_temp.value(), this->source));
            } else {
                throw MolangSyntaxError("Expected identifier after array Type", token.position);
            }

            if (!this->tokens.next(Token{TokenType::LeftBracket})) {
                throw MolangSyntaxError("Expected left bracket", token.position);
            }

            auto index_expression = this->build_expression_recursive(0);

            if (!this->tokens.next(Token{TokenType::RightBracket})) {
                throw MolangSyntaxError("Expected right bracket", token.position);
            }

            returns = std::move(
                std::make_unique<ast::ArrayAccess>(token.position, token.size, std::move(ident),
                                                   std::move(index_expression)));
            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_parse_loop(ast::ExpressionPtr &returns) {
        if (const auto temp_token = this->tokens.next_value(Token{TokenType::Loop}); temp_token.has_value()) {
            const auto token = temp_token.value();

            if (!this->tokens.next(Token{TokenType::LeftParen})) {
                throw MolangSyntaxError("Expected left paren after loop", token.position);
            }

            auto count_expression = this->build_expression_recursive(0);
            if (!count_expression) {
                throw MolangSyntaxError("Expected Expression in count", token.position);
            }

            if (!this->tokens.next(Token{TokenType::Comma})) {
                throw MolangSyntaxError("Expected Comma after count", token.position);
            }

            auto loop_expression = this->build_block();

            if (!loop_expression) {
                throw MolangSyntaxError("Expected Expression in loop", token.position);
            }

            if (!this->tokens.next(Token{TokenType::RightParen})) {
                throw MolangSyntaxError("Expected right paren after expression", token.position);
            }

            returns = std::make_unique<ast::LoopExpression>(token.position, token.size, std::move(count_expression),
                                                            std::move(loop_expression.value()));

            return true;
        }

        return false;
    }

    bool MolangAstGenerator::try_parse_for_each(ast::ExpressionPtr &returns) {
        if (const auto temp_token = this->tokens.next_value(Token{TokenType::ForEach}); temp_token.has_value()) {
            const auto token = temp_token.value();

            if (!this->tokens.next(Token{TokenType::LeftParen})) {
                throw MolangSyntaxError("Expected left paren", token.position);
            }

            auto variable = this->build_variable();

            if (!variable) {
                throw MolangSyntaxError("Expected variable declaration", token.position);
            }

            if (!this->tokens.next(Token{TokenType::Comma})) {
                throw MolangSyntaxError("Expected Comma after variable declaration", token.position);
            }

            auto array_fetch_expression = this->build_expression_recursive(0);

            if (!array_fetch_expression) {
                throw MolangSyntaxError("Expected Expression in array Type", token.position);
            }

            if (!this->tokens.next(Token{TokenType::Comma})) {
                throw MolangSyntaxError("Expected Comma after array Type", token.position);
            }

            auto for_each_code = this->build_block();

            if (!for_each_code) {
                throw MolangSyntaxError("Expected Expression in for_each", token.position);
            }

            if (!this->tokens.next(Token{TokenType::RightParen})) {
                throw MolangSyntaxError("Expected right paren", token.position);
            }

            returns = std::move(std::make_unique<ast::ForEachExpression>(
                token.position, token.size, std::move(variable.value()), std::move(array_fetch_expression),
                std::move(for_each_code.value())));

            return true;
        }
        return false;
    }

    bool MolangAstGenerator::try_parse_keyword(ast::ExpressionPtr &returns) {
        if (const auto temp_token = this->tokens.next_value(Token{TokenType::Break}); temp_token.has_value()) {
            const auto token = temp_token.value();
            returns = std::move(std::make_unique<ast::BreakNode>(token.position, token.size));
            return true;
        }

        if (const auto temp_token = this->tokens.next_value(Token{TokenType::Continue}); temp_token.has_value()) {
            const auto token = temp_token.value();
            returns = std::move(std::make_unique<ast::ContinueNode>(token.position, token.size));
            return true;
        }

        if (const auto temp_token = this->tokens.next_value(Token{TokenType::This}); temp_token.has_value()) {
            const auto token = temp_token.value();
            returns = std::move(std::make_unique<ast::ThisNode>(token.position, token.size));
            return true;
        }

        return false;
    }

    bool MolangAstGenerator::try_parse_return(ast::ExpressionPtr &returns) {
        if (const auto temp_token = this->tokens.next_value(Token{TokenType::Return}); temp_token.has_value()) {
            const auto token = temp_token.value();
            auto expr = this->build_expression_recursive(0); // We don't care if this is a nullptr
            returns = std::move(std::make_unique<ast::ReturnNode>(token.position, token.size, std::move(expr)));
            return true;
        }

        return false;
    }


    // This function builds any "complex" expressions, such as a binary operation since that needs the last operation
    // NOLINTNEXTLINE
    std::pair<ast::ExpressionPtr, bool> MolangAstGenerator::next_expression(
        ast::ExpressionPtr &&lhs, const uint8_t min_binding_power) {
        try {
            if (ast::ExpressionPtr ptr; try_parse_branch(std::move(lhs), ptr)) {
                return std::make_pair(std::move(ptr), true);
            } else {
                lhs = std::move(ptr); // ptr is equal to what lhs was
            }

            if (std::pair<ast::ExpressionPtr, bool> value; try_parse_binary(std::move(lhs), value, min_binding_power))
                return value;
        } catch (std::out_of_range &) {
            // swallow the error since this just means we hit the end of the tree
        }

        return {std::move(lhs), false};
    }

    bool MolangAstGenerator::token_is_bin_op(const Token &token) {
        constexpr auto bin_ops = std::array{
            BinaryOp::Equality,
            BinaryOp::Inequality,
            BinaryOp::LessThan,
            BinaryOp::LessEqualThan,
            BinaryOp::GreaterThan,
            BinaryOp::GreaterEqualThan,
            BinaryOp::Addition,
            BinaryOp::Subtraction,
            BinaryOp::Multiplication,
            BinaryOp::Division,
            BinaryOp::Or,
            BinaryOp::And,
            BinaryOp::Coalesce,
        };

        return std::ranges::contains(bin_ops, static_cast<BinaryOp>(token.type));
    }

    bool MolangAstGenerator::token_is_unary(const Token &token) {
        return std::ranges::contains(unary_ops, static_cast<UnaryOp>(token.type));
    }

    bool MolangAstGenerator::token_is_call(const Token &token) {
        return std::ranges::contains(call_ops, static_cast<CallType>(token.type));
    }

    bool MolangAstGenerator::token_is_resource(const Token &token) {
        return std::ranges::contains(resource_tokens, token.type);
    }
} // molar
