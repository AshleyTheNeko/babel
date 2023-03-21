project(babel_client)

set(SOURCES
    src/main.cpp
    src/Audio/Opus.cpp
    src/Widgets/Call.cpp
    src/Widgets/Login.cpp
    src/Widgets/Contacts.cpp
    src/Audio/AudioBuffer.cpp
    src/Widgets/MainWindow.cpp
    src/Widgets/WindowSlots.cpp
    src/Widgets/NetFunctions.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${COMMON_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
target_link_libraries(${PROJECT_NAME} portaudio::portaudio)
target_link_libraries(${PROJECT_NAME} Opus::Opus)