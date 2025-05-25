//
// Created by Akashic on 5/25/2025.
//

#include "variable.hpp"
#include <array>

#include "molang_error.hpp"

namespace molar::ast {
    VariableId::VariableId(const Token &token, const molar_impl::SourceBuffer &buffer,
                           TokenBuffer &token_buffer) : IdentifierLiteral(token, buffer) {
        if (token != TokenType::Identifier) {
            throw MolangSyntaxError("Expected identifier", token.position);
        }

        if (token_buffer.next(Token(TokenType::Dot))) {
            this->child_id = std::move(std::make_unique<VariableId>(token_buffer.next_value(), buffer, token_buffer));
        }
    }

    Variable::Variable(const Token &token, const molar_impl::SourceBuffer &buffer,
                       TokenBuffer &token_buffer) : ExpressionNode(
        token, AstKind::VariableDeclaration) {
        switch (token.type) {
            case TokenType::Context: {
                this->decl_type = VariableDeclarationType::Context;
                break;
            };
            case TokenType::Identifier: {
                this->decl_type = VariableDeclarationType::Temp;
                break;
            };
            case TokenType::Temporary: {
                this->decl_type = VariableDeclarationType::Temp;
                break;
            };
            default: {
                throw std::logic_error("Invalid variable type found");
            }
        }

        if (!token_buffer.next(Token{TokenType::Dot})) {
            throw MolangSyntaxError("Expected dot after variable declaration", token.position);
        }

        this->variable_id = std::move(VariableId(token_buffer.next_value(), buffer, token_buffer));
    }
} // molar
