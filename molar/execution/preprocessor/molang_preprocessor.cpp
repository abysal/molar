//
// Created by Akashic on 5/30/2025.
//

#include "molang_preprocessor.hpp"

#include "ast_rebuilder.hpp"

#include <ranges>
#include <unordered_map>
#include <variant>

#include "ast/access_expression.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/expression.hpp"

namespace molar::exec {
    class SlotCollectorInner final : public ast::AstVisitor {
    public:
        explicit SlotCollectorInner(ast::RawExpression& expression, AstCollectorState& state)
            : AstVisitor(expression), state(state) {}

        bool visit_call(class ast::CallExpression& expression) override {
            this->state.func_call_state.add_id(expression.build_full_name());
            return true;
        }

        bool visit_variable(ast::VariableReference& variable) override {
            if (auto [full_id, structure_id] = variable.build_lookup_names();
                structure_id.empty()) {
                this->state.get_state(variable.get_access_type())
                    .add_variable(std::move(full_id));
            } else {
                auto&      state     = this->state.get_state(variable.get_access_type());
                const auto member_id = state.add_variable(std::move(full_id));

                state.create_or_get_struct(structure_id).children_id.emplace_back(member_id);
            }
            return true;
        }

        bool visit_array_access(ast::ArrayAccess& expression) override {
            this->state.array_state.add_variable(
                std::string(expression.get_array_id().get_value())
            );
            return true;
        }

        bool visit_resource(ast::ResourceExpression& expression) override { return true; }

    private:
        AstCollectorState& state;
    };

    class SlotCollector {
    public:
        void collect(const molar::ast::RawExpressionList& exprs) {
            for (auto& expr : exprs) {
                SlotCollectorInner(*expr, this->state).visit();
            }
        }

    public:
        AstCollectorState state{};
    };

    uint32_t AstCollectorState::NamedIdMap::add_id(std::string&& var_id) {
        std::string str = std::move(var_id);

        if (this->name_to_id.contains(str)) {
            return this->name_to_id.at(str);
        }

        const auto id = this->id();

        this->name_to_id.emplace(str, id);
        this->id_to_name.emplace_back(std::move(str));
        return id;
    }

    uint32_t AstCollectorState::VariableState::add_variable(std::string&& name) {
        if (this->variable_index_map.contains(name)) {
            return this->variable_index_map[name];
        }

        const auto id = this->id();

        this->variable_index_map[name] = id;
        this->index_variable_map[id]   = std::move(name);
        return id;
    }

    AstCollectorState::VariableState::MolangStructInfo&
    AstCollectorState::VariableState::create_or_get_struct(const std::string& name) {
        if (this->struct_info_map.contains(name)) {
            return this->struct_info_map[name];
        }
        MolangStructInfo info{
            .name        = name,
            .children_id = {},
        };

        this->struct_info_map[name] = std::move(info);
        return this->struct_info_map[name];
    }

    AstCollectorState::VariableState&
    AstCollectorState::get_state(const ast::VariableDeclarationType type) {
        if (type == ast::VariableDeclarationType::Context) {
            throw std::logic_error("Context variable declaration type not supported");
        }
        if (type == ast::VariableDeclarationType::Temp) {
            return this->temp_variables;
        }
        return this->variables;
    }

    void MolangPreprocessor::process() {
        SlotCollector processor{};
        processor.collect(this->ast.get_expressions());
        this->collection_state = std::move(processor.state);
    }

    void MolangPreprocessor::rebuild() {
        Rebuilder(this->collection_state, this->ast).rebuild();
    }
} // namespace molar::exec
