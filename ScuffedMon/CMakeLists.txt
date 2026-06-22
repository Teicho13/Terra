file(GLOB_RECURSE SCUFFEDMON_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

add_executable(ScuffedMon ${SCUFFEDMON_SOURCES})

set_target_properties(ScuffedMon PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:ScuffedMon>"
)

target_include_directories(ScuffedMon
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

target_link_libraries(ScuffedMon
    PRIVATE
        Terra
)

add_custom_command(TARGET ScuffedMon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/Assets/ScuffedMon"
)