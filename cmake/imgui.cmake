if(TARGET imgui)
    return()
endif()

set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../external/imgui")

add_library(imgui STATIC)

target_sources(imgui 
    PRIVATE
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
)

target_include_directories(imgui 
    PUBLIC
        "${IMGUI_DIR}"
)

target_compile_features(imgui 
    PUBLIC 
        cxx_std_20
)