cmake_minimum_required(VERSION 3.20)

project(molar LANGUAGES CXX)

option(MOLAR_INSTALL_TARGET "Creates install rules for the molar target" ON)
option(MOLAR_BUILD_TEST "Enables the testing playground" ${PROJECT_IS_TOP_LEVEL})


set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/molar")
file(GLOB_RECURSE MOLAR_SOURCES
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.hpp"
)

add_library(molar STATIC ${MOLAR_SOURCES}
        molar/internal/source_buffer.hpp
        molar/molang_token.hpp
        molar/molang_ast_generator.cpp
        molar/molang_ast_generator.hpp
        molar/ast/expression.hpp
        molar/ast/Literal.cpp
        molar/ast/Literal.hpp
        molar/ast/variable.cpp
        molar/ast/variable.hpp
        molar/ast/internal/variable_manager.hpp
        molar/molang_token.cpp
        molar/ast/expression.cpp
        molar/ast/access_expression.cpp
        molar/ast/access_expression.hpp
        molar/ast/controll_flow.cpp
        molar/ast/controll_flow.hpp
        molar/ast/keyword.cpp
        molar/ast/keyword.hpp
        molar/execution/preprocessor/molang_preprocessor.cpp
        molar/execution/preprocessor/molang_preprocessor.hpp
        molar/ast/ast_visitor.hpp
        molar/ast/ast_visitor.cpp
        molar/execution/preprocessor/execution_nodes/pre_allocated_variable.cpp
        molar/execution/preprocessor/execution_nodes/pre_allocated_variable.hpp
        molar/execution/preprocessor/execution_nodes/pre_allocated.cpp
        molar/execution/preprocessor/execution_nodes/pre_allocated.hpp
        molar/execution/preprocessor/processed_ast_visitor.cpp
        molar/execution/preprocessor/processed_ast_visitor.hpp
        molar/internal/checked_down_cast.hpp
        molar/execution/preprocessor/ast_rebuilder.cpp
        molar/execution/preprocessor/ast_rebuilder.hpp
        molar/ast/ast_replacer.cpp
        molar/ast/ast_replacer.hpp
)


if (MSVC)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
        target_compile_options(molar PRIVATE /clang:-Wall /clang:-Wextra /clang:-Wconversion /clang:-Werror)
    else ()
        target_compile_options(molar PRIVATE /W3 /WX)
    endif ()
endif ()

target_compile_features(molar PUBLIC cxx_std_23)

target_include_directories(molar PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/molar>
        $<INSTALL_INTERFACE:include/molar>
)

if (MOLAR_INSTALL_TARGET)
    install(TARGETS molar
            EXPORT molar-targets
            RUNTIME DESTINATION "bin"
            ARCHIVE DESTINATION "lib"
            LIBRARY DESTINATION "lib"
    )

    install(DIRECTORY ${SOURCE_DIR}/
            DESTINATION include/molar
            FILES_MATCHING PATTERN "*.hpp"
    )
endif ()


if (MOLAR_BUILD_TEST)
    file(GLOB_RECURSE TEST_SOURCES "molar_test/*.cpp" "molar_test/*.hpp")
    add_executable(molar_test ${TEST_SOURCES})

    target_link_libraries(molar_test PRIVATE molar)

endif ()