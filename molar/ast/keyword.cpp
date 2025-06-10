//
// Created by Akashic on 5/28/2025.
//

#include "keyword.hpp"

#include "expression.hpp"
#include "internal/variable_manager.hpp"
#include <ostream>

namespace molar::ast {
    void ReturnNode::visit_node(class AstVisitor &visitor) {
        if (visitor.visit_return(*this)) {
            this->value->visit_node(visitor);
        }
    }

    void ReturnNode::print(std::ostream &out, const uint32_t index) {
        Expression::print(out, index);
        if (!this->value) {
            return;
        }
        out << "\n";
        Expression::print_util_tab(out, index);
        out << "return expression:\n";
        this->value->print(out, index + 1);
    }
} // molar
