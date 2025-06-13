//
// Created by Akashic on 5/25/2025.
//

#include "variable.hpp"
#include <array>

#include "ast_visitor.hpp"
#include "molang_error.hpp"
#include <ostream>

namespace molar::ast {
    VariableId::VariableId(
        const Token& token, const molar_impl::SourceBuffer& buffer, TokenBuffer& token_buffer
    )
        : IdentifierLiteral(token, buffer) {
        // The reason we need to do this convoluted check is because technically
        // v.math.context.continue.for_each is a valid variable
        if (!token.check_types(tokens_which_could_be_names)) {
            throw MolangSyntaxError("Expected identifier", token.position);
        }

        if (token_buffer.next(Token(TokenType::Dot))) {
            this->child_id = std::move(
                std::make_unique<VariableId>(token_buffer.next_value(), buffer, token_buffer)
            );
        }
    }

    VariableReference::VariableReference(
        const Token& token, const molar_impl::SourceBuffer& buffer, TokenBuffer& token_buffer
    )
        : Expression(token, AstKind::VariableReference) {
        switch (token.type) {
        case TokenType::Context: {
            this->decl_type = VariableDeclarationType::Context;
            break;
        };
        case TokenType::Variable: {
            this->decl_type = VariableDeclarationType::Var;
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

        this->variable_id =
            std::move(VariableId(token_buffer.next_value(), buffer, token_buffer));
    }

    void VariableReference::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << VariableReference::var_decl_type_to_string(this->decl_type);

        const VariableId* next_id = &this->variable_id;

        while (next_id) {
            out << "." << next_id->value;
            next_id = next_id->child_id.get();
        }

        out << '\n';
    }

    std::string VariableReference::var_decl_type_to_string(VariableDeclarationType type) {
        switch (type) {
        case VariableDeclarationType::Temp:
            return "Temp";
        case VariableDeclarationType::Var:
            return "Var";
        case VariableDeclarationType::Context:
            return "Context";
        default:
            return "Unknown";
        }
    }

    std::string VariableReference::var_decl_type_to_string_short(VariableDeclarationType type) {
        switch (type) {
        case VariableDeclarationType::Temp:
            return "t";
        case VariableDeclarationType::Var:
            return "v";
        case VariableDeclarationType::Context:
            return "x";
        default:
            return "u";
        }
    }

    std::string VariableReference::build_access_string() const {
        std::string start = VariableReference::var_decl_type_to_string(this->decl_type);

        const VariableId* next_id = &this->variable_id;

        while (next_id) {
            start   += "." + next_id->value;
            next_id  = next_id->child_id.get();
        }

        return start;
    }

    std::pair<std::string, std::string> VariableReference::build_lookup_names() const {
        if (this->is_struct()) {
            std::string full_id = {};
            std::string structure_id =
                VariableReference::var_decl_type_to_string_short(this->decl_type);

            const auto* id = &this->get_raw_id();

            while (id) {
                full_id.append(id->get_value());
                if (id->get_child() != nullptr) {
                    structure_id.append(id->get_value());
                }
                id = id->get_child().get();
            }

            return {full_id, structure_id};
        } else {
            return {this->get_raw_id().get_value(), std::string{}};
        }
    }

    void VariableReference::visit_node(class AstVisitor& visitor) {
        (void)visitor.visit_variable(*this);
    }

    void VariableAssign::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        this->variable.print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "Expression:\n";
        this->expression->print(out, index + 1);
    }

    void VariableAssign::visit_node(class AstVisitor& visitor) {

        if (visitor.visit_assignment(*this)) {
            this->variable.visit_node(visitor);
            this->expression->visit_node(visitor);
        }
    }
} // namespace molar::ast
