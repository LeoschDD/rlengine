add_executable(sandbox)

file(GLOB_RECURSE
    SANDBOX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(sandbox
    PRIVATE
        ${SANDBOX_SRC}
)

target_include_directories(sandbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(sandbox
    PRIVATE
        rlengine
)

target_compile_definitions(sandbox 
    PRIVATE
        SANDBOX_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)