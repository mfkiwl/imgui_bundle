add_simple_external_library_with_sources(imgui_color_text_edit ImGuiColorTextEdit)
target_include_directories(imgui_color_text_edit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiColorTextEdit/vendor/regex/include)
if (IMGUI_BUNDLE_BUILD_PYTHON)
    target_compile_definitions(imgui_color_text_edit PRIVATE IMGUI_BUNDLE_BUILD_PYTHON)
endif()
