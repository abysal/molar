//
// Created by Akashic on 5/30/2025.
//

#include "ast_visitor.hpp"

#include "variable.hpp"

namespace molar::ast {
    void AstVisitor::visit() {
        this->root_expression.visit_node(*this);
    }
} // molar
