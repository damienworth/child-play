option(WITH_FILESYSTEM "Write file first" OFF)
mark_as_advanced(WITH_FILESYSTEM)
option(WRITE_FILE "Write file first" OFF)
mark_as_advanced(WRITE_FILE)

set(BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/binary_dir.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/binary_dir.hpp @ONLY)

add_executable(file-io
    src/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/binary_dir.hpp)

target_include_directories(file-io
PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_compile_features(file-io
PRIVATE
    cxx_std_17)

target_compile_options(file-io
PRIVATE
    -Wall
    -Werror
    -Wextra
    -pedantic
    -Wconversion
    -Winit-self
    -Woverloaded-virtual
    -Wunreachable-code
    -Wold-style-cast
    -Wsign-promo
    -Wshadow)

if (WRITE_FILE)
    target_compile_definitions(file-io
    PRIVATE
        WRITE_FILE)
endif (WRITE_FILE)

if (WITH_FILESYSTEM)
    target_compile_definitions(file-io
    PRIVATE
        WITH_FILESYSTEM)

    target_link_libraries(file-io
    PRIVATE
        stdc++fs)
endif (WITH_FILESYSTEM)
