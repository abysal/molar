//
// Created by Akashic on 5/25/2025.
//

#include <array>
#include <iostream>
#include <print>

#include "ast/variable.hpp"
#include "execution/preprocessor/molang_preprocessor.hpp"
#include "molang_ast_generator.hpp"
#include "molang_tokenizer.hpp"

void simple_token() {
    constexpr auto expressions =
        std::array{"==", "m.sin(v.x + v.z * q.pos())", "v.our_id == 'some_string :3'"};

    for (const auto expression : expressions) {
        molar::MolangTokenizer parser{expression};
        std::cout << expression << " : ";
        parser.print_tokens(parser.parse_tokens(), false, std::cout);
    }
}

void variable_ast() {
    constexpr auto         expression = "v.math.hello_world.context";
    molar::MolangTokenizer parser{expression};
    auto                   tokens = parser.parse_tokens();
    molar::TokenBuffer     buffer{std::move(tokens)};
    const auto             source_buffer = parser.move_buffer();

    molar::ast::VariableReference var{buffer.next_value(), source_buffer, buffer};
}

void full_simple_ast() {
    constexpr auto expressions = std::array{
        "v.x = 2"
        "v.math * v.awa + v.x * v.y + 1;",
        "true + false;",
        "'hello' + ' ' + 'world';",
        "(v.x + v.y * 2; 2 + 2);",
        "{v.uwu + v.x; (200 + 2) * t.x;}",
        "20 * -v.r + -20.f;",
        "v.x * math.sin(20 * 30);",
        "Array.my_array[20 * (1 + 2 / 3)]",
        "loop(700 * 1.1, {v.q + math.sin(q.pos);});",
        "for_each(t.x, q.players(), {q.print(t.x);})",
        "return;",
        "return v.x + 2;",
        "q.foo ? 1 : v.bar == 13 ? 2 : 3;",
        "m.max(math.sin(q.pos() * math.cos(q.velocity + 20)), q.max_value);"
        "variable.hand_bob = query.life_time < 0.01 ? 0.0 : variable.hand_bob + "
        "((query.is_on_ground && query.is_alive ? "
        "math.clamp(math.sqrt(math.pow(query.position_delta(0), 2.0) + "
        "math.pow(query.position_delta(2), 2.0)), 0.0, 0.1) : 0.0) - variable.hand_bob) * "
        "0.02;",
    };

    for (const auto expression : expressions) {
        molar::MolangTokenizer parser{expression};
        auto                   tokens = parser.parse_tokens();
        molar::TokenBuffer     buffer{std::move(tokens)};
        auto                   source_buffer = parser.move_buffer();

        molar::MolangAstGenerator ast(std::move(buffer), std::move(source_buffer));

        for (auto out = ast.build_ast(); const auto& p : out.get_expressions()) {
            p->print(std::cout, 0);
            std::cout << std::endl;
        }
    }
}

void execution_tree_builder() {
    constexpr auto expressions = std::array{
        "v.x = math.sin(20);", "v.a + v.b * 2.5 + "
                               "t.speed / (v.mass + 0.1) + "
                               "temp.counter * 4 - v.xtra * "
                               "v.a + v.b * "
                               "v.location.x + v.location.y + v.location.z"
    };

    for (const auto expression : expressions) {
        molar::MolangTokenizer parser{expression};
        auto                   tokens = parser.parse_tokens();
        molar::TokenBuffer     buffer{std::move(tokens)};
        auto                   source_buffer = parser.move_buffer();

        molar::MolangAstGenerator ast(std::move(buffer), std::move(source_buffer));

        molar::exec::MolangPreprocessor processor{std::move(ast.build_ast())};
        processor.process();
        processor.rebuild();

        for (auto        processed_ast = processor.consume_ast();
             const auto& p : processed_ast.get_expressions()) {
            p->print(std::cout, 0);
            std::cout << std::endl;
        }
    }
}

int main() { execution_tree_builder(); }
