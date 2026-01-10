cmake_minimum_required(VERSION 3.10)
project(Polytris VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/AssetsManager.cpp
    src/AttackBuffer.cpp
    src/Block.cpp
    src/Keyboard.cpp
    src/Leaderboard.cpp
    src/Menu.cpp
    src/NameEntry.cpp
    src/Player.cpp
    src/Window.cpp
    src/textTools.cpp
)

find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

add_executable(polytris ${SOURCES})

target_include_directories(polytris PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} src)
target_link_libraries(polytris PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Ensure compiler sees pkg-config include/link flags if present
foreach(flag ${SDL2_CFLAGS_OTHER})
  string(STRIP "${flag}" flag)
  if(flag)
    target_compile_options(polytris PRIVATE ${flag})
  endif()
endforeach()

# Copy resource folder next to the executable after build
add_custom_command(TARGET polytris POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/rsc $<TARGET_FILE_DIR:polytris>/rsc
)

install(TARGETS polytris RUNTIME DESTINATION bin)
install(DIRECTORY rsc DESTINATION share/Polytris)
