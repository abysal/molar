//
// Created by Akashic on 5/25/2025.
//

#include <array>
#include <iostream>
#include <print>

#include "molang_tokenizer.hpp"

int main() {
    constexpr auto expressions = std::array{"==", "m.sin(v.x + v.z * q.pos())", "v.our_id == 'some_string :3'"};

    for (const auto expression: expressions) {
        molar::MolangTokenizer parser{expression};
        std::cout << expression << " : ";
        parser.print_tokens(parser.parse_tokens(), false, std::cout);
    }
}
