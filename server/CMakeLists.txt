project(babel_server)

set(SOURCES
    src/main.cpp
    src/Server.cpp
    src/Clients.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} asio::asio)