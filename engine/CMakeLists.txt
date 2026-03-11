add_library(rlengine STATIC)

file(GLOB_RECURSE 
    ENGINE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(rlengine 
    PRIVATE 
        ${ENGINE_SRC}
)

target_include_directories(rlengine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(rlengine 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/core/Headers.hpp
)

target_compile_features(rlengine 
    PUBLIC 
        cxx_std_20
)

target_link_libraries(rlengine 
    PUBLIC 
        raylib 
        imgui 
        rlImGui
        nlohmann_json::nlohmann_json
        spdlog
)

target_compile_definitions(rlengine 
    PRIVATE
        ENGINE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)