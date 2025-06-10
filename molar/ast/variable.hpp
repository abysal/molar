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

        [[nodiscard]] bool is_end() const { return this->child_id == nullptr; }

        VariableId(VariableId &&) noexcept = default;

        VariableId(const VariableId &) = delete; // still deleted due to unique_ptr
        VariableId &operator=(const VariableId &) = delete;

        VariableId &operator=(VariableId &&) noexcept = default;


        [[nodiscard]] const std::unique_ptr<VariableId> &get_child() const {
            return this->child_id;
        }

    private:
        std::unique_ptr<VariableId> child_id{}; // Handles cases where we have nested members
        // Such as v.foo.bar
        // we would be foo
        // and the child would be bar

        friend class VariableReference;

    private:
        VariableId() = default;

    public:
        void print(std::ostream &out, const uint32_t index) override {
        }

        ~VariableId() override = default;
    };

    class VariableReference final : public Expression {
    public:
        VariableReference(const Token &token, const molar_impl::SourceBuffer &buffer, TokenBuffer &token_buffer);

        ~VariableReference() override = default;

        VariableReference(VariableReference &&) noexcept = default;

        void print(std::ostream &out, uint32_t index) override;

        static std::string var_decl_type_to_string(VariableDeclarationType type);

        [[nodiscard]] std::string build_access_string() const;

        [[nodiscard]] VariableDeclarationType get_access_type() const {
            return this->decl_type;
        }

        [[nodiscard]] const VariableId &get_raw_id() const {
            return this->variable_id;
        }

        void visit_node(class AstVisitor &visitor) override;

    protected:
        VariableDeclarationType decl_type{};
        VariableId variable_id{};
    };

    class VariableAssign final : public Expression {
    public:
        VariableAssign(VariableReference &&variable, RawExpressionPtr &&expression) : Expression(
                variable.get_position(),
                variable.get_size(),
                AstKind::AssignmentExpression),
            variable(std::move(variable)),
            expression(std::move(expression)) {
        }

        void print(std::ostream &out, uint32_t index) override;

        [[nodiscard]] VariableReference &get_variable() {
            return this->variable;
        }

        [[nodiscard]] RawExpressionPtr &get_expression() {
            return this->expression;
        }

        void visit_node(class AstVisitor &visitor) override;

    protected:
        VariableReference variable;
        RawExpressionPtr expression;
    };
} // molar::ast

#endif //VARIABLE_HPP
