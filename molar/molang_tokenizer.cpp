//
// Created by Akashic on 5/25/2025.
//

#include "molang_tokenizer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <ostream>

#include "molang_error.hpp"

namespace molar {
    constexpr auto simple_tokens = std::array{
        std::pair{TokenType::LeftParen, '('},   std::pair{TokenType::RightParen, ')'},
        std::pair{TokenType::LeftBrace, '{'},   std::pair{TokenType::RightBrace, '}'},
        std::pair{TokenType::LeftBracket, '['}, std::pair{TokenType::RightBracket, ']'},
        std::pair{TokenType::Assign, '='},      std::pair{TokenType::Not, '!'},
        std::pair{TokenType::Lt, '<'},          std::pair{TokenType::Gt, '>'},
        std::pair{TokenType::Dot, '.'},         std::pair{TokenType::Conditional, '?'},
        std::pair{TokenType::Colon, ':'},       std::pair{TokenType::Semi, ';'},
        std::pair{TokenType::Comma, ','},       std::pair{TokenType::Minus, '-'},
        std::pair{TokenType::Plus, '+'},        std::pair{TokenType::Star, '*'},
        std::pair{TokenType::Slash, '/'}
    };

    constexpr auto single_char_complex = std::array{
        std::pair{TokenType::Query, 'q'},   std::pair{TokenType::Variable, 'v'},
        std::pair{TokenType::Context, 'c'}, std::pair{TokenType::Temporary, 't'},
        std::pair{TokenType::Math, 'm'},
    };

    constexpr auto skip = std::array{
        std::string_view{" "},
        std::string_view{"\r\n"},
        std::string_view{"\n"},
        std::string_view{"\t"},
    };

    constexpr auto multi_char_tokens =
        std::array{// Multi-char symbolic operators
                   std::pair{TokenType::Eq, std::string_view{"=="}},
                   std::pair{TokenType::NotEq, std::string_view{"!="}},
                   std::pair{TokenType::LtEq, std::string_view{"<="}},
                   std::pair{TokenType::GtEq, std::string_view{">="}},
                   std::pair{TokenType::Or, std::string_view{"||"}},
                   std::pair{TokenType::And, std::string_view{"&&"}},
                   std::pair{TokenType::Arrow, std::string_view{"->"}},
                   std::pair{TokenType::NullCoal, std::string_view{"??"}},

                   // Keywords (unambiguous)
                   std::pair{TokenType::True, std::string_view{"true"}},
                   std::pair{TokenType::False, std::string_view{"false"}},
                   std::pair{TokenType::This, std::string_view{"this"}},
                   std::pair{TokenType::Break, std::string_view{"break"}},
                   std::pair{TokenType::Continue, std::string_view{"continue"}},
                   std::pair{TokenType::ForEach, std::string_view{"for_each"}},
                   std::pair{TokenType::Loop, std::string_view{"loop"}},
                   std::pair{TokenType::Return, std::string_view{"return"}},
                   std::pair{TokenType::Temporary, std::string_view{"temp"}},
                   std::pair{TokenType::Context, std::string_view{"context"}}
        };

    constexpr auto case_insensitive_tokens = std::array{
        std::pair{TokenType::Math, std::string_view{"math"}},
        std::pair{TokenType::Query, std::string_view{"query"}},
        std::pair{TokenType::Variable, std::string_view{"variable"}},
        std::pair{TokenType::Geometry, std::string_view{"geometry"}},
        std::pair{TokenType::Material, std::string_view{"material"}},
        std::pair{TokenType::Texture, std::string_view{"texture"}},
        std::pair{TokenType::Array, std::string_view{"array"}}
    };

