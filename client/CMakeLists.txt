project(babel_client)

set(SOURCES
    src/main.cpp
    src/Widgets/Call.cpp
    src/Widgets/Login.cpp
    src/Widgets/Contacts.cpp
    src/Widgets/MainWindow.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)