project(babel_client)

set(SOURCES
    src/main.cpp
    src/Opus/Encoder.cpp
    src/Widgets/Call.cpp
    src/Opus/Decoder.cpp
    src/Widgets/Login.cpp
    src/Widgets/Contacts.cpp
    src/Audio/AudioBuffer.cpp
    src/Widgets/MainWindow.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
target_link_libraries(${PROJECT_NAME} portaudio::portaudio)
target_link_libraries(${PROJECT_NAME} Opus::Opus)