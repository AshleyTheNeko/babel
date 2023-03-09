project(babel_client)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(EXISTS ${CLANG_TIDY_EXE})
    set(CLANG_TIDY
        "${CLANG_TIDY_EXE}"
        "-extra-arg=-std=c++20"
        "-warnings-as-errors=-*"
        "-header-filter=^.*(?:third-party).*gm")
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
endif()