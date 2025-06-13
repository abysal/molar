//
// Created by Akashic on 6/10/2025.
//

#include "ast_rebuilder.hpp"

#include "ast/access_expression.hpp"
#include "ast/ast_replacer.hpp"
#include "ast/controll_flow.hpp"
#include "execution_nodes/pre_allocated.hpp"
#include "execution_nodes/pre_allocated_variable.hpp"
#include "internal/checked_down_cast.hpp"
#include "processed_ast_visitor.hpp"

namespace molar::exec {
    using namespace molar::ast;

    class Replacer final : public AstReplaceVisitor {
    public:
        ~Replacer() override = default;

        explicit Replacer(AstCollectorState& state) : state(state) {}

        [[nodiscard]] ast::PreAllocatedVariable
        build_variable(const VariableReference& expression) const {
            auto [full_id, _structure_id] = expression.build_lookup_names();
            const auto variable           = this->state.get_state(expression.get_access_type())
                                      .variable_index_map.at(full_id);

            return ast::PreAllocatedVariable{variable};
        }

        [[nodiscard]] RawExpressionPtr rebuild_assignment(VariableAssign& expression) const {
            return std::make_unique<ast::PreAllocatedVariableAssign>(
                this->build_variable(expression.get_variable()).get_value(),
                std::move(expression.get_expression())
            );
        }

        [[nodiscard]] RawExpressionPtr rebuild_call(CallExpression& expression) const {
            const auto id    = expression.build_full_name();
            const auto index = this->state.func_call_state.name_to_id.at(id);

            return std::make_unique<ast::PreAllocatedCall>(
                index, std::move(expression.get_arguments())
            );
        }

        [[nodiscard]] RawExpressionPtr rebuild_variable(const VariableReference& expression
        ) const {
            return std::make_unique<ast::PreAllocatedVariable>(
                std::move(this->build_variable(expression))
            );
        }

        [[nodiscard]] RawExpressionPtr rebuild_for_loop(ForEachExpression& expression) const {
            const auto var = this->build_variable(expression.get_storage());
            return std::make_unique<ast::PreAllocatedForLoop>(
                var.get_value(), std::move(expression.get_array_fetch_expression()),
                std::move(expression.get_loop_expression())
            );
        }

        RawExpressionPtr replace(RawExpressionPtr&& expression) override {
            if (expression->get_type() == AstKind::VariableReference) {
                return this->rebuild_variable(
                    molar::details::type_asserted_cast<VariableReference&>(*expression)
                );
            }

            if (expression->get_type() == AstKind::AssignmentExpression) {
                return this->rebuild_assignment(
                    molar::details::type_asserted_cast<VariableAssign&>(*expression)
                );
            }

            if (expression->get_type() == AstKind::CallExpression) {
                return this->rebuild_call(
                    molar::details::type_asserted_cast<CallExpression&>(*expression)
                );
            }

            if (expression->get_type() == AstKind::ForEachExpression) {
                return this->rebuild_for_loop(
                    molar::details::type_asserted_cast<ForEachExpression&>(*expression)
                );
            }

            return std::move(expression);
        }

    private:
        AstCollectorState& state;
    };

    void Rebuilder::rebuild() const {
        for (auto& expression : this->ast.get_expressions()) {
            this->replace_in_expression(expression);
        }
    }

    void Rebuilder::replace_in_expression(RawExpressionPtr& expression) const {
        auto replacer = Replacer{this->ast_information};

        AstReplacer(expression).visit_all<ast::details::ProcessedAstVisitorReplacer>(replacer);
    }
} // namespace molar::exec
