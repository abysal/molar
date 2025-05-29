//
// Created by Akashic on 5/25/2025.
//

#ifndef LITERAL_HPP
#define LITERAL_HPP
#include "expression.hpp"
#include "internal/source_buffer.hpp"
#include "internal/variable_manager.hpp"

namespace molar::ast {
    class BoolLiteral final : public Expression, public VariableManager<bool> {
    public:
        BoolLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);

        ~BoolLiteral() override = default;

        void print(std::ostream &out, const uint32_t index) override;
    };

    class StringLiteral final : public Expression, public VariableManager<std::string> {
    public:
        StringLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);

        ~StringLiteral() override = default;

        void print(std::ostream &out, const uint32_t index) override;
    };

    class NumericLiteral final : public Expression, public VariableManager<float> {
    public:
        NumericLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);

        ~NumericLiteral() override = default;

        void print(std::ostream &out, const uint32_t index) override;
    };

    class IdentifierLiteral : public Expression, public VariableManager<std::string> {
    public:
        IdentifierLiteral(const Token &token, const molar_impl::SourceBuffer &buffer);

        IdentifierLiteral(IdentifierLiteral &&) noexcept = default;

        IdentifierLiteral &operator=(IdentifierLiteral &&) noexcept = default;

        IdentifierLiteral() = default;

    public:
        void print(std::ostream &out, const uint32_t index) override;
    };
}

#endif //LITERAL_HPP
