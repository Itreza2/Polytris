cmake_minimum_required(VERSION 3.10)
project(Polytris VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SDL submodules
set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)

# Source files
set(SOURCES
    src/core/Window.cpp
    src/core/textTools.cpp
    src/core/AssetsManager.cpp
    src/core/Keyboard.cpp
    src/main.cpp
    src/AttackBuffer.cpp
    src/Block.cpp
    src/Leaderboard.cpp
    src/Menu.cpp
    src/NameEntry.cpp
    src/Player.cpp
)

add_executable(polytris ${SOURCES})

target_include_directories(polytris PRIVATE include src)
target_link_libraries(polytris PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

# Copy resource folder next to the executable after build
add_custom_command(TARGET polytris POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/rsc $<TARGET_FILE_DIR:polytris>/rsc
)

install(TARGETS polytris RUNTIME DESTINATION bin)
install(DIRECTORY rsc DESTINATION share/Polytris)

add_custom_target(clean-all
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
)
