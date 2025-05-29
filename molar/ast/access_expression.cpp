//
// Created by Akashic on 5/28/2025.
//

#include "access_expression.hpp"

namespace molar::ast {
    void CallExpression::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(static_cast<TokenType>(this->call_type)) << "." << this->function_id.get_value() << "\n";

        Expression::print_util_tab(out, index);
        if (!this->arguments.empty()) {
            out << "(\n";

            for (const auto &expression: this->arguments) {
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

    void ResourceExpression::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(static_cast<TokenType>(this->resource_kind)) << "." << this->resource_id.get_value() << "\n";
    }

    void ArrayAccess::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(TokenType::Array) << "." << this->array_id.get_value() << "[\n";
        this->index_expression->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "]\n";
    }

    void ArrowAccess::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        out << to_string(TokenType::Arrow) << "\n";
        Expression::print_util_tab(out, index);
        out << "LHS:\n";
        this->lhs->print(out, index + 1);
        Expression::print_util_tab(out, index);
        out << "RHS:\n";
        this->rhs->print(out, index + 1);
    }
} // molar
