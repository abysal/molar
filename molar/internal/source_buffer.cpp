//
// Created by Akashic on 5/25/2025.
//

#include "source_buffer.hpp"
#include <stdexcept>
#include <ranges>

namespace molar_impl {
    std::optional<size_t> SourceBuffer::next(const std::string_view next) {
        try {
            this->bounds_check(next.size());
        } catch (...) {
            return std::nullopt;
        }

        if (this->get_view().starts_with(next)) {
            const auto current_index = this->current_pos;
            this->current_pos += next.size();
            return current_index;
        }

        return std::nullopt;
    }

    std::optional<size_t> SourceBuffer::next(const char next) {
        try {
            this->bounds_check(1);
        } catch (...) {
            return std::nullopt;
        }
        if (this->get_view().starts_with(next)) {
            const auto current_index = this->current_pos;
            this->current_pos += 1;
            return current_index;
        }

        return std::nullopt;
    }

    std::optional<size_t> SourceBuffer::caseless_next(std::string_view next) {
        try {
            this->bounds_check(next.size());
        } catch (...) {
            return std::nullopt;
        }
        for (const auto [source, expected]: std::views::zip(this->get_view(), next)) {
            if (toupper(source) != toupper(expected)) {
                return std::nullopt;
            }
        }

        const auto current_index = this->current_pos;
        this->current_pos += next.size();
        return current_index;
    }

    char SourceBuffer::next_char() {
        this->bounds_check(1);
        const char value = this->get_view()[0];
        this->current_pos += 1;
        return value;
    }

    SourceBuffer::Peak SourceBuffer::peak_char() {
        this->bounds_check(1);
        return Peak{
            [&] {
                const auto value = this->next_char();
                this->current_pos -= 1;
                return value;
            }(),
            this
        };
    }

    void SourceBuffer::skip(const size_t i) {
        this->bounds_check(i);
        this->current_pos += i;
    }

    std::string_view SourceBuffer::slice_from_source(const size_t start, const size_t length) const {
        return std::string_view{this->string_source}.substr(start, length);
    }

    void SourceBuffer::bounds_check(const size_t extra) const {
        if (this->current_pos + extra > this->string_source.size()) {
            throw std::out_of_range("SourceBuffer::bounds_check");
        }
    }

    std::string_view SourceBuffer::get_view() const {
        return std::string_view{this->string_source}.substr(this->current_pos);
    }
} // molar_impl
