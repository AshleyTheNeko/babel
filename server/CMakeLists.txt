project(babel_server)

set(SOURCES
    src/main.cpp
    src/Server.cpp
    src/Clients.cpp
    src/Database.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${COMMON_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} SQLite::SQLite)
target_link_libraries(${PROJECT_NAME} asio::asio)