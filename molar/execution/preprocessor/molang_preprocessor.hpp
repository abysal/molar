//
// Created by Akashic on 5/30/2025.
//

#ifndef MOLANG_PREPROCESSOR_HPP
#define MOLANG_PREPROCESSOR_HPP
#include <unordered_map>

#include "molang_ast_generator.hpp"

namespace molar::exec {
    struct AstCollectorState {
        struct NamedIdMap {
            std::unordered_map<std::string, uint32_t> name_to_id{};
            std::vector<std::string>                  id_to_name{};
            uint32_t                                  next_id{0};

            uint32_t id() { return this->next_id++; }

            uint32_t add_id(std::string&& var_id);
        };

        struct VariableState {
            struct MolangStructInfo {
                std::string           name{};
                std::vector<uint32_t> children_id{};
            };

            std::unordered_map<std::string, uint32_t>         variable_index_map{};
            std::unordered_map<uint32_t, std::string>         index_variable_map{};
            std::unordered_map<std::string, MolangStructInfo> struct_info_map{};
            uint32_t                                          variable_index{0};

            uint32_t id() { return this->variable_index++; }

            uint32_t add_variable(std::string&& name);

            MolangStructInfo& create_or_get_struct(const std::string& name);
        };

        VariableState variables{};
        VariableState temp_variables{};
        VariableState array_state{};
        NamedIdMap    func_call_state{};

        VariableState& get_state(ast::VariableDeclarationType type);
    };

    ///@brief This class is responsible with the processing of the raw molang AST into a more
    /// efficient state
    class MolangPreprocessor {
    public:
        class ProcessedTree {
        public:
            [[nodiscard]] size_t get_variable_slot_count() const {
                return this->variable_slot_count;
            };
            [[nodiscard]] size_t get_resource_slot_count() const {
                return this->resource_slot_count;
            };
            [[nodiscard]] size_t get_array_slot_count() const {
                return this->array_slot_count;
            };

        private:
            size_t variable_slot_count{};
            size_t temp_slot_count{};
            size_t context_slot_count{};
            size_t resource_slot_count{};
            size_t array_slot_count{};
            friend class MolangPreprocessor;
        };

    public:
        explicit MolangPreprocessor(MolangAstGenerator::MolarAst&& ast) : ast(std::move(ast)) {}

        void process();

        void rebuild();

        MolangAstGenerator::MolarAst consume_ast() { return std::move(this->ast); }

    private:
        MolangAstGenerator::MolarAst ast;
        AstCollectorState            collection_state{};
    };
} // namespace molar::exec

#endif // MOLANG_PREPROCESSOR_HPP
