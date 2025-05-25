//
// Created by Akashic on 5/25/2025.
//

#ifndef VARIABLE_HPP
#define VARIABLE_HPP
#include <memory>

#include "expression.hpp"
#include "literal.hpp"
#include "internal/source_buffer.hpp"
#include "internal/variable_manager.hpp"

namespace molar::ast {
    enum class VariableDeclarationType {
        Temp,
        Var,
        Context
    };

    class VariableId : public IdentifierLiteral {
    public:
        VariableId(const Token &token, const molar_impl::SourceBuffer &buffer, TokenBuffer &token_buffer);

        bool is_end() const { return this->child_id == nullptr; }

    private:
        std::unique_ptr<VariableId> child_id{}; // Handles cases where we have nested members
        // Such as v.foo.bar
        // we would be foo
        // and the child would be bar

        friend class Variable;

    private:
        VariableId() = default;
    };

    class Variable : public ExpressionNode {
    public:
        Variable(const Token &token, const molar_impl::SourceBuffer &buffer, TokenBuffer &token_buffer);

    protected:
        VariableDeclarationType decl_type{};
        VariableId variable_id{};
    };
} // molar::ast

#endif //VARIABLE_HPP
