file(GLOB_RECURSE TERRA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp"
)

add_library(Terra STATIC ${TERRA_SOURCES})

target_include_directories(Terra
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
        "${CMAKE_SOURCE_DIR}/External/OpenGL/GLAD/include"
)

target_link_libraries(Terra
    PUBLIC
        OpenGL::GL
        glfw
        glm
        stb
)

target_compile_definitions(Terra
    PUBLIC
        $<$<PLATFORM_ID:Windows>:WIN32>
)