    std::optional<Token> MolangTokenizer::parse_identifier() {
        try {
            const auto restore    = this->buffer.get_rollback_point();
            const auto first_char = this->buffer.next_char();

            if (std::ranges::contains(std::array{'v', 'q', 't', 'c', 'm'}, first_char)) {
                const auto our_rest = this->buffer.get_rollback_point();
                if (const auto second_char = this->buffer.next_char(); second_char == '.') {
                    this->buffer.apply_rollback_point(restore);
                    return std::nullopt;
                }
                this->buffer.apply_rollback_point(our_rest);
            }

            if (!std::isalpha(first_char) && first_char != '_') {
                this->buffer.apply_rollback_point(restore);
                return std::nullopt;
            }

            size_t ident_size = 1;

            while (this->buffer.has_any()) {
                const auto next_char = this->buffer.peak_char();
                if (!isalnum(next_char) && next_char != '_') {
                    break;
                }
                next_char.apply();
                ident_size++;
            }

            return Token{restore, ident_size, TokenType::Identifier};
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<Token> MolangTokenizer::parse_number() {
        try {
            const auto restore = this->buffer.get_rollback_point();

            if (const auto first_char = this->buffer.next_char(); first_char == '-') {
                const auto inner_rest = this->buffer.get_rollback_point();
                if (!isdigit(this->buffer.next_char())) {
                    this->buffer.apply_rollback_point(restore);
                    return std::nullopt;
                }
                this->buffer.apply_rollback_point(inner_rest);
            } else if (!isdigit(first_char)) {
                this->buffer.apply_rollback_point(restore);
                return std::nullopt;
            }

            size_t number_size = 1;

            while (this->buffer.has_any()) {
                const auto number_char = this->buffer.peak_char();
                if (!isdigit(number_char) && number_char != '.') {
                    if (toupper(number_char) == 'F') {
                        number_char.apply();
                    }

                    break;
                }
                number_char.apply();
                number_size++;
            }

            return Token{restore, number_size, TokenType::Number};
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<Token> MolangTokenizer::parse_string() {
        try {
            const auto position = this->buffer.get_rollback_point();
            if (const auto next_char = this->buffer.peak_char(); next_char != '\'') {
                return std::nullopt;
            } else {
                next_char.apply();
            }

            size_t string_size = 1;
            bool   escaped     = false;

            molar_impl::SourceBuffer::Peak next_char{};

            while (this->buffer.has_any()) {
                next_char = this->buffer.peak_char();
                next_char.apply();

                if (!escaped && next_char == '\'') {
                    string_size++;
                    break;
                }

                if (escaped) {
                    escaped = false;
                }

                if (next_char == '\\') {
                    escaped = true;
                    string_size++;
                }

                string_size++;
            }

            if (next_char == '\'') {
                return Token{position, string_size, TokenType::String};
            }

            throw MolangSyntaxError("Unterminated string", position);
        } catch (std::out_of_range& /*unused */) {
            return std::nullopt;
        }
    }

    std::vector<Token> MolangTokenizer::parse_tokens() {
        std::vector<Token> tokens{};

        while (this->buffer.has_any()) {
            for (const auto text : skip) {
                if (const auto token_start = this->buffer.next(text); token_start.has_value()) {
                    goto end;
                }
            }

            if (const auto token = this->parse_string(); token.has_value()) {
                tokens.emplace_back(token.value());
                goto end;
            }

            for (const auto [token, text] : case_insensitive_tokens) {
                if (const auto token_start = this->buffer.caseless_next(text);
                    token_start.has_value()) {
                    tokens.emplace_back(token_start.value(), text, token);
                    goto end;
                }
            }

            for (const auto [token, text] : multi_char_tokens) {
                if (const auto token_start = this->buffer.next(text); token_start.has_value()) {
                    tokens.emplace_back(token_start.value(), text, token);
                    goto end;
                }
            }

            if (const auto token = this->parse_number(); token.has_value()) {
                tokens.emplace_back(token.value());
                goto end;
            }

            for (const auto [token, text] : simple_tokens) {
                if (const auto token_start = this->buffer.next(text); token_start.has_value()) {
                    tokens.emplace_back(token_start.value(), token);
                    goto end;
                }
            }

            if (const auto token = parse_identifier(); token.has_value()) {
                tokens.emplace_back(token.value());
                goto end;
            }

            for (const auto [token, text] : single_char_complex) {
                if (const auto token_start = this->buffer.next(text); token_start.has_value()) {
                    tokens.emplace_back(token_start.value(), token);
                    goto end;
                }
            }

            // Just skip an unknown character
            try {
                this->buffer.skip(1);
            } catch (...) {}
        end:
            (void)1; // Makes sure the end is valid syntax
        }

        return tokens;
    }

    void MolangTokenizer::print_tokens(
        const std::vector<Token>& tokens, const bool pretty, std::ostream& location
    ) const {
        const auto break_char = pretty ? '\n' : ' ';
        const auto tab        = pretty ? '\t' : ' ';

        for (const auto& token : tokens) {
            location << std::format(
                "Token{{{}{}Position: {},{}{}Value: {},{}{}Type: {}{}}},{}", break_char, tab,
                token.position, break_char, tab,
                this->buffer.slice_from_source(token.position, token.size), break_char, tab,
                to_string(token.type), break_char, break_char
            );
        }
    }
} // namespace molar
