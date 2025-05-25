//
// Created by Akashic on 5/25/2025.
//

#ifndef SOURCE_BUFFER_HPP
#define SOURCE_BUFFER_HPP
#include <algorithm>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>

namespace molar_impl {
    class SourceBuffer {
    public:
        using RollbackPoint = size_t;


        class Peak {
        public:
            Peak() = default;

            // NOLINTNEXTLINE
            operator char() const {
                return our_value;
            }

            void apply() const {
                this->parent->skip(1);
            }

        private:
            char our_value{};
            SourceBuffer *parent{};

            friend class SourceBuffer;


            Peak(const char value, SourceBuffer *source_buffer) : our_value(value), parent(source_buffer) {
            }
        };

    public:
        explicit SourceBuffer(std::string &&source) : string_source(std::move(source)) {
        }

        /**
         * Checks if the next bytes are equal to the next value
         * @param next Start string to look for in the next positon
         * @return A valid optional if it was found containing the index it is at
         */
        std::optional<size_t> next(std::string_view next);

        std::optional<size_t> next(char next);

        std::optional<size_t> caseless_next(std::string_view next);

        [[nodiscard]] RollbackPoint get_rollback_point() const { return this->current_pos; }
        void apply_rollback_point(const RollbackPoint rollback_point) { this->current_pos = rollback_point; }

        char next_char();

        Peak peak_char();

        [[nodiscard]] bool has_any() const { return this->current_pos < this->string_source.size(); }

        void skip(size_t i);

        std::string_view slice_from_source(size_t start, size_t length) const;

    private:
        void bounds_check(size_t extra) const;

        [[nodiscard]] std::string_view get_view() const;

    private:
        std::string string_source{};
        size_t current_pos{};
    };

    template<typename Container>
    class SpanBuffer {
    public:
        using value_type = typename Container::value_type;
        using view = std::span<value_type>;
        using const_view = std::span<const value_type>;
        using RollbackPoint = size_t;


        class Peak {
        public:
            Peak() = default;

            // NOLINTNEXTLINE
            operator char() const {
                return our_value;
            }

            void apply() const {
                this->parent->skip(1);
            }

        private:
            value_type our_value{};
            SourceBuffer *parent{};

            friend class SourceBuffer;


            Peak(const value_type &value, SourceBuffer *source_buffer) : our_value(value), parent(source_buffer) {
            }
        };

    public:
        explicit SpanBuffer(Container &&source) : container_source(std::move(source)) {
        }

        std::optional<size_t> next(view next) {
            try {
                this->bounds_check(1);
            } catch (...) {
                return std::nullopt;
            }

            if (std::ranges::starts_with(this->get_view(), next)) {
                const auto current_index = this->current_pos;
                this->current_pos += 1;
                return current_index;
            }

            return std::nullopt;
        }

        std::optional<size_t> next(value_type next) {
            return this->next(view{&next, 1});
        }

        [[nodiscard]] RollbackPoint get_rollback_point() const { return this->current_pos; }
        void apply_rollback_point(const RollbackPoint rollback_point) { this->current_pos = rollback_point; }

        value_type next_value() {
            this->bounds_check(1);
            const value_type value = this->get_view()[0];
            this->current_pos += 1;
            return value;
        }

        Peak peak_value() {
            this->bounds_check(1);
            return Peak{
                [&] {
                    const auto value = this->next_value();
                    this->current_pos -= 1;
                    return value;
                }(),
                this
            };
        }

        [[nodiscard]] bool has_any() const {
            return this->current_pos < this->container_source.size();
        }

        void skip(size_t i) {
            this->bounds_check(i);
            this->current_pos += i;
        }

        view slice_from_source(const size_t start, const size_t length) const {
            return std::string_view{this->container_source}.substr(start, length);
        }

    private:
        void bounds_check(size_t extra) const {
            if (this->current_pos + extra > this->container_source.size()) {
                throw std::out_of_range("SourceBuffer::bounds_check");
            }
        }

        [[nodiscard]] const_view get_view() const {
            return std::span{this->container_source}.subspan(this->current_pos);
        }

    private
    :
        Container container_source{};
        size_t current_pos{};
    };
} // molar_impl

#endif //SOURCE_BUFFER_HPP
