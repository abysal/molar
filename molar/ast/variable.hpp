//
// Created by Akashic on 5/25/2025.
//

#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include <memory>

#include "expression.hpp"
#include "literal.hpp"
#include "internal/source_buffer.hpp"

namespace molar::ast {
    enum class VariableDeclarationType {
        Temp,
        Var,
        Context
    };

    class VariableId final : public IdentifierLiteral {
    public:
        VariableId(const Token &token, const molar_impl::SourceBuffer &buffer, TokenBuffer &token_buffer);

        bool is_end() const { return this->child_id == nullptr; }

        VariableId(VariableId &&) noexcept = default;

        VariableId(const VariableId &) = delete; // still deleted due to unique_ptr
        VariableId &operator=(const VariableId &) = delete;

        VariableId &operator=(VariableId &&) noexcept = default;

    private:
        std::unique_ptr<VariableId> child_id{}; // Handles cases where we have nested members
        // Such as v.foo.bar
        // we would be foo
        // and the child would be bar

        friend class Variable;

    private:
        VariableId() = default;

    public:
        void print(std::ostream &out, const uint32_t index) override {
        }

        ~VariableId() override = default;
    };

    class Variable final : public Expression {
    public:
        Variable(const Token &token, const molar_impl::SourceBuffer &buffer, TokenBuffer &token_buffer);

        ~Variable() override = default;

        Variable(Variable &&) noexcept = default;

        void print(std::ostream &out, uint32_t index) override;

        static std::string var_decl_type_to_string(VariableDeclarationType type);

    protected:
        VariableDeclarationType decl_type{};
        VariableId variable_id{};
    };

    class VariableAssign final : public Expression {
    public:
        VariableAssign(Variable &&variable, ExpressionPtr &&expression) : Expression(variable.get_position(),
                                                                              variable.get_size(),
                                                                              AstKind::AssignmentExpression),
                                                                          variable(std::move(variable)),
                                                                          expression(std::move(expression)) {
        }

        void print(std::ostream &out, const uint32_t index) override;

        [[nodiscard]] Variable &get_variable() {
            return this->variable;
        }

        [[nodiscard]] ExpressionPtr &get_expression() {
            return this->expression;
        }

    protected:
        Variable variable;
        ExpressionPtr expression;
    };
} // molar::ast

#endif //VARIABLE_HPP
