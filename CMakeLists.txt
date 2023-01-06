cmake_minimum_required(VERSION 3.14)

project("My Robot"
    VERSION 0.1.0
    DESCRIPTION "1111 Computer Graphics - HW5"
)

find_package(OpenGL REQUIRED)

add_executable(my-robot)

add_library(freeglut SHARED IMPORTED)
add_library(freeglutd SHARED IMPORTED)
set_target_properties(freeglut PROPERTIES
    IMPORTED_IMPLIB
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib/freeglut.lib"
    IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bin/freeglut.dll"
)
set_target_properties(freeglutd PROPERTIES
    IMPORTED_IMPLIB
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib/freeglutd.lib"
    IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bin/freeglutd.dll"
)

target_sources(my-robot PRIVATE
    "src/main.cpp"
    "src/stb_image.cpp"
)

target_include_directories(my-robot PRIVATE "include/")

target_link_libraries(my-robot PRIVATE
    OpenGL::GL
    OpenGL::GLU
    $<$<CONFIG:Debug>:freeglutd>
    $<$<NOT:$<CONFIG:Debug>>:freeglut>
)

# 複製料夾到建置目錄
add_custom_command(TARGET my-robot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bin/freeglut.dll"
        "$<TARGET_FILE_DIR:my-robot>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bin/freeglutd.dll"
        "$<TARGET_FILE_DIR:my-robot>"
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/images"
        "$<TARGET_FILE_DIR:my-robot>/images"
)
