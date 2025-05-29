//
// Created by Akashic on 5/25/2025.
//

#ifndef MOLANG_TOKEN_HPP
#define MOLANG_TOKEN_HPP
#include <cassert>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <string_view>

#include "internal/source_buffer.hpp"

namespace molar {
    enum class TokenType : uint8_t {
        // End of file/input.
        Eof,

        // An identifier (e.g., variable or function name). Must not start with a digit.
        Identifier,

        // A string literal enclosed in single quotes ('example').
        String,

        // A numeric literal. May include a decimal point. An optional 'f' suffix is stripped.
        Number,

        // Left parenthesis: (
        LeftParen,

        // Right parenthesis: )
        RightParen,

        // Left brace: {
        LeftBrace,

        // Right brace: }
        RightBrace,

        // Left bracket: [
        LeftBracket,

        // Right bracket: ]
        RightBracket,

        // Assignment operator: =
        Assign,

        // Logical negation or "not": !
        Not,

        // Equality operator: ==
        Eq,

        // Inequality operator: !=
        NotEq,

        // Less-than operator: <
        Lt,

        // Greater-than operator: >
        Gt,

        // Less-than or equal to: <=
        LtEq,

        // Greater-than or equal to: >=
        GtEq,

        // Logical OR: ||
        Or,

        // Logical AND: &&
        And,

        // Arrow operator: ->
        Arrow,

        // Dot/Member access operator: .
        Dot,

        // Conditional (ternary) operator: ?
        Conditional,

        // Null coalescing operator: ??
        NullCoal,

        // Colon symbol: :
        Colon,

        // Semicolon symbol: ;
        Semi,

        // Comma separator: ,
        Comma,

        // Minus or subtraction operator: -
        Minus,

        // Plus or addition operator: +
        Plus,

        // Multiplication operator: *
        Star,

        // Division operator: /
        Slash,

        // Keyword: temp
        Temporary,

        // Keyword: variable or "v"
        Variable,

        // Keyword: context
        Context,

        // Matches "Math" or "math" or "m"
        Math,

        // Matches "Query" or "query" or "q"
        Query,

        // Matches "Geometry" or "geometry"
        Geometry,

        // Matches "Material" or "material"
        Material,

        // Matches "Texture" or "texture"
        Texture,

        // Matches "Array" or "array"
        Array,

        // Boolean literal: true
        True,

        // Boolean literal: false
        False,

        // Keyword: this (refers to current object/context)
        This,

        // Keyword: break (exit a loop)
        Break,

        // Keyword: continue (skip to next loop iteration)
        Continue,

        // Keyword: for_each (loop construct)
        ForEach,

        // Keyword: loop
        Loop,

        // Keyword: return
        Return,
    };

    enum class BinaryOp : uint8_t {
        /// `==`
        Equality = TokenType::Eq,
        /// `!=`
        Inequality = TokenType::NotEq,
        /// `<`
        LessThan = TokenType::Lt,
        /// `<=`
        LessEqualThan = TokenType::LtEq,
        /// `>`
        GreaterThan = TokenType::Gt,
        /// `>=`
        GreaterEqualThan = TokenType::GtEq,
        /// `+`
        Addition = TokenType::Plus,
        /// `-`
        Subtraction = TokenType::Minus,
        /// `*`
        Multiplication = TokenType::Star,
        /// `/`
        Division = TokenType::Slash,
        /// `||`
        Or = TokenType::Or,
        /// `&&`
        And = TokenType::And,
        /// `??`
        Coalesce = TokenType::NullCoal,
    };

    enum class UnaryOp : uint8_t {
        Negate = TokenType::Minus,
        Not = TokenType::Not,
    };

    enum class CallType : uint8_t {
        Math = TokenType::Math,
        Query = TokenType::Query,
    };

    enum class ResourceKind : uint8_t {
        Geometry = TokenType::Geometry,
        Material = TokenType::Material,
        Texture = TokenType::Texture,
    };

    std::optional<std::pair<uint8_t, uint8_t> > token_bind_power(TokenType type);


    constexpr auto tokens_which_could_be_names = std::array{
        TokenType::Math,
        TokenType::Query,
        TokenType::Variable,
        TokenType::Geometry,
        TokenType::Material,
        TokenType::Texture,
        TokenType::Array,
        TokenType::True,
        TokenType::False,
        TokenType::This,
        TokenType::Break,
        TokenType::Continue,
        TokenType::ForEach,
        TokenType::Loop,
        TokenType::Return,
        TokenType::Temporary,
        TokenType::Context,
        TokenType::Eq,
        TokenType::NotEq,
        TokenType::LtEq,
        TokenType::GtEq,
        TokenType::Or,
        TokenType::And,
        TokenType::Arrow,
        TokenType::NullCoal,
        TokenType::Identifier,
    };


    struct Token {
        size_t size{1};
        TokenType type{};
        size_t position{};

        Token() = default;

        Token(const size_t position, const std::string_view string,
              const TokenType type) : type(type), position(position) {
            this->size = string.size();
        }

        Token(const size_t position, const TokenType type) : type(type), position(position) {
        }

        Token(const size_t position, const size_t size,
              const TokenType type
        ) : size(size), type(type), position(position) {
        }

        constexpr explicit Token(const TokenType type) : type(type), position(-1) {
        }

        bool operator==(const Token &other) const {
            return this->type == other.type;
        }

        bool operator!=(const Token &other) const {
            return !(*this == other);
        }

        bool operator==(const TokenType &other) const {
            return this->type == other;
        }

        bool operator!=(const TokenType &other) const {
            return !(*this == other);
        }


        // TODO: Make this a debug only function, else just make it do nothing so it gets removed in release

        template<size_t N>
        void assert_type(const std::array<TokenType, N> &types) const {
            for (const auto typ: types) {
                if (*this == typ) {
                    return;
                }
            }
            assert(false);
        }

        template<size_t N>
        bool check_types(const std::array<TokenType, N> &types) const {
            for (const auto typ: types) {
                if (*this == typ) {
                    return true;
                }
            }
            return false;
        }

        void assert_type(const TokenType type) const {
            if (*this != type) {
                // "Token is not of the right type"
                assert(false);
            }
        }
    };

    using TokenBuffer = molar_impl::SpanBuffer<std::vector<Token> >;


    std::string to_string(TokenType type);

    std::string to_symbol_string(TokenType type);
}


#endif //MOLANG_TOKEN_HPP
