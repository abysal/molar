//
// Created by Akashic on 5/25/2025.
//

#ifndef MOLANG_PARSER_HPP
#define MOLANG_PARSER_HPP
#include <format>
#include <stdexcept>
#include <vector>

#include "molang_token.hpp"
#include "internal/source_buffer.hpp"

namespace molar {
    class MolangTokenizer {
    public:
        struct ParserOptions {
        };

    public:
        explicit MolangTokenizer(std::string &&string, const ParserOptions options = ParserOptions{}) : buffer{
                std::forward<std::string>(string)
            }, options(options) {
        }


        std::vector<Token> parse_tokens();

        void print_tokens(const std::vector<Token> &tokens, bool pretty, std::ostream &location) const;

    private:
        std::optional<Token> parse_identifier();

        std::optional<Token> parse_number();

        std::optional<Token> parse_string();

    private:
        molar_impl::SourceBuffer buffer;
        ParserOptions options{};
    };
} // molar

#endif //MOLANG_PARSER_HPP
