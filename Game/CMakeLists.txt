file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

add_executable(Game ${GAME_SOURCES})

set_target_properties(Game PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
)

target_include_directories(Game
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

target_link_libraries(Game
    PRIVATE
        Terra
        perlin_noise
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/Assets/Game"
)