//
// Created by Akashic on 5/25/2025.
//

#include "literal.hpp"
#include <ostream>

#include "ast_visitor.hpp"

namespace molar::ast {
    BoolLiteral::BoolLiteral(const Token& token, const molar_impl::SourceBuffer& buffer)
        : Expression(token, AstKind::BooleanLiteral) {
        this->value = buffer.slice_from_source(token.position, token.size) == "true";
    }

    void BoolLiteral::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << std::boolalpha << this->value << "\n";
    }

    void BoolLiteral::visit_node(class AstVisitor& visitor) { visitor.visit_bool(*this); }

    StringLiteral::StringLiteral(const Token& token, const molar_impl::SourceBuffer& buffer)
        : Expression(token, AstKind::StringLiteral) {
        token.assert_type(TokenType::String);
        this->value = buffer.slice_from_source(token.position + 1, token.size - 2);
    }

    void StringLiteral::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << this->value << "\n";
    }

    void StringLiteral::visit_node(class AstVisitor& visitor) { visitor.visit_string(*this); }

    NumericLiteral::NumericLiteral(const Token& token, const molar_impl::SourceBuffer& buffer)
        : Expression(token, AstKind::NumericLiteral) {
        token.assert_type(TokenType::Number);
        // This std::string construction shouldn't be that expensive since we can store up to 16
        // bytes with SSO on MSVC Meaning no heap allocations for most cases
        this->value =
            std::stof(std::string(buffer.slice_from_source(token.position, token.size)));
    }

    void NumericLiteral::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << this->value << "\n";
    }

    void NumericLiteral::visit_node(class AstVisitor& visitor) { visitor.visit_number(*this); }

    IdentifierLiteral::IdentifierLiteral(
        const Token& token, const molar_impl::SourceBuffer& buffer
    )
        : Expression(token, AstKind::IdentifierLiteral) {
        token.assert_type(tokens_which_could_be_names);
        this->value = std::string(buffer.slice_from_source(token.position, token.size));
    }

    void IdentifierLiteral::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << this->value;
    }

    void IdentifierLiteral::visit_node(class AstVisitor& visitor) {
        throw std::logic_error("IdentifierLiteral should not be called");
    }
} // namespace molar::ast
