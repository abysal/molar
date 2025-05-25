//
// Created by Akashic on 5/25/2025.
//

#ifndef MOLANG_TOKEN_HPP
#define MOLANG_TOKEN_HPP
#include <assert.h>
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

        explicit Token(const TokenType type) : type(type), position(-1) {
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

        template<size_t N>
        void assert_type(const std::array<TokenType, N> &types) const {
            for (const auto typ: types) {
                if (*this == typ) {
                    return;
                }
            }
            assert(false);
        }

        void assert_type(const TokenType type) const {
            if (*this != type) {
                // "Token is not of the right type"
                assert(false);
            }
        }
    };

    using TokenBuffer = molar_impl::SpanBuffer<std::vector<Token> >;


    inline std::string to_string(const TokenType type) {
        switch (type) {
            case TokenType::Eof: return "Eof";
            case TokenType::Number: return "Number";
            case TokenType::LeftParen: return "LeftParen";
            case TokenType::RightParen: return "RightParen";
            case TokenType::LeftBrace: return "LeftBrace";
            case TokenType::RightBrace: return "RightBrace";
            case TokenType::LeftBracket: return "LeftBracket";
            case TokenType::RightBracket: return "RightBracket";
            case TokenType::Assign: return "Assign";
            case TokenType::Not: return "Not";
            case TokenType::Eq: return "Eq";
            case TokenType::NotEq: return "NotEq";
            case TokenType::Lt: return "Lt";
            case TokenType::Gt: return "Gt";
            case TokenType::LtEq: return "LtEq";
            case TokenType::GtEq: return "GtEq";
            case TokenType::Or: return "Or";
            case TokenType::And: return "And";
            case TokenType::Arrow: return "Arrow";
            case TokenType::Dot: return "Dot";
            case TokenType::Conditional: return "Conditional";
            case TokenType::NullCoal: return "NullCoal";
            case TokenType::Colon: return "Colon";
            case TokenType::Semi: return "Semi";
            case TokenType::Comma: return "Comma";
            case TokenType::Minus: return "Minus";
            case TokenType::Plus: return "Plus";
            case TokenType::Star: return "Star";
            case TokenType::Slash: return "Slash";
            case TokenType::Temporary: return "Temporary";
            case TokenType::Variable: return "Variable";
            case TokenType::Context: return "Context";
            case TokenType::Math: return "Math";
            case TokenType::Query: return "Query";
            case TokenType::Geometry: return "Geometry";
            case TokenType::Material: return "Material";
            case TokenType::Texture: return "Texture";
            case TokenType::Array: return "Array";
            case TokenType::True: return "True";
            case TokenType::False: return "False";
            case TokenType::This: return "This";
            case TokenType::Break: return "Break";
            case TokenType::Continue: return "Continue";
            case TokenType::ForEach: return "ForEach";
            case TokenType::Loop: return "Loop";
            case TokenType::Return: return "Return";
            case TokenType::Identifier: return "Identifier";
            case TokenType::String: return "String";
            default: return "Unknown";
        }
    }

    inline std::string to_symbol_string(TokenType type) {
        switch (type) {
            case TokenType::Eof: return "<eof>";
            case TokenType::Number: return "number";
            case TokenType::LeftParen: return "(";
            case TokenType::RightParen: return ")";
            case TokenType::LeftBrace: return "{";
            case TokenType::RightBrace: return "}";
            case TokenType::LeftBracket: return "[";
            case TokenType::RightBracket: return "]";

            case TokenType::Assign: return "=";
            case TokenType::Not: return "!";
            case TokenType::Eq: return "==";
            case TokenType::NotEq: return "!=";
            case TokenType::Lt: return "<";
            case TokenType::Gt: return ">";
            case TokenType::LtEq: return "<=";
            case TokenType::GtEq: return ">=";

            case TokenType::Or: return "||";
            case TokenType::And: return "&&";
            case TokenType::Arrow: return "->";
            case TokenType::Dot: return ".";
            case TokenType::Conditional: return "?";
            case TokenType::NullCoal: return "??";
            case TokenType::Colon: return ":";
            case TokenType::Semi: return ";";
            case TokenType::Comma: return ",";
            case TokenType::Minus: return "-";
            case TokenType::Plus: return "+";
            case TokenType::Star: return "*";
            case TokenType::Slash: return "/";

            // Keywords
            case TokenType::Temporary: return "temp";
            case TokenType::Variable: return "v";
            case TokenType::Context: return "context";
            case TokenType::Math: return "math";
            case TokenType::Query: return "query";
            case TokenType::Geometry: return "geometry";
            case TokenType::Material: return "material";
            case TokenType::Texture: return "texture";
            case TokenType::Array: return "array";
            case TokenType::True: return "true";
            case TokenType::False: return "false";
            case TokenType::This: return "this";
            case TokenType::Break: return "break";
            case TokenType::Continue: return "continue";
            case TokenType::ForEach: return "for_each";
            case TokenType::Loop: return "loop";
            case TokenType::Return: return "return";
            case TokenType::Identifier: return "identifier";
            case TokenType::String: return "string";

            default: return "<unknown>";
        }
    }
}


#endif //MOLANG_TOKEN_HPP
