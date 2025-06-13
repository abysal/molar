//
// Created by Akashic on 5/25/2025.
//

#ifndef VARIABLE_MANAGER_HPP
#define VARIABLE_MANAGER_HPP

namespace molar::ast {
    template <typename VariableType> class VariableManager {
    public:
        const VariableType& get_value() const { return this->value; }

        VariableType& get_value() { return this->value; }

        VariableManager() = default;

        explicit VariableManager(VariableType t) { this->value = t; }

    protected:
        VariableType value{};
    };
} // namespace molar::ast

#endif // VARIABLE_MANAGER_HPP
