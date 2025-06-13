//
// Created by Akashic on 5/25/2025.
//

#ifndef MOLANG_ERROR_HPP
#define MOLANG_ERROR_HPP
#include <format>
#include <stdexcept>

namespace molar {
    class MolangSyntaxError : public std::runtime_error {
    public:
        explicit MolangSyntaxError(const std::string& _Message, const size_t position)
            : runtime_error(std::format("{}. At {}", _Message, position)), position(position) {}

    private:
        size_t position;
    };
} // namespace molar
#endif // MOLANG_ERROR_HPP
