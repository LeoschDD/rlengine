if(TARGET rlImGui)
    return()
endif()

set(RLIMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../external/rlImGui")

add_library(rlImGui STATIC
    "${RLIMGUI_DIR}/rlImGui.cpp"
)

target_include_directories(rlImGui 
    PUBLIC
        "${RLIMGUI_DIR}"
)

target_link_libraries(rlImGui 
    PUBLIC
        imgui
        raylib
)

target_compile_features(rlImGui 
    PUBLIC 
        cxx_std_20
)