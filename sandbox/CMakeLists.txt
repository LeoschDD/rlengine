# sandbox library

add_library(sandbox_project)

file(GLOB_RECURSE
    SANDBOX_PROJECT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(sandbox_project
    PRIVATE
        ${SANDBOX_PROJECT_SRC}
)

target_include_directories(sandbox_project 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sandbox_project PUBLIC rlengine)

target_compile_definitions(sandbox_project 
    PUBLIC
        SANDBOX_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

# sandbox executable

add_executable(sandbox main.cpp)

target_link_libraries(sandbox
    PRIVATE
        sandbox_project
)