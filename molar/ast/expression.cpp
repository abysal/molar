//
// Created by Akashic on 5/28/2025.
//
#include "expression.hpp"
#include "ast_visitor.hpp"
#include <ostream>

namespace molar::ast {
    std::string ast_kind_to_string(const AstKind kind) {
        switch (kind) {
        case AstKind::BooleanLiteral:
            return "BooleanLiteral";
        case AstKind::NumericLiteral:
            return "NumericLiteral";
        case AstKind::StringLiteral:
            return "StringLiteral";
        case AstKind::IdentifierLiteral:
            return "IdentifierLiteral";
        case AstKind::VariableReference:
            return "VariableReference";
        case AstKind::ParenthesizedExpression:
            return "ParenthesizedExpression";
        case AstKind::BlockExpression:
            return "BlockExpression";
        case AstKind::BinaryExpression:
            return "BinaryExpression";
        case AstKind::UnaryExpression:
            return "UnaryExpression";
        case AstKind::TernaryExpression:
            return "TernaryExpression";
        case AstKind::ConditionalExpression:
            return "ConditionalExpression";
        case AstKind::AssignmentExpression:
            return "AssignmentExpression";
        case AstKind::ResourceExpression:
            return "ResourceExpression";
        case AstKind::ArrayAccessExpression:
            return "ArrayAccessExpression";
        case AstKind::ArrowAccessExpression:
            return "ArrowAccessExpression";
        case AstKind::CallExpression:
            return "CallExpression";
        case AstKind::LoopExpression:
            return "LoopExpression";
        case AstKind::ForEachExpression:
            return "ForEachExpression";
        case AstKind::Break:
            return "Break";
        case AstKind::Continue:
            return "Continue";
        case AstKind::This:
            return "This";
        case AstKind::Return:
            return "Return";
        default:
            return "unk";
        }
        std::unreachable();
    }

    void Expression::print(std::ostream& out, const uint32_t index) {
        Expression::print_util_tab(out, index);
        out << ast_kind_to_string(this->type) << ": ";
    }

    void RawExpression::print_util_tab(std::ostream& out, const uint32_t indent_level) {
        out << std::string(indent_level * 4, ' ');
    }

    void ParenthesizedExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        for (auto& expr : this->expressions) {
            expr->print(out, index + 1);
            out << "\n";
        }
    }

    void ParenthesizedExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_parenthesized(*this)) {
            for (auto& expr : this->expressions) {
                expr->visit_node(visitor);
            }
        }
    }

    void BlockExpression::print(std::ostream& out, const uint32_t index) {
        ParenthesizedExpression::print(out, index);
    }

    void BlockExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_block(*this)) {
            for (auto& expr : this->expressions) {
                expr->visit_node(visitor);
            }
        }
    }

    void BinaryExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << std::format(
            "Operator: {}\n", to_symbol_string(static_cast<TokenType>(this->operation))
        );
        Expression::print_util_tab(out, index);
        out << "Left:\n";
        this->left->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "Right:\n";
        this->right->print(out, index + 1);
    }

    void BinaryExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_binary(*this)) {
            this->left->visit_node(visitor);
            this->right->visit_node(visitor);
        }
    }

    void UnaryExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << "\n";
        Expression::print_util_tab(out, index);
        out << std::format(
            "Operator: {}\n", to_symbol_string(static_cast<TokenType>(this->operation))
        );
        Expression::print_util_tab(out, index);
        out << "Expression:\n";
        this->expression->print(out, index + 1);
    }

    void UnaryExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_unary(*this)) {
            this->expression->visit_node(visitor);
        }
    }
} // namespace molar::ast
