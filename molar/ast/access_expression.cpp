//
// Created by Akashic on 5/28/2025.
//

#include "access_expression.hpp"
#include "ast_visitor.hpp"
#include "expression.hpp"
#include "keyword.hpp"
#include "variable.hpp"
#include <ostream>

namespace molar::ast {
    std::string CallExpression::build_full_name() const {
        return std::format(
            "{}{}", this->get_function_id().get_value(),
            this->get_call_type() == CallType::Math ? "m" : "q"
        );
    }
    void CallExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(static_cast<TokenType>(this->call_type)) << "."
            << this->function_id.get_value() << "\n";

        Expression::print_util_tab(out, index);
        if (!this->arguments.empty()) {
            out << "(\n";

            for (const auto& expression : this->arguments) {
                expression->print(out, index + 1);
                Expression::print_util_tab(out, index + 1);
                out << ",\n";
            }

            Expression::print_util_tab(out, index);
            out << ")\n";
        } else {
            out << "()\n";
        }
    }

    void CallExpression::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_call(*this)) {
            for (auto& expr : this->arguments) {
                expr->visit_node(visitor);
            }
        }
    }

    void ResourceExpression::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(static_cast<TokenType>(this->resource_kind)) << "."
            << this->resource_id.get_value() << "\n";
    }

    std::string ResourceExpression::compile_id() {
        return std::format(
            "{}.{}", to_string(static_cast<TokenType>(this->resource_kind)),
            this->resource_id.get_value()
        );
    }

    void ResourceExpression::visit_node(class AstVisitor& visitor) {
        (void)visitor.visit_resource(*this);
    }

    void ArrayAccess::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(TokenType::Array) << "." << this->array_id.get_value() << "[\n";
        this->index_expression->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "]\n";
    }

    void ArrayAccess::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_array_access(*this)) {
            this->index_expression->visit_node(visitor);
        }
    }

    void ArrowAccess::print(std::ostream& out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(TokenType::Arrow) << "\n";
        Expression::print_util_tab(out, index);
        out << "LHS:\n";
        this->lhs->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "RHS:\n";
        this->rhs->print(out, index + 1);
    }

    void ArrowAccess::visit_node(class AstVisitor& visitor) {
        if (visitor.visit_arrow_access(*this)) {
            this->lhs->visit_node(visitor);
            this->rhs->visit_node(visitor);
        }
    }
} // namespace molar::ast
