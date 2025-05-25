//
// Created by Akashic on 5/25/2025.
//

#include "literal.hpp"

namespace molar::ast {
    BoolLiteral::BoolLiteral(const Token &token, const molar_impl::SourceBuffer &buffer) : ExpressionNode(
        token, AstKind::BooleanLiteral) {
        this->value = buffer.slice_from_source(token.position, token.size) == "true";
    }

    StringLiteral::StringLiteral(const Token &token, const molar_impl::SourceBuffer &buffer) : ExpressionNode(
        token, AstKind::StringLiteral) {
        token.assert_type(TokenType::String);
        this->value = buffer.slice_from_source(token.position + 1, token.size - 2);
    }

    NumericLiteral::NumericLiteral(const Token &token, const molar_impl::SourceBuffer &buffer) : ExpressionNode(
        token, AstKind::NumericLiteral) {
        token.assert_type(TokenType::Number);
        // This std::string construction shouldn't be that expensive since we can store up to 16 bytes with SSO on MSVC
        // Meaning no heap allocations for most cases
        this->value = std::stof(std::string(buffer.slice_from_source(token.position, token.size)));
    }

    IdentifierLiteral::IdentifierLiteral(const Token &token, const molar_impl::SourceBuffer &buffer) : ExpressionNode(
        token, AstKind::IdentifierLiteral) {
        token.assert_type(TokenType::Identifier);
        this->value = std::string(buffer.slice_from_source(token.position, token.size));
    }
}
