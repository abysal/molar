//
// Created by Akashic on 5/25/2025.
//

#ifndef HPP
#define HPP
#include "expression.hpp"
#include "internal/source_buffer.hpp"
#include "internal/variable_manager.hpp"

namespace molar::ast {
    class BoolLiteral : public ExpressionNode, public VariableManager<bool> {
    public:
        BoolLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);
    };

    class StringLiteral : public ExpressionNode, public VariableManager<std::string> {
    public:
        StringLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);
    };

    class NumericLiteral : public ExpressionNode, public VariableManager<float> {
    public:
        NumericLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);
    };

    class IdentifierLiteral : public ExpressionNode, public VariableManager<std::string> {
    public:
        IdentifierLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);
    };
}

#endif //HPP
