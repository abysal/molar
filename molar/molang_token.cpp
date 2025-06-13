//
// Created by Akashic on 5/26/2025.
//
#include "molang_token.hpp"

namespace molar {
    // Stolen from Nolana
    std::optional<std::pair<uint8_t, uint8_t>> token_bind_power(const TokenType type) {
        switch (type) {
        case TokenType::Not:
            return std::pair(16, 17);
        case TokenType::Star:
        case TokenType::Slash:
            return std::pair(14, 15);
        case TokenType::Plus:
        case TokenType::Minus:
            return std::pair(12, 13);
        case TokenType::Lt:
        case TokenType::Gt:
        case TokenType::LtEq:
        case TokenType::GtEq:
            return std::pair(10, 11);
        case TokenType::Eq:
        case TokenType::NotEq:
            return std::pair(8, 9);
        case TokenType::And:
            return std::pair(6, 7);
        case TokenType::Or:
            return std::pair(4, 5);
        case TokenType::Conditional:
            return std::pair(3, 4);
        case TokenType::NullCoal:
            return std::pair(1, 2);
        default:
            return std::nullopt;
        }
    }

    std::string to_string(const TokenType type) {
        switch (type) {
        case TokenType::Eof:
            return "Eof";
        case TokenType::Number:
            return "Number";
        case TokenType::LeftParen:
            return "LeftParen";
        case TokenType::RightParen:
            return "RightParen";
        case TokenType::LeftBrace:
            return "LeftBrace";
        case TokenType::RightBrace:
            return "RightBrace";
        case TokenType::LeftBracket:
            return "LeftBracket";
        case TokenType::RightBracket:
            return "RightBracket";
        case TokenType::Assign:
            return "Assign";
        case TokenType::Not:
            return "Not";
        case TokenType::Eq:
            return "Eq";
        case TokenType::NotEq:
            return "NotEq";
        case TokenType::Lt:
            return "Lt";
        case TokenType::Gt:
            return "Gt";
        case TokenType::LtEq:
            return "LtEq";
        case TokenType::GtEq:
            return "GtEq";
        case TokenType::Or:
            return "Or";
        case TokenType::And:
            return "And";
        case TokenType::Arrow:
            return "Arrow";
        case TokenType::Dot:
            return "Dot";
        case TokenType::Conditional:
            return "Conditional";
        case TokenType::NullCoal:
            return "NullCoal";
        case TokenType::Colon:
            return "Colon";
        case TokenType::Semi:
            return "Semi";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Plus:
            return "Plus";
        case TokenType::Star:
            return "Star";
        case TokenType::Slash:
            return "Slash";
        case TokenType::Temporary:
            return "Temporary";
        case TokenType::Variable:
            return "Variable";
        case TokenType::Context:
            return "Context";
        case TokenType::Math:
            return "Math";
        case TokenType::Query:
            return "Query";
        case TokenType::Geometry:
            return "Geometry";
        case TokenType::Material:
            return "Material";
        case TokenType::Texture:
            return "Texture";
        case TokenType::Array:
            return "Array";
        case TokenType::True:
            return "True";
        case TokenType::False:
            return "False";
        case TokenType::This:
            return "This";
        case TokenType::Break:
            return "Break";
        case TokenType::Continue:
            return "Continue";
        case TokenType::ForEach:
            return "ForEach";
        case TokenType::Loop:
            return "Loop";
        case TokenType::Return:
            return "Return";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::String:
            return "String";
        default:
            return "Unknown";
        }
    }

    std::string to_symbol_string(const TokenType type) {
        switch (type) {
        case TokenType::Eof:
            return "<eof>";
        case TokenType::Number:
            return "number";
        case TokenType::LeftParen:
            return "(";
        case TokenType::RightParen:
            return ")";
        case TokenType::LeftBrace:
            return "{";
        case TokenType::RightBrace:
            return "}";
        case TokenType::LeftBracket:
            return "[";
        case TokenType::RightBracket:
            return "]";

        case TokenType::Assign:
            return "=";
        case TokenType::Not:
            return "!";
        case TokenType::Eq:
            return "==";
        case TokenType::NotEq:
            return "!=";
        case TokenType::Lt:
            return "<";
        case TokenType::Gt:
            return ">";
        case TokenType::LtEq:
            return "<=";
        case TokenType::GtEq:
            return ">=";

        case TokenType::Or:
            return "||";
        case TokenType::And:
            return "&&";
        case TokenType::Arrow:
            return "->";
        case TokenType::Dot:
            return ".";
        case TokenType::Conditional:
            return "?";
        case TokenType::NullCoal:
            return "??";
        case TokenType::Colon:
            return ":";
        case TokenType::Semi:
            return ";";
        case TokenType::Comma:
            return ",";
        case TokenType::Minus:
            return "-";
        case TokenType::Plus:
            return "+";
        case TokenType::Star:
            return "*";
        case TokenType::Slash:
            return "/";

        // Keywords
        case TokenType::Temporary:
            return "temp";
        case TokenType::Variable:
            return "v";
        case TokenType::Context:
            return "context";
        case TokenType::Math:
            return "math";
        case TokenType::Query:
            return "query";
        case TokenType::Geometry:
            return "geometry";
        case TokenType::Material:
            return "material";
        case TokenType::Texture:
            return "texture";
        case TokenType::Array:
            return "array";
        case TokenType::True:
            return "true";
        case TokenType::False:
            return "false";
        case TokenType::This:
            return "this";
        case TokenType::Break:
            return "break";
        case TokenType::Continue:
            return "continue";
        case TokenType::ForEach:
            return "for_each";
        case TokenType::Loop:
            return "loop";
        case TokenType::Return:
            return "return";
        case TokenType::Identifier:
            return "identifier";
        case TokenType::String:
            return "string";

        default:
            return "<unknown>";
        }
    }
} // namespace molar
