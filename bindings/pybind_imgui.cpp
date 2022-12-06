#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "imgui.h"
#include "imgui_toggle/imgui_toggle.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_docking_internal_types.h"
#include "imgui_bundle/patch_imgui.h"
#include "litgen_glue_code.h"

namespace py = pybind11;


//pybind11::array font_atlas_get_tex_data_as_rgba32(ImFontAtlas* self)   // Broken
//{
//    unsigned char *pixels;
//    int width, height, bytes_per_pixel;
//    self->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);
//    std::vector<std::size_t> shape = {(size_t) height * (size_t) width * (size_t) bytes_per_pixel};
//    auto r = pybind11::array(
//        pybind11::format_descriptor<uint8_t>::format(),
//        shape,
//        pixels
//    );
//    return r;
//};


// ----------------------------------------------------------------------------
// Part 1 : Type casters for ImVec2 and ImVec4
//    Python => C++: accept Tuple, List and np.array
//    C++ => Python: emit np.array (since it provides math operations)
//
// pybind11 reference: https://pybind11.readthedocs.io/en/stable/advanced/cast/custom.html
// ----------------------------------------------------------------------------

struct MyVec2
{
    MyVec2() { values[0] = values[1] = 42.f; }
    MyVec2(float x, float y) { values[0] = x; values[1] = y; }
    float& operator[](int i) { return values[i]; }
    float operator[](int i) const { return values[i]; }
    float values[2];
};


namespace pybind11
{
    namespace detail
    {
        template<> struct type_caster<MyVec2>
        {
        public:
            // This macro establishes the name 'ImVec2' in  function signatures and declares a local variable 'value' of type ImVec2
            PYBIND11_TYPE_CASTER(MyVec2, const_name("MyVec2"));

            // Conversion part 1 (Python->C++):
            // Return false upon failure.
            // The second argument indicates whether implicit conversions should be applied.
            bool load(handle src, bool)
            {
                if (isinstance<list>(src) || isinstance<tuple>(src) || isinstance<array>(src) )
                {
                    std::vector<float> as_floats = src.cast<std::vector<float>>();
                    if (as_floats.size() != 2)
                    {
                        printf("Cast python(list|tuple|array) to ImVec2: size should be 2!\n");
                        return false;
                    }
                    for (size_t i = 0; i < 2; ++i)
                        value[i] = as_floats[i];
                    return true;
                }
                printf("default false\n");
                return false;
            }

            // Conversion part 2 (C++ -> Python):
            // The second and third arguments are used to indicate the return value policy and parent object
            // (for ``return_value_policy::reference_internal``) and are generally
            // ignored by implicit casters.
            static handle cast(const MyVec2& v, return_value_policy, handle defval)
            {
                constexpr py::ssize_t shape[1] = { 2 };
                constexpr py::ssize_t strides[1] = { sizeof(float) };
                static std::string float_numpy_str = pybind11::format_descriptor<float>::format();
                static auto dtype_float = pybind11::dtype(float_numpy_str);

                auto as_array = py::array(dtype_float, shape, strides);
                return as_array.release();
            }
        };
    }
}




void py_init_module_imgui_main(py::module& m)
{
    m.def("c2p", []() {
        return MyVec2(1, 2);
    });
    m.def("p2c", [](const MyVec2& v) {
       printf("MyVec2: %f %f\n", v[0], v[1]);
    });

    ///////////////////////////////////////////////////////////////////////////
    // Manual patches for elements whose signature is too esoteric
    // and cannot be automatically bound
    ///////////////////////////////////////////////////////////////////////////

    // FLT_MIN & FLT_MAX
    m.attr("FLT_MIN") = (float)FLT_MIN;
    m.attr("FLT_MAX") = (float)FLT_MAX;

    m.def("set_io_ini_filename", PatchImGui::set_imgui_io_filename);
    m.def("set_io_log_filename", PatchImGui::set_imgui_log_filename);
    // m.def("font_atlas_get_tex_data_as_rgba32", font_atlas_get_tex_data_as_rgba32);

    m.def("font_atlas_add_font_from_file_ttf", PatchImGui::font_atlas_add_font_from_file_ttf,
          py::arg("font_atlas"),
          py::arg("filename"),
          py::arg("size_pixels"),
          py::arg("font_cfg") = py::none(),
          py::arg("glyph_ranges_as_int_list") = py::none(),
          py::return_value_policy::reference
          );

    m.def("font_atlas_glyph_ranges_default", PatchImGui::font_atlas_glyph_ranges_default, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_greek", PatchImGui::font_atlas_glyph_ranges_greek, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_korean", PatchImGui::font_atlas_glyph_ranges_korean, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_japanese", PatchImGui::font_atlas_glyph_ranges_japanese, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_chinese_full", PatchImGui::font_atlas_glyph_ranges_chinese_full, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_chinese_simplified_common", PatchImGui::font_atlas_glyph_ranges_chinese_simplified_common, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_cyrillic", PatchImGui::font_atlas_glyph_ranges_cyrillic, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_thai", PatchImGui::font_atlas_glyph_ranges_thai, py::arg("font_atlas"));
    m.def("font_atlas_glyph_ranges_vietnamese", PatchImGui::font_atlas_glyph_ranges_vietnamese, py::arg("font_atlas"));


    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // <litgen_pydef> // Autogenerated code below! Do not edit!
    ////////////////////    <generated_from:imgui.h>    ////////////////////
    auto pyClassImVec2 =
        py::class_<ImVec2>    // imgui.h:259
            (m, "ImVec2", "")
        .def_readwrite("x", &ImVec2::x, "")    // imgui.h:261
        .def_readwrite("y", &ImVec2::y, "")    // imgui.h:261
        .def(py::init<>())    // imgui.h:262
        .def(py::init<float, float>(),    // imgui.h:263
            py::arg("_x"), py::arg("_y"))
        ;


    auto pyClassImVec4 =
        py::class_<ImVec4>    // imgui.h:272
            (m, "ImVec4", "ImVec4: 4D vector used to store clipping rectangles, colors etc. [Compile-time configurable type]")
        .def_readwrite("x", &ImVec4::x, "")    // imgui.h:274
        .def_readwrite("y", &ImVec4::y, "")    // imgui.h:274
        .def_readwrite("z", &ImVec4::z, "")    // imgui.h:274
        .def_readwrite("w", &ImVec4::w, "")    // imgui.h:274
        .def(py::init<>())    // imgui.h:275
        .def(py::init<float, float, float, float>(),    // imgui.h:276
            py::arg("_x"), py::arg("_y"), py::arg("_z"), py::arg("_w"))
        ;


    m.def("create_context",    // imgui.h:294
        ImGui::CreateContext,
        py::arg("shared_font_atlas") = py::none(),
        pybind11::return_value_policy::reference);

    m.def("destroy_context",    // imgui.h:295
        ImGui::DestroyContext,
        py::arg("ctx") = py::none(),
        "None = destroy current context");

    m.def("get_current_context",    // imgui.h:296
        ImGui::GetCurrentContext, pybind11::return_value_policy::reference);

    m.def("set_current_context",    // imgui.h:297
        ImGui::SetCurrentContext, py::arg("ctx"));

    m.def("get_io",    // imgui.h:300
        ImGui::GetIO,
        "access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)",
        pybind11::return_value_policy::reference);

    m.def("get_style",    // imgui.h:301
        ImGui::GetStyle,
        "access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame!",
        pybind11::return_value_policy::reference);

    m.def("new_frame",    // imgui.h:302
        ImGui::NewFrame, "start a new Dear ImGui frame, you can submit any command from this point until Render()/EndFrame().");

    m.def("end_frame",    // imgui.h:303
        ImGui::EndFrame, "ends the Dear ImGui frame. automatically called by Render(). If you don't need to render data (skipping rendering) you may call EndFrame() without Render()... but you'll have wasted CPU already! If you don't need to render, better to not create any windows and not call NewFrame() at all!");

    m.def("render",    // imgui.h:304
        ImGui::Render, "ends the Dear ImGui frame, finalize the draw data. You can then get call GetDrawData().");

    m.def("get_draw_data",    // imgui.h:305
        ImGui::GetDrawData,
        "valid after Render() and until the next call to NewFrame(). this is what you have to render.",
        pybind11::return_value_policy::reference);

    m.def("show_demo_window",    // imgui.h:308
        [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
        {
            auto ShowDemoWindow_adapt_modifiable_immutable_to_return = [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                ImGui::ShowDemoWindow(p_open_adapt_modifiable);
                return p_open;
            };

            return ShowDemoWindow_adapt_modifiable_immutable_to_return(p_open);
        },
        py::arg("p_open") = py::none(),
        "create Demo window. demonstrate most ImGui features. call this to learn about the library! try to make it always available in your application!");

    m.def("show_metrics_window",    // imgui.h:309
        [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
        {
            auto ShowMetricsWindow_adapt_modifiable_immutable_to_return = [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                ImGui::ShowMetricsWindow(p_open_adapt_modifiable);
                return p_open;
            };

            return ShowMetricsWindow_adapt_modifiable_immutable_to_return(p_open);
        },
        py::arg("p_open") = py::none(),
        "create Metrics/Debugger window. display Dear ImGui internals: windows, draw commands, various internal state, etc.");

    m.def("show_debug_log_window",    // imgui.h:310
        [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
        {
            auto ShowDebugLogWindow_adapt_modifiable_immutable_to_return = [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                ImGui::ShowDebugLogWindow(p_open_adapt_modifiable);
                return p_open;
            };

            return ShowDebugLogWindow_adapt_modifiable_immutable_to_return(p_open);
        },
        py::arg("p_open") = py::none(),
        "create Debug Log window. display a simplified log of important dear imgui events.");

    m.def("show_stack_tool_window",    // imgui.h:311
        [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
        {
            auto ShowStackToolWindow_adapt_modifiable_immutable_to_return = [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                ImGui::ShowStackToolWindow(p_open_adapt_modifiable);
                return p_open;
            };

            return ShowStackToolWindow_adapt_modifiable_immutable_to_return(p_open);
        },
        py::arg("p_open") = py::none(),
        "create Stack Tool window. hover items with mouse to query information about the source of their unique ID.");

    m.def("show_about_window",    // imgui.h:312
        [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
        {
            auto ShowAboutWindow_adapt_modifiable_immutable_to_return = [](std::optional<bool> p_open = std::nullopt) -> std::optional<bool>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                ImGui::ShowAboutWindow(p_open_adapt_modifiable);
                return p_open;
            };

            return ShowAboutWindow_adapt_modifiable_immutable_to_return(p_open);
        },
        py::arg("p_open") = py::none(),
        "create About window. display Dear ImGui version, credits and build/system information.");

    m.def("show_style_editor",    // imgui.h:313
        ImGui::ShowStyleEditor,
        py::arg("ref") = py::none(),
        "add style editor block (not a window). you can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it uses the default style)");

    m.def("show_style_selector",    // imgui.h:314
        ImGui::ShowStyleSelector,
        py::arg("label"),
        "add style selector block (not a window), essentially a combo listing the default styles.");

    m.def("show_font_selector",    // imgui.h:315
        ImGui::ShowFontSelector,
        py::arg("label"),
        "add font selector block (not a window), essentially a combo listing the loaded fonts.");

    m.def("show_user_guide",    // imgui.h:316
        ImGui::ShowUserGuide, "add basic help/info block (not a window): how to manipulate ImGui as an end-user (mouse/keyboard controls).");

    m.def("get_version",    // imgui.h:317
        ImGui::GetVersion,
        "get the compiled version string e.g. \"1.80 WIP\" (essentially the value for IMGUI_VERSION from the compiled version of imgui.cpp)",
        pybind11::return_value_policy::reference);

    m.def("style_colors_dark",    // imgui.h:320
        ImGui::StyleColorsDark,
        py::arg("dst") = py::none(),
        "new, recommended style (default)");

    m.def("style_colors_light",    // imgui.h:321
        ImGui::StyleColorsLight,
        py::arg("dst") = py::none(),
        "best used with borders and a custom, thicker font");

    m.def("style_colors_classic",    // imgui.h:322
        ImGui::StyleColorsClassic,
        py::arg("dst") = py::none(),
        "classic imgui style");

    m.def("begin",    // imgui.h:336
        [](const char * name, std::optional<bool> p_open = std::nullopt, ImGuiWindowFlags flags = 0) -> std::tuple<bool, std::optional<bool>>
        {
            auto Begin_adapt_modifiable_immutable_to_return = [](const char * name, std::optional<bool> p_open = std::nullopt, ImGuiWindowFlags flags = 0) -> std::tuple<bool, std::optional<bool>>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                bool r = ImGui::Begin(name, p_open_adapt_modifiable, flags);
                return std::make_tuple(r, p_open);
            };

            return Begin_adapt_modifiable_immutable_to_return(name, p_open, flags);
        },     py::arg("name"), py::arg("p_open") = py::none(), py::arg("flags") = 0);

    m.def("end",    // imgui.h:337
        ImGui::End);

    m.def("begin_child",    // imgui.h:347
        py::overload_cast<const char *, const ImVec2 &, bool, ImGuiWindowFlags>(ImGui::BeginChild), py::arg("str_id"), py::arg("size") = ImVec2(0, 0), py::arg("border") = false, py::arg("flags") = 0);

    m.def("begin_child",    // imgui.h:348
        py::overload_cast<ImGuiID, const ImVec2 &, bool, ImGuiWindowFlags>(ImGui::BeginChild), py::arg("id_"), py::arg("size") = ImVec2(0, 0), py::arg("border") = false, py::arg("flags") = 0);

    m.def("end_child",    // imgui.h:349
        ImGui::EndChild);

    m.def("is_window_appearing",    // imgui.h:353
        ImGui::IsWindowAppearing);

    m.def("is_window_collapsed",    // imgui.h:354
        ImGui::IsWindowCollapsed);

    m.def("is_window_focused",    // imgui.h:355
        ImGui::IsWindowFocused,
        py::arg("flags") = 0,
        "is current window focused? or its root/child, depending on flags. see flags for options.");

    m.def("is_window_hovered",    // imgui.h:356
        ImGui::IsWindowHovered,
        py::arg("flags") = 0,
        "is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!");

    m.def("get_window_draw_list",    // imgui.h:357
        py::overload_cast<>(ImGui::GetWindowDrawList),
        "get draw list associated to the current window, to append your own drawing primitives",
        pybind11::return_value_policy::reference);

    m.def("get_window_dpi_scale",    // imgui.h:358
        ImGui::GetWindowDpiScale, "get DPI scale currently associated to the current window's viewport.");

    m.def("get_window_pos",    // imgui.h:359
        ImGui::GetWindowPos, "get current window position in screen space (useful if you want to do your own drawing via the DrawList API)");

    m.def("get_window_size",    // imgui.h:360
        ImGui::GetWindowSize, "get current window size");

    m.def("get_window_width",    // imgui.h:361
        ImGui::GetWindowWidth, "get current window width (shortcut for GetWindowSize().x)");

    m.def("get_window_height",    // imgui.h:362
        ImGui::GetWindowHeight, "get current window height (shortcut for GetWindowSize().y)");

    m.def("get_window_viewport",    // imgui.h:363
        ImGui::GetWindowViewport,
        "get viewport currently associated to the current window.",
        pybind11::return_value_policy::reference);

    m.def("set_next_window_pos",    // imgui.h:367
        ImGui::SetNextWindowPos,
        py::arg("pos"), py::arg("cond") = 0, py::arg("pivot") = ImVec2(0, 0),
        "set next window position. call before Begin(). use pivot=(0.5,0.5) to center on given point, etc.");

    m.def("set_next_window_size",    // imgui.h:368
        ImGui::SetNextWindowSize,
        py::arg("size"), py::arg("cond") = 0,
        "set next window size. set axis to 0.0 to force an auto-fit on this axis. call before Begin()");

    m.def("set_next_window_size_constraints",    // imgui.h:369
        [](const ImVec2 & size_min, const ImVec2 & size_max, void * custom_callback_data = NULL)
        {
            auto SetNextWindowSizeConstraints_adapt_exclude_params = [](const ImVec2 & size_min, const ImVec2 & size_max, void * custom_callback_data = NULL)
            {
                ImGui::SetNextWindowSizeConstraints(size_min, size_max, NULL, custom_callback_data);
            };

            SetNextWindowSizeConstraints_adapt_exclude_params(size_min, size_max, custom_callback_data);
        },
        py::arg("size_min"), py::arg("size_max"), py::arg("custom_callback_data") = py::none(),
        "set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down. Use callback to apply non-trivial programmatic constraints.");

    m.def("set_next_window_content_size",    // imgui.h:370
        ImGui::SetNextWindowContentSize,
        py::arg("size"),
        "set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. set an axis to 0.0 to leave it automatic. call before Begin()");

    m.def("set_next_window_collapsed",    // imgui.h:371
        ImGui::SetNextWindowCollapsed,
        py::arg("collapsed"), py::arg("cond") = 0,
        "set next window collapsed state. call before Begin()");

    m.def("set_next_window_focus",    // imgui.h:372
        ImGui::SetNextWindowFocus, "set next window to be focused / top-most. call before Begin()");

    m.def("set_next_window_scroll",    // imgui.h:373
        ImGui::SetNextWindowScroll,
        py::arg("scroll"),
        "set next window scrolling value (use < 0.0 to not affect a given axis).");

    m.def("set_next_window_bg_alpha",    // imgui.h:374
        ImGui::SetNextWindowBgAlpha,
        py::arg("alpha"),
        "set next window background color alpha. helper to easily override the Alpha component of ImGuiCol_WindowBg/ChildBg/PopupBg. you may also use ImGuiWindowFlags_NoBackground.");

    m.def("set_next_window_viewport",    // imgui.h:375
        ImGui::SetNextWindowViewport,
        py::arg("viewport_id"),
        "set next window viewport");

    m.def("set_window_pos",    // imgui.h:376
        py::overload_cast<const ImVec2 &, ImGuiCond>(ImGui::SetWindowPos),
        py::arg("pos"), py::arg("cond") = 0,
        "(not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.");

    m.def("set_window_size",    // imgui.h:377
        py::overload_cast<const ImVec2 &, ImGuiCond>(ImGui::SetWindowSize),
        py::arg("size"), py::arg("cond") = 0,
        "(not recommended) set current window size - call within Begin()/End(). set to ImVec2(0, 0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.");

    m.def("set_window_collapsed",    // imgui.h:378
        py::overload_cast<bool, ImGuiCond>(ImGui::SetWindowCollapsed),
        py::arg("collapsed"), py::arg("cond") = 0,
        "(not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().");

    m.def("set_window_focus",    // imgui.h:379
        py::overload_cast<>(ImGui::SetWindowFocus), "(not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus().");

    m.def("set_window_font_scale",    // imgui.h:380
        ImGui::SetWindowFontScale,
        py::arg("scale"),
        "[OBSOLETE] set font scale. Adjust IO.FontGlobalScale if you want to scale all windows. This is an old API! For correct scaling, prefer to reload font + rebuild ImFontAtlas + call style.ScaleAllSizes().");

    m.def("set_window_pos",    // imgui.h:381
        py::overload_cast<const char *, const ImVec2 &, ImGuiCond>(ImGui::SetWindowPos),
        py::arg("name"), py::arg("pos"), py::arg("cond") = 0,
        "set named window position.");

    m.def("set_window_size",    // imgui.h:382
        py::overload_cast<const char *, const ImVec2 &, ImGuiCond>(ImGui::SetWindowSize),
        py::arg("name"), py::arg("size"), py::arg("cond") = 0,
        "set named window size. set axis to 0.0 to force an auto-fit on this axis.");

    m.def("set_window_collapsed",    // imgui.h:383
        py::overload_cast<const char *, bool, ImGuiCond>(ImGui::SetWindowCollapsed),
        py::arg("name"), py::arg("collapsed"), py::arg("cond") = 0,
        "set named window collapsed state");

    m.def("set_window_focus",    // imgui.h:384
        py::overload_cast<const char *>(ImGui::SetWindowFocus),
        py::arg("name"),
        "set named window to be focused / top-most. use None to remove focus.");

    m.def("get_content_region_avail",    // imgui.h:389
        ImGui::GetContentRegionAvail, "== GetContentRegionMax() - GetCursorPos()");

    m.def("get_content_region_max",    // imgui.h:390
        ImGui::GetContentRegionMax, "current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates");

    m.def("get_window_content_region_min",    // imgui.h:391
        ImGui::GetWindowContentRegionMin, "content boundaries min for the full window (roughly (0,0)-Scroll), in window coordinates");

    m.def("get_window_content_region_max",    // imgui.h:392
        ImGui::GetWindowContentRegionMax, "content boundaries max for the full window (roughly (0,0)+Size-Scroll) where Size can be overridden with SetNextWindowContentSize(), in window coordinates");

    m.def("get_scroll_x",    // imgui.h:397
        ImGui::GetScrollX, "get scrolling amount [0 .. GetScrollMaxX()]");

    m.def("get_scroll_y",    // imgui.h:398
        ImGui::GetScrollY, "get scrolling amount [0 .. GetScrollMaxY()]");

    m.def("set_scroll_x",    // imgui.h:399
        py::overload_cast<float>(ImGui::SetScrollX),
        py::arg("scroll_x"),
        "set scrolling amount [0 .. GetScrollMaxX()]");

    m.def("set_scroll_y",    // imgui.h:400
        py::overload_cast<float>(ImGui::SetScrollY),
        py::arg("scroll_y"),
        "set scrolling amount [0 .. GetScrollMaxY()]");

    m.def("get_scroll_max_x",    // imgui.h:401
        ImGui::GetScrollMaxX, "get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - DecorationsSize.x");

    m.def("get_scroll_max_y",    // imgui.h:402
        ImGui::GetScrollMaxY, "get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - DecorationsSize.y");

    m.def("set_scroll_here_x",    // imgui.h:403
        py::overload_cast<float>(ImGui::SetScrollHereX),
        py::arg("center_x_ratio") = 0.5f,
        "adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a \"default/current item\" visible, consider using SetItemDefaultFocus() instead.");

    m.def("set_scroll_here_y",    // imgui.h:404
        py::overload_cast<float>(ImGui::SetScrollHereY),
        py::arg("center_y_ratio") = 0.5f,
        "adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a \"default/current item\" visible, consider using SetItemDefaultFocus() instead.");

    m.def("set_scroll_from_pos_x",    // imgui.h:405
        py::overload_cast<float, float>(ImGui::SetScrollFromPosX),
        py::arg("local_x"), py::arg("center_x_ratio") = 0.5f,
        "adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.");

    m.def("set_scroll_from_pos_y",    // imgui.h:406
        py::overload_cast<float, float>(ImGui::SetScrollFromPosY),
        py::arg("local_y"), py::arg("center_y_ratio") = 0.5f,
        "adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.");

    m.def("push_font",    // imgui.h:409
        ImGui::PushFont,
        py::arg("font"),
        "use None as a shortcut to push default font");

    m.def("pop_font",    // imgui.h:410
        ImGui::PopFont);

    m.def("push_style_color",    // imgui.h:411
        py::overload_cast<ImGuiCol, ImU32>(ImGui::PushStyleColor),
        py::arg("idx"), py::arg("col"),
        "modify a style color. always use this if you modify the style after NewFrame().");

    m.def("push_style_color",    // imgui.h:412
        py::overload_cast<ImGuiCol, const ImVec4 &>(ImGui::PushStyleColor), py::arg("idx"), py::arg("col"));

    m.def("pop_style_color",    // imgui.h:413
        ImGui::PopStyleColor, py::arg("count") = 1);

    m.def("push_style_var",    // imgui.h:414
        py::overload_cast<ImGuiStyleVar, float>(ImGui::PushStyleVar),
        py::arg("idx"), py::arg("val"),
        "modify a style float variable. always use this if you modify the style after NewFrame().");

    m.def("push_style_var",    // imgui.h:415
        py::overload_cast<ImGuiStyleVar, const ImVec2 &>(ImGui::PushStyleVar),
        py::arg("idx"), py::arg("val"),
        "modify a style ImVec2 variable. always use this if you modify the style after NewFrame().");

    m.def("pop_style_var",    // imgui.h:416
        ImGui::PopStyleVar, py::arg("count") = 1);

    m.def("push_allow_keyboard_focus",    // imgui.h:417
        ImGui::PushAllowKeyboardFocus,
        py::arg("allow_keyboard_focus"),
        "== tab stop enable. Allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets");

    m.def("pop_allow_keyboard_focus",    // imgui.h:418
        ImGui::PopAllowKeyboardFocus);

    m.def("push_button_repeat",    // imgui.h:419
        ImGui::PushButtonRepeat,
        py::arg("repeat"),
        "in 'repeat' mode, Button*() functions return repeated True in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.");

    m.def("pop_button_repeat",    // imgui.h:420
        ImGui::PopButtonRepeat);

    m.def("push_item_width",    // imgui.h:423
        ImGui::PushItemWidth,
        py::arg("item_width"),
        "push width of items for common large \"item+label\" widgets. >0.0: width in pixels, <0.0 align xx pixels to the right of window (so -FLT_MIN always align width to the right side).");

    m.def("pop_item_width",    // imgui.h:424
        ImGui::PopItemWidth);

    m.def("set_next_item_width",    // imgui.h:425
        ImGui::SetNextItemWidth,
        py::arg("item_width"),
        "set width of the _next_ common large \"item+label\" widget. >0.0: width in pixels, <0.0 align xx pixels to the right of window (so -FLT_MIN always align width to the right side)");

    m.def("calc_item_width",    // imgui.h:426
        ImGui::CalcItemWidth, "width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.");

    m.def("push_text_wrap_pos",    // imgui.h:427
        ImGui::PushTextWrapPos,
        py::arg("wrap_local_pos_x") = 0.0f,
        "push word-wrapping position for Text*() commands. < 0.0: no wrapping; 0.0: wrap to end of window (or column); > 0.0: wrap at 'wrap_pos_x' position in window local space");

    m.def("pop_text_wrap_pos",    // imgui.h:428
        ImGui::PopTextWrapPos);

    m.def("get_font",    // imgui.h:432
        ImGui::GetFont,
        "get current font",
        pybind11::return_value_policy::reference);

    m.def("get_font_size",    // imgui.h:433
        ImGui::GetFontSize, "get current font size (= height in pixels) of current font with current scale applied");

    m.def("get_font_tex_uv_white_pixel",    // imgui.h:434
        ImGui::GetFontTexUvWhitePixel, "get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API");

    m.def("get_color_u32",    // imgui.h:435
        py::overload_cast<ImGuiCol, float>(ImGui::GetColorU32),
        py::arg("idx"), py::arg("alpha_mul") = 1.0f,
        "retrieve given style color with style alpha applied and optional extra alpha multiplier, packed as a 32-bit value suitable for ImDrawList");

    m.def("get_color_u32",    // imgui.h:436
        py::overload_cast<const ImVec4 &>(ImGui::GetColorU32),
        py::arg("col"),
        "retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList");

    m.def("get_color_u32",    // imgui.h:437
        py::overload_cast<ImU32>(ImGui::GetColorU32),
        py::arg("col"),
        "retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList");

    m.def("get_style_color_vec4",    // imgui.h:438
        ImGui::GetStyleColorVec4,
        py::arg("idx"),
        "retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.",
        pybind11::return_value_policy::reference);

    m.def("separator",    // imgui.h:447
        ImGui::Separator, "separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.");

    m.def("same_line",    // imgui.h:448
        ImGui::SameLine,
        py::arg("offset_from_start_x") = 0.0f, py::arg("spacing") = -1.0f,
        "call between widgets or groups to layout them horizontally. X position given in window coordinates.");

    m.def("new_line",    // imgui.h:449
        ImGui::NewLine, "undo a SameLine() or force a new line when in a horizontal-layout context.");

    m.def("spacing",    // imgui.h:450
        ImGui::Spacing, "add vertical spacing.");

    m.def("dummy",    // imgui.h:451
        ImGui::Dummy,
        py::arg("size"),
        "add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.");

    m.def("indent",    // imgui.h:452
        ImGui::Indent,
        py::arg("indent_w") = 0.0f,
        "move content position toward the right, by indent_w, or style.IndentSpacing if indent_w <= 0");

    m.def("unindent",    // imgui.h:453
        ImGui::Unindent,
        py::arg("indent_w") = 0.0f,
        "move content position back to the left, by indent_w, or style.IndentSpacing if indent_w <= 0");

    m.def("begin_group",    // imgui.h:454
        ImGui::BeginGroup, "lock horizontal starting position");

    m.def("end_group",    // imgui.h:455
        ImGui::EndGroup, "unlock horizontal starting position + capture the whole group bounding box into one \"item\" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)");

    m.def("get_cursor_pos",    // imgui.h:456
        ImGui::GetCursorPos, "cursor position in window coordinates (relative to window position)");

    m.def("get_cursor_pos_x",    // imgui.h:457
        ImGui::GetCursorPosX, "(some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.");

    m.def("get_cursor_pos_y",    // imgui.h:458
        ImGui::GetCursorPosY, "other functions such as GetCursorScreenPos or everything in ImDrawList::");

    m.def("set_cursor_pos",    // imgui.h:459
        ImGui::SetCursorPos,
        py::arg("local_pos"),
        "are using the main, absolute coordinate system.");

    m.def("set_cursor_pos_x",    // imgui.h:460
        ImGui::SetCursorPosX,
        py::arg("local_x"),
        "GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.)");

    m.def("set_cursor_pos_y",    // imgui.h:461
        ImGui::SetCursorPosY, py::arg("local_y"));

    m.def("get_cursor_start_pos",    // imgui.h:462
        ImGui::GetCursorStartPos, "initial cursor position in window coordinates");

    m.def("get_cursor_screen_pos",    // imgui.h:463
        ImGui::GetCursorScreenPos, "cursor position in absolute coordinates (useful to work with ImDrawList API). generally top-left == GetMainViewport()->Pos == (0,0) in single viewport mode, and bottom-right == GetMainViewport()->Pos+Size == io.DisplaySize in single-viewport mode.");

    m.def("set_cursor_screen_pos",    // imgui.h:464
        ImGui::SetCursorScreenPos,
        py::arg("pos"),
        "cursor position in absolute coordinates");

    m.def("align_text_to_frame_padding",    // imgui.h:465
        ImGui::AlignTextToFramePadding, "vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)");

    m.def("get_text_line_height",    // imgui.h:466
        ImGui::GetTextLineHeight, "~ FontSize");

    m.def("get_text_line_height_with_spacing",    // imgui.h:467
        ImGui::GetTextLineHeightWithSpacing, "~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)");

    m.def("get_frame_height",    // imgui.h:468
        ImGui::GetFrameHeight, "~ FontSize + style.FramePadding.y * 2");

    m.def("get_frame_height_with_spacing",    // imgui.h:469
        ImGui::GetFrameHeightWithSpacing, "~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)");

    m.def("push_id",    // imgui.h:482
        py::overload_cast<const char *>(ImGui::PushID),
        py::arg("str_id"),
        "push string into the ID stack (will hash string).");

    m.def("push_id",    // imgui.h:483
        py::overload_cast<const char *, const char *>(ImGui::PushID),
        py::arg("str_id_begin"), py::arg("str_id_end"),
        "push string into the ID stack (will hash string).");

    m.def("push_id",    // imgui.h:484
        py::overload_cast<const void *>(ImGui::PushID),
        py::arg("ptr_id"),
        "push pointer into the ID stack (will hash pointer).");

    m.def("push_id",    // imgui.h:485
        py::overload_cast<int>(ImGui::PushID),
        py::arg("int_id"),
        "push integer into the ID stack (will hash integer).");

    m.def("pop_id",    // imgui.h:486
        ImGui::PopID, "pop from the ID stack.");

    m.def("get_id",    // imgui.h:487
        py::overload_cast<const char *>(ImGui::GetID),
        py::arg("str_id"),
        "calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into ImGuiStorage yourself");

    m.def("get_id",    // imgui.h:488
        py::overload_cast<const char *, const char *>(ImGui::GetID), py::arg("str_id_begin"), py::arg("str_id_end"));

    m.def("get_id",    // imgui.h:489
        py::overload_cast<const void *>(ImGui::GetID), py::arg("ptr_id"));

    m.def("text_unformatted",    // imgui.h:492
        ImGui::TextUnformatted,
        py::arg("text"), py::arg("text_end") = py::none(),
        "raw text without formatting. Roughly equivalent to Text(\"%s\", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.");

    m.def("text",    // imgui.h:493
        [](const char * fmt)
        {
            auto Text_adapt_variadic_format = [](const char * fmt)
            {
                ImGui::Text("%s", fmt);
            };

            Text_adapt_variadic_format(fmt);
        },
        py::arg("fmt"),
        "formatted text");

    m.def("text_colored",    // imgui.h:495
        [](const ImVec4 & col, const char * fmt)
        {
            auto TextColored_adapt_variadic_format = [](const ImVec4 & col, const char * fmt)
            {
                ImGui::TextColored(col, "%s", fmt);
            };

            TextColored_adapt_variadic_format(col, fmt);
        },
        py::arg("col"), py::arg("fmt"),
        "shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();");

    m.def("text_disabled",    // imgui.h:497
        [](const char * fmt)
        {
            auto TextDisabled_adapt_variadic_format = [](const char * fmt)
            {
                ImGui::TextDisabled("%s", fmt);
            };

            TextDisabled_adapt_variadic_format(fmt);
        },
        py::arg("fmt"),
        "shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();");

    m.def("text_wrapped",    // imgui.h:499
        [](const char * fmt)
        {
            auto TextWrapped_adapt_variadic_format = [](const char * fmt)
            {
                ImGui::TextWrapped("%s", fmt);
            };

            TextWrapped_adapt_variadic_format(fmt);
        },
        py::arg("fmt"),
        "shortcut for PushTextWrapPos(0.0); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().");

    m.def("label_text",    // imgui.h:501
        [](const char * label, const char * fmt)
        {
            auto LabelText_adapt_variadic_format = [](const char * label, const char * fmt)
            {
                ImGui::LabelText(label, "%s", fmt);
            };

            LabelText_adapt_variadic_format(label, fmt);
        },
        py::arg("label"), py::arg("fmt"),
        "display text+label aligned the same way as value+label widgets");

    m.def("bullet_text",    // imgui.h:503
        [](const char * fmt)
        {
            auto BulletText_adapt_variadic_format = [](const char * fmt)
            {
                ImGui::BulletText("%s", fmt);
            };

            BulletText_adapt_variadic_format(fmt);
        },
        py::arg("fmt"),
        "shortcut for Bullet()+Text()");

    m.def("button",    // imgui.h:509
        ImGui::Button,
        py::arg("label"), py::arg("size") = ImVec2(0, 0),
        "button");

    m.def("small_button",    // imgui.h:510
        ImGui::SmallButton,
        py::arg("label"),
        "button with FramePadding=(0,0) to easily embed within text");

    m.def("invisible_button",    // imgui.h:511
        ImGui::InvisibleButton,
        py::arg("str_id"), py::arg("size"), py::arg("flags") = 0,
        "flexible button behavior without the visuals, frequently useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)");

    m.def("arrow_button",    // imgui.h:512
        ImGui::ArrowButton,
        py::arg("str_id"), py::arg("dir"),
        "square button with an arrow shape");

    m.def("checkbox",    // imgui.h:513
        [](const char * label, bool v) -> std::tuple<bool, bool>
        {
            auto Checkbox_adapt_modifiable_immutable_to_return = [](const char * label, bool v) -> std::tuple<bool, bool>
            {
                bool * v_adapt_modifiable = & v;

                bool r = ImGui::Checkbox(label, v_adapt_modifiable);
                return std::make_tuple(r, v);
            };

            return Checkbox_adapt_modifiable_immutable_to_return(label, v);
        },     py::arg("label"), py::arg("v"));

    m.def("checkbox_flags",    // imgui.h:514
        [](const char * label, int flags, int flags_value) -> std::tuple<bool, int>
        {
            auto CheckboxFlags_adapt_modifiable_immutable_to_return = [](const char * label, int flags, int flags_value) -> std::tuple<bool, int>
            {
                int * flags_adapt_modifiable = & flags;

                bool r = ImGui::CheckboxFlags(label, flags_adapt_modifiable, flags_value);
                return std::make_tuple(r, flags);
            };

            return CheckboxFlags_adapt_modifiable_immutable_to_return(label, flags, flags_value);
        },     py::arg("label"), py::arg("flags"), py::arg("flags_value"));

    m.def("checkbox_flags",    // imgui.h:515
        [](const char * label, unsigned int flags, unsigned int flags_value) -> std::tuple<bool, unsigned int>
        {
            auto CheckboxFlags_adapt_modifiable_immutable_to_return = [](const char * label, unsigned int flags, unsigned int flags_value) -> std::tuple<bool, unsigned int>
            {
                unsigned int * flags_adapt_modifiable = & flags;

                bool r = ImGui::CheckboxFlags(label, flags_adapt_modifiable, flags_value);
                return std::make_tuple(r, flags);
            };

            return CheckboxFlags_adapt_modifiable_immutable_to_return(label, flags, flags_value);
        },     py::arg("label"), py::arg("flags"), py::arg("flags_value"));

    m.def("radio_button",    // imgui.h:516
        py::overload_cast<const char *, bool>(ImGui::RadioButton),
        py::arg("label"), py::arg("active"),
        "use with e.g. if (RadioButton(\"one\", my_value==1)) { my_value = 1; }");

    m.def("radio_button",    // imgui.h:517
        [](const char * label, int v, int v_button) -> std::tuple<bool, int>
        {
            auto RadioButton_adapt_modifiable_immutable_to_return = [](const char * label, int v, int v_button) -> std::tuple<bool, int>
            {
                int * v_adapt_modifiable = & v;

                bool r = ImGui::RadioButton(label, v_adapt_modifiable, v_button);
                return std::make_tuple(r, v);
            };

            return RadioButton_adapt_modifiable_immutable_to_return(label, v, v_button);
        },
        py::arg("label"), py::arg("v"), py::arg("v_button"),
        "shortcut to handle the above pattern when value is an integer");

    m.def("progress_bar",    // imgui.h:518
        ImGui::ProgressBar, py::arg("fraction"), py::arg("size_arg") = ImVec2(-FLT_MIN, 0), py::arg("overlay") = py::none());

    m.def("bullet",    // imgui.h:519
        ImGui::Bullet, "draw a small circle + keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses");

    m.def("image",    // imgui.h:523
        ImGui::Image, py::arg("user_texture_id"), py::arg("size"), py::arg("uv0") = ImVec2(0, 0), py::arg("uv1") = ImVec2(1, 1), py::arg("tint_col") = ImVec4(1, 1, 1, 1), py::arg("border_col") = ImVec4(0, 0, 0, 0));

    m.def("image_button",    // imgui.h:524
        py::overload_cast<const char *, ImTextureID, const ImVec2 &, const ImVec2 &, const ImVec2 &, const ImVec4 &, const ImVec4 &>(ImGui::ImageButton), py::arg("str_id"), py::arg("user_texture_id"), py::arg("size"), py::arg("uv0") = ImVec2(0, 0), py::arg("uv1") = ImVec2(1, 1), py::arg("bg_col") = ImVec4(0, 0, 0, 0), py::arg("tint_col") = ImVec4(1, 1, 1, 1));

    m.def("begin_combo",    // imgui.h:529
        ImGui::BeginCombo, py::arg("label"), py::arg("preview_value"), py::arg("flags") = 0);

    m.def("end_combo",    // imgui.h:530
        ImGui::EndCombo, "only call EndCombo() if BeginCombo() returns True!");

    m.def("combo",    // imgui.h:531
        [](const char * label, int current_item, const std::vector<std::string> & items, int popup_max_height_in_items = -1) -> std::tuple<bool, int>
        {
            auto Combo_adapt_modifiable_immutable_to_return = [](const char * label, int current_item, const char * const items[], int items_count, int popup_max_height_in_items = -1) -> std::tuple<bool, int>
            {
                int * current_item_adapt_modifiable = & current_item;

                bool r = ImGui::Combo(label, current_item_adapt_modifiable, items, items_count, popup_max_height_in_items);
                return std::make_tuple(r, current_item);
            };
            auto Combo_adapt_c_string_list = [&Combo_adapt_modifiable_immutable_to_return](const char * label, int current_item, const std::vector<std::string> & items, int popup_max_height_in_items = -1) -> std::tuple<bool, int>
            {
                std::vector<const char *> items_ptrs;
                for (const auto& v: items)
                    items_ptrs.push_back(v.c_str());
                int items_count = static_cast<int>(items.size());

                auto r = Combo_adapt_modifiable_immutable_to_return(label, current_item, items_ptrs.data(), items_count, popup_max_height_in_items);
                return r;
            };

            return Combo_adapt_c_string_list(label, current_item, items, popup_max_height_in_items);
        },     py::arg("label"), py::arg("current_item"), py::arg("items"), py::arg("popup_max_height_in_items") = -1);

    m.def("combo",    // imgui.h:532
        [](const char * label, int current_item, const char * items_separated_by_zeros, int popup_max_height_in_items = -1) -> std::tuple<bool, int>
        {
            auto Combo_adapt_modifiable_immutable_to_return = [](const char * label, int current_item, const char * items_separated_by_zeros, int popup_max_height_in_items = -1) -> std::tuple<bool, int>
            {
                int * current_item_adapt_modifiable = & current_item;

                bool r = ImGui::Combo(label, current_item_adapt_modifiable, items_separated_by_zeros, popup_max_height_in_items);
                return std::make_tuple(r, current_item);
            };

            return Combo_adapt_modifiable_immutable_to_return(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
        },
        py::arg("label"), py::arg("current_item"), py::arg("items_separated_by_zeros"), py::arg("popup_max_height_in_items") = -1,
        "Separate items with \\0 within a string, end item-list with \\0\\0. e.g. \"One\\0Two\\0Three\\0\"");

    m.def("drag_float",    // imgui.h:547
        [](const char * label, float v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
        {
            auto DragFloat_adapt_modifiable_immutable_to_return = [](const char * label, float v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
            {
                float * v_adapt_modifiable = & v;

                bool r = ImGui::DragFloat(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragFloat_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },
        py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f, py::arg("format") = "%.3f", py::arg("flags") = 0,
        "If v_min >= v_max we have no bound");

    m.def("drag_float2",    // imgui.h:548
        [](const char * label, std::array<float, 2> v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 2>>
        {
            auto DragFloat2_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 2> v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 2>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::DragFloat2(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragFloat2_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f, py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("drag_float3",    // imgui.h:549
        [](const char * label, std::array<float, 3> v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
        {
            auto DragFloat3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 3> v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::DragFloat3(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragFloat3_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f, py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("drag_float4",    // imgui.h:550
        [](const char * label, std::array<float, 4> v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
        {
            auto DragFloat4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 4> v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::DragFloat4(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragFloat4_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f, py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("drag_float_range2",    // imgui.h:551
        [](const char * label, float v_current_min, float v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", const char * format_max = NULL, ImGuiSliderFlags flags = 0) -> std::tuple<bool, float, float>
        {
            auto DragFloatRange2_adapt_modifiable_immutable_to_return = [](const char * label, float v_current_min, float v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char * format = "%.3f", const char * format_max = NULL, ImGuiSliderFlags flags = 0) -> std::tuple<bool, float, float>
            {
                float * v_current_min_adapt_modifiable = & v_current_min;
                float * v_current_max_adapt_modifiable = & v_current_max;

                bool r = ImGui::DragFloatRange2(label, v_current_min_adapt_modifiable, v_current_max_adapt_modifiable, v_speed, v_min, v_max, format, format_max, flags);
                return std::make_tuple(r, v_current_min, v_current_max);
            };

            return DragFloatRange2_adapt_modifiable_immutable_to_return(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags);
        },     py::arg("label"), py::arg("v_current_min"), py::arg("v_current_max"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f, py::arg("format") = "%.3f", py::arg("format_max") = py::none(), py::arg("flags") = 0);

    m.def("drag_int",    // imgui.h:552
        [](const char * label, int v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, int>
        {
            auto DragInt_adapt_modifiable_immutable_to_return = [](const char * label, int v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, int>
            {
                int * v_adapt_modifiable = & v;

                bool r = ImGui::DragInt(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragInt_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },
        py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0, py::arg("v_max") = 0, py::arg("format") = "%d", py::arg("flags") = 0,
        "If v_min >= v_max we have no bound");

    m.def("drag_int2",    // imgui.h:553
        [](const char * label, std::array<int, 2> v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 2>>
        {
            auto DragInt2_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 2> v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 2>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::DragInt2(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragInt2_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0, py::arg("v_max") = 0, py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("drag_int3",    // imgui.h:554
        [](const char * label, std::array<int, 3> v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 3>>
        {
            auto DragInt3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 3> v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 3>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::DragInt3(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragInt3_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0, py::arg("v_max") = 0, py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("drag_int4",    // imgui.h:555
        [](const char * label, std::array<int, 4> v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 4>>
        {
            auto DragInt4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 4> v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 4>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::DragInt4(label, v_adapt_modifiable, v_speed, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return DragInt4_adapt_modifiable_immutable_to_return(label, v, v_speed, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0, py::arg("v_max") = 0, py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("drag_int_range2",    // imgui.h:556
        [](const char * label, int v_current_min, int v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", const char * format_max = NULL, ImGuiSliderFlags flags = 0) -> std::tuple<bool, int, int>
        {
            auto DragIntRange2_adapt_modifiable_immutable_to_return = [](const char * label, int v_current_min, int v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char * format = "%d", const char * format_max = NULL, ImGuiSliderFlags flags = 0) -> std::tuple<bool, int, int>
            {
                int * v_current_min_adapt_modifiable = & v_current_min;
                int * v_current_max_adapt_modifiable = & v_current_max;

                bool r = ImGui::DragIntRange2(label, v_current_min_adapt_modifiable, v_current_max_adapt_modifiable, v_speed, v_min, v_max, format, format_max, flags);
                return std::make_tuple(r, v_current_min, v_current_max);
            };

            return DragIntRange2_adapt_modifiable_immutable_to_return(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags);
        },     py::arg("label"), py::arg("v_current_min"), py::arg("v_current_max"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0, py::arg("v_max") = 0, py::arg("format") = "%d", py::arg("format_max") = py::none(), py::arg("flags") = 0);

    m.def("drag_scalar",    // imgui.h:557
        py::overload_cast<const char *, ImGuiDataType, void *, float, const void *, const void *, const char *, ImGuiSliderFlags>(ImGui::DragScalar), py::arg("label"), py::arg("data_type"), py::arg("p_data"), py::arg("v_speed") = 1.0f, py::arg("p_min") = py::none(), py::arg("p_max") = py::none(), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("drag_scalar_n",    // imgui.h:558
        py::overload_cast<const char *, ImGuiDataType, void *, int, float, const void *, const void *, const char *, ImGuiSliderFlags>(ImGui::DragScalarN), py::arg("label"), py::arg("data_type"), py::arg("p_data"), py::arg("components"), py::arg("v_speed") = 1.0f, py::arg("p_min") = py::none(), py::arg("p_max") = py::none(), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("slider_float",    // imgui.h:566
        [](const char * label, float v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
        {
            auto SliderFloat_adapt_modifiable_immutable_to_return = [](const char * label, float v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
            {
                float * v_adapt_modifiable = & v;

                bool r = ImGui::SliderFloat(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderFloat_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },
        py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%.3f", py::arg("flags") = 0,
        "adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.");

    m.def("slider_float2",    // imgui.h:567
        [](const char * label, std::array<float, 2> v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 2>>
        {
            auto SliderFloat2_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 2> v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 2>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::SliderFloat2(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderFloat2_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("slider_float3",    // imgui.h:568
        [](const char * label, std::array<float, 3> v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
        {
            auto SliderFloat3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 3> v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::SliderFloat3(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderFloat3_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("slider_float4",    // imgui.h:569
        [](const char * label, std::array<float, 4> v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
        {
            auto SliderFloat4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 4> v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::SliderFloat4(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderFloat4_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("slider_angle",    // imgui.h:570
        [](const char * label, float v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char * format = "%.0f deg", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
        {
            auto SliderAngle_adapt_modifiable_immutable_to_return = [](const char * label, float v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char * format = "%.0f deg", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
            {
                float * v_rad_adapt_modifiable = & v_rad;

                bool r = ImGui::SliderAngle(label, v_rad_adapt_modifiable, v_degrees_min, v_degrees_max, format, flags);
                return std::make_tuple(r, v_rad);
            };

            return SliderAngle_adapt_modifiable_immutable_to_return(label, v_rad, v_degrees_min, v_degrees_max, format, flags);
        },     py::arg("label"), py::arg("v_rad"), py::arg("v_degrees_min") = -360.0f, py::arg("v_degrees_max") = +360.0f, py::arg("format") = "%.0f deg", py::arg("flags") = 0);

    m.def("slider_int",    // imgui.h:571
        [](const char * label, int v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, int>
        {
            auto SliderInt_adapt_modifiable_immutable_to_return = [](const char * label, int v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, int>
            {
                int * v_adapt_modifiable = & v;

                bool r = ImGui::SliderInt(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderInt_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("slider_int2",    // imgui.h:572
        [](const char * label, std::array<int, 2> v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 2>>
        {
            auto SliderInt2_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 2> v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 2>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::SliderInt2(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderInt2_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("slider_int3",    // imgui.h:573
        [](const char * label, std::array<int, 3> v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 3>>
        {
            auto SliderInt3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 3> v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 3>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::SliderInt3(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderInt3_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("slider_int4",    // imgui.h:574
        [](const char * label, std::array<int, 4> v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 4>>
        {
            auto SliderInt4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 4> v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, std::array<int, 4>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::SliderInt4(label, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return SliderInt4_adapt_modifiable_immutable_to_return(label, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("slider_scalar",    // imgui.h:575
        py::overload_cast<const char *, ImGuiDataType, void *, const void *, const void *, const char *, ImGuiSliderFlags>(ImGui::SliderScalar), py::arg("label"), py::arg("data_type"), py::arg("p_data"), py::arg("p_min"), py::arg("p_max"), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("slider_scalar_n",    // imgui.h:576
        py::overload_cast<const char *, ImGuiDataType, void *, int, const void *, const void *, const char *, ImGuiSliderFlags>(ImGui::SliderScalarN), py::arg("label"), py::arg("data_type"), py::arg("p_data"), py::arg("components"), py::arg("p_min"), py::arg("p_max"), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("v_slider_float",    // imgui.h:577
        [](const char * label, const ImVec2 & size, float v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
        {
            auto VSliderFloat_adapt_modifiable_immutable_to_return = [](const char * label, const ImVec2 & size, float v, float v_min, float v_max, const char * format = "%.3f", ImGuiSliderFlags flags = 0) -> std::tuple<bool, float>
            {
                float * v_adapt_modifiable = & v;

                bool r = ImGui::VSliderFloat(label, size, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return VSliderFloat_adapt_modifiable_immutable_to_return(label, size, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("size"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("v_slider_int",    // imgui.h:578
        [](const char * label, const ImVec2 & size, int v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, int>
        {
            auto VSliderInt_adapt_modifiable_immutable_to_return = [](const char * label, const ImVec2 & size, int v, int v_min, int v_max, const char * format = "%d", ImGuiSliderFlags flags = 0) -> std::tuple<bool, int>
            {
                int * v_adapt_modifiable = & v;

                bool r = ImGui::VSliderInt(label, size, v_adapt_modifiable, v_min, v_max, format, flags);
                return std::make_tuple(r, v);
            };

            return VSliderInt_adapt_modifiable_immutable_to_return(label, size, v, v_min, v_max, format, flags);
        },     py::arg("label"), py::arg("size"), py::arg("v"), py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%d", py::arg("flags") = 0);

    m.def("v_slider_scalar",    // imgui.h:579
        ImGui::VSliderScalar, py::arg("label"), py::arg("size"), py::arg("data_type"), py::arg("p_data"), py::arg("p_min"), py::arg("p_max"), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("input_float",    // imgui.h:587
        [](const char * label, float v, float step = 0.0f, float step_fast = 0.0f, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, float>
        {
            auto InputFloat_adapt_modifiable_immutable_to_return = [](const char * label, float v, float step = 0.0f, float step_fast = 0.0f, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, float>
            {
                float * v_adapt_modifiable = & v;

                bool r = ImGui::InputFloat(label, v_adapt_modifiable, step, step_fast, format, flags);
                return std::make_tuple(r, v);
            };

            return InputFloat_adapt_modifiable_immutable_to_return(label, v, step, step_fast, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("step") = 0.0f, py::arg("step_fast") = 0.0f, py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("input_float2",    // imgui.h:588
        [](const char * label, std::array<float, 2> v, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<float, 2>>
        {
            auto InputFloat2_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 2> v, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<float, 2>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::InputFloat2(label, v_adapt_modifiable, format, flags);
                return std::make_tuple(r, v);
            };

            return InputFloat2_adapt_modifiable_immutable_to_return(label, v, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("input_float3",    // imgui.h:589
        [](const char * label, std::array<float, 3> v, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
        {
            auto InputFloat3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 3> v, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::InputFloat3(label, v_adapt_modifiable, format, flags);
                return std::make_tuple(r, v);
            };

            return InputFloat3_adapt_modifiable_immutable_to_return(label, v, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("input_float4",    // imgui.h:590
        [](const char * label, std::array<float, 4> v, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
        {
            auto InputFloat4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 4> v, const char * format = "%.3f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
            {
                float * v_adapt_modifiable = v.data();

                bool r = ImGui::InputFloat4(label, v_adapt_modifiable, format, flags);
                return std::make_tuple(r, v);
            };

            return InputFloat4_adapt_modifiable_immutable_to_return(label, v, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("format") = "%.3f", py::arg("flags") = 0);

    m.def("input_int",    // imgui.h:591
        [](const char * label, int v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, int>
        {
            auto InputInt_adapt_modifiable_immutable_to_return = [](const char * label, int v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, int>
            {
                int * v_adapt_modifiable = & v;

                bool r = ImGui::InputInt(label, v_adapt_modifiable, step, step_fast, flags);
                return std::make_tuple(r, v);
            };

            return InputInt_adapt_modifiable_immutable_to_return(label, v, step, step_fast, flags);
        },     py::arg("label"), py::arg("v"), py::arg("step") = 1, py::arg("step_fast") = 100, py::arg("flags") = 0);

    m.def("input_int2",    // imgui.h:592
        [](const char * label, std::array<int, 2> v, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<int, 2>>
        {
            auto InputInt2_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 2> v, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<int, 2>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::InputInt2(label, v_adapt_modifiable, flags);
                return std::make_tuple(r, v);
            };

            return InputInt2_adapt_modifiable_immutable_to_return(label, v, flags);
        },     py::arg("label"), py::arg("v"), py::arg("flags") = 0);

    m.def("input_int3",    // imgui.h:593
        [](const char * label, std::array<int, 3> v, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<int, 3>>
        {
            auto InputInt3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 3> v, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<int, 3>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::InputInt3(label, v_adapt_modifiable, flags);
                return std::make_tuple(r, v);
            };

            return InputInt3_adapt_modifiable_immutable_to_return(label, v, flags);
        },     py::arg("label"), py::arg("v"), py::arg("flags") = 0);

    m.def("input_int4",    // imgui.h:594
        [](const char * label, std::array<int, 4> v, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<int, 4>>
        {
            auto InputInt4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<int, 4> v, ImGuiInputTextFlags flags = 0) -> std::tuple<bool, std::array<int, 4>>
            {
                int * v_adapt_modifiable = v.data();

                bool r = ImGui::InputInt4(label, v_adapt_modifiable, flags);
                return std::make_tuple(r, v);
            };

            return InputInt4_adapt_modifiable_immutable_to_return(label, v, flags);
        },     py::arg("label"), py::arg("v"), py::arg("flags") = 0);

    m.def("input_double",    // imgui.h:595
        [](const char * label, double v, double step = 0.0, double step_fast = 0.0, const char * format = "%.6f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, double>
        {
            auto InputDouble_adapt_modifiable_immutable_to_return = [](const char * label, double v, double step = 0.0, double step_fast = 0.0, const char * format = "%.6f", ImGuiInputTextFlags flags = 0) -> std::tuple<bool, double>
            {
                double * v_adapt_modifiable = & v;

                bool r = ImGui::InputDouble(label, v_adapt_modifiable, step, step_fast, format, flags);
                return std::make_tuple(r, v);
            };

            return InputDouble_adapt_modifiable_immutable_to_return(label, v, step, step_fast, format, flags);
        },     py::arg("label"), py::arg("v"), py::arg("step") = 0.0, py::arg("step_fast") = 0.0, py::arg("format") = "%.6f", py::arg("flags") = 0);

    m.def("input_scalar",    // imgui.h:596
        ImGui::InputScalar, py::arg("label"), py::arg("data_type"), py::arg("p_data"), py::arg("p_step") = py::none(), py::arg("p_step_fast") = py::none(), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("input_scalar_n",    // imgui.h:597
        ImGui::InputScalarN, py::arg("label"), py::arg("data_type"), py::arg("p_data"), py::arg("components"), py::arg("p_step") = py::none(), py::arg("p_step_fast") = py::none(), py::arg("format") = py::none(), py::arg("flags") = 0);

    m.def("color_edit3",    // imgui.h:602
        [](const char * label, std::array<float, 3> col, ImGuiColorEditFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
        {
            auto ColorEdit3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 3> col, ImGuiColorEditFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
            {
                float * col_adapt_modifiable = col.data();

                bool r = ImGui::ColorEdit3(label, col_adapt_modifiable, flags);
                return std::make_tuple(r, col);
            };

            return ColorEdit3_adapt_modifiable_immutable_to_return(label, col, flags);
        },     py::arg("label"), py::arg("col"), py::arg("flags") = 0);

    m.def("color_edit4",    // imgui.h:603
        [](const char * label, std::array<float, 4> col, ImGuiColorEditFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
        {
            auto ColorEdit4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 4> col, ImGuiColorEditFlags flags = 0) -> std::tuple<bool, std::array<float, 4>>
            {
                float * col_adapt_modifiable = col.data();

                bool r = ImGui::ColorEdit4(label, col_adapt_modifiable, flags);
                return std::make_tuple(r, col);
            };

            return ColorEdit4_adapt_modifiable_immutable_to_return(label, col, flags);
        },     py::arg("label"), py::arg("col"), py::arg("flags") = 0);

    m.def("color_picker3",    // imgui.h:604
        [](const char * label, std::array<float, 3> col, ImGuiColorEditFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
        {
            auto ColorPicker3_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 3> col, ImGuiColorEditFlags flags = 0) -> std::tuple<bool, std::array<float, 3>>
            {
                float * col_adapt_modifiable = col.data();

                bool r = ImGui::ColorPicker3(label, col_adapt_modifiable, flags);
                return std::make_tuple(r, col);
            };

            return ColorPicker3_adapt_modifiable_immutable_to_return(label, col, flags);
        },     py::arg("label"), py::arg("col"), py::arg("flags") = 0);

    m.def("color_picker4",    // imgui.h:605
        [](const char * label, std::array<float, 4> col, ImGuiColorEditFlags flags = 0, const float * ref_col = NULL) -> std::tuple<bool, std::array<float, 4>>
        {
            auto ColorPicker4_adapt_modifiable_immutable_to_return = [](const char * label, std::array<float, 4> col, ImGuiColorEditFlags flags = 0, const float * ref_col = NULL) -> std::tuple<bool, std::array<float, 4>>
            {
                float * col_adapt_modifiable = col.data();

                bool r = ImGui::ColorPicker4(label, col_adapt_modifiable, flags, ref_col);
                return std::make_tuple(r, col);
            };

            return ColorPicker4_adapt_modifiable_immutable_to_return(label, col, flags, ref_col);
        },     py::arg("label"), py::arg("col"), py::arg("flags") = 0, py::arg("ref_col") = py::none());

    m.def("color_button",    // imgui.h:606
        ImGui::ColorButton,
        py::arg("desc_id"), py::arg("col"), py::arg("flags") = 0, py::arg("size") = ImVec2(0, 0),
        "display a color square/button, hover for details, return True when pressed.");

    m.def("set_color_edit_options",    // imgui.h:607
        ImGui::SetColorEditOptions,
        py::arg("flags"),
        "initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.");

    m.def("tree_node",    // imgui.h:611
        py::overload_cast<const char *>(ImGui::TreeNode), py::arg("label"));

    m.def("tree_node",    // imgui.h:612
        [](const char * str_id, const char * fmt) -> bool
        {
            auto TreeNode_adapt_variadic_format = [](const char * str_id, const char * fmt) -> bool
            {
                auto r = ImGui::TreeNode(str_id, "%s", fmt);
                return r;
            };

            return TreeNode_adapt_variadic_format(str_id, fmt);
        },
        py::arg("str_id"), py::arg("fmt"),
        "helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().");

    m.def("tree_node",    // imgui.h:613
        [](const void * ptr_id, const char * fmt) -> bool
        {
            auto TreeNode_adapt_variadic_format = [](const void * ptr_id, const char * fmt) -> bool
            {
                auto r = ImGui::TreeNode(ptr_id, "%s", fmt);
                return r;
            };

            return TreeNode_adapt_variadic_format(ptr_id, fmt);
        },
        py::arg("ptr_id"), py::arg("fmt"),
        "\"");

    m.def("tree_node_ex",    // imgui.h:616
        py::overload_cast<const char *, ImGuiTreeNodeFlags>(ImGui::TreeNodeEx), py::arg("label"), py::arg("flags") = 0);

    m.def("tree_node_ex",    // imgui.h:617
        [](const char * str_id, ImGuiTreeNodeFlags flags, const char * fmt) -> bool
        {
            auto TreeNodeEx_adapt_variadic_format = [](const char * str_id, ImGuiTreeNodeFlags flags, const char * fmt) -> bool
            {
                auto r = ImGui::TreeNodeEx(str_id, flags, "%s", fmt);
                return r;
            };

            return TreeNodeEx_adapt_variadic_format(str_id, flags, fmt);
        },     py::arg("str_id"), py::arg("flags"), py::arg("fmt"));

    m.def("tree_node_ex",    // imgui.h:618
        [](const void * ptr_id, ImGuiTreeNodeFlags flags, const char * fmt) -> bool
        {
            auto TreeNodeEx_adapt_variadic_format = [](const void * ptr_id, ImGuiTreeNodeFlags flags, const char * fmt) -> bool
            {
                auto r = ImGui::TreeNodeEx(ptr_id, flags, "%s", fmt);
                return r;
            };

            return TreeNodeEx_adapt_variadic_format(ptr_id, flags, fmt);
        },     py::arg("ptr_id"), py::arg("flags"), py::arg("fmt"));

    m.def("tree_push",    // imgui.h:621
        py::overload_cast<const char *>(ImGui::TreePush),
        py::arg("str_id"),
        "~ Indent()+PushId(). Already called by TreeNode() when returning True, but you can call TreePush/TreePop yourself if desired.");

    m.def("tree_push",    // imgui.h:622
        py::overload_cast<const void *>(ImGui::TreePush),
        py::arg("ptr_id"),
        "\"");

    m.def("tree_pop",    // imgui.h:623
        ImGui::TreePop, "~ Unindent()+PopId()");

    m.def("get_tree_node_to_label_spacing",    // imgui.h:624
        ImGui::GetTreeNodeToLabelSpacing, "horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode");

    m.def("collapsing_header",    // imgui.h:625
        py::overload_cast<const char *, ImGuiTreeNodeFlags>(ImGui::CollapsingHeader),
        py::arg("label"), py::arg("flags") = 0,
        "if returning 'True' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().");

    m.def("collapsing_header",    // imgui.h:626
        [](const char * label, bool p_visible, ImGuiTreeNodeFlags flags = 0) -> std::tuple<bool, bool>
        {
            auto CollapsingHeader_adapt_modifiable_immutable_to_return = [](const char * label, bool p_visible, ImGuiTreeNodeFlags flags = 0) -> std::tuple<bool, bool>
            {
                bool * p_visible_adapt_modifiable = & p_visible;

                bool r = ImGui::CollapsingHeader(label, p_visible_adapt_modifiable, flags);
                return std::make_tuple(r, p_visible);
            };

            return CollapsingHeader_adapt_modifiable_immutable_to_return(label, p_visible, flags);
        },
        py::arg("label"), py::arg("p_visible"), py::arg("flags") = 0,
        "when 'p_visible != None': if '*p_visible==True' display an additional small close button on upper right of the header which will set the bool to False when clicked, if '*p_visible==False' don't display the header.");

    m.def("set_next_item_open",    // imgui.h:627
        ImGui::SetNextItemOpen,
        py::arg("is_open"), py::arg("cond") = 0,
        "set next TreeNode/CollapsingHeader open state.");

    m.def("selectable",    // imgui.h:632
        py::overload_cast<const char *, bool, ImGuiSelectableFlags, const ImVec2 &>(ImGui::Selectable),
        py::arg("label"), py::arg("selected") = false, py::arg("flags") = 0, py::arg("size") = ImVec2(0, 0),
        "\"bool selected\" carry the selection state (read-only). Selectable() is clicked is returns True so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height");

    m.def("selectable",    // imgui.h:633
        [](const char * label, bool p_selected, ImGuiSelectableFlags flags = 0, const ImVec2 & size = ImVec2(0, 0)) -> std::tuple<bool, bool>
        {
            auto Selectable_adapt_modifiable_immutable_to_return = [](const char * label, bool p_selected, ImGuiSelectableFlags flags = 0, const ImVec2 & size = ImVec2(0, 0)) -> std::tuple<bool, bool>
            {
                bool * p_selected_adapt_modifiable = & p_selected;

                bool r = ImGui::Selectable(label, p_selected_adapt_modifiable, flags, size);
                return std::make_tuple(r, p_selected);
            };

            return Selectable_adapt_modifiable_immutable_to_return(label, p_selected, flags, size);
        },
        py::arg("label"), py::arg("p_selected"), py::arg("flags") = 0, py::arg("size") = ImVec2(0, 0),
        "\"bool* p_selected\" point to the selection state (read-write), as a convenient helper.");

    m.def("begin_list_box",    // imgui.h:641
        ImGui::BeginListBox,
        py::arg("label"), py::arg("size") = ImVec2(0, 0),
        "open a framed scrolling region");

    m.def("end_list_box",    // imgui.h:642
        ImGui::EndListBox, "only call EndListBox() if BeginListBox() returned True!");

    m.def("list_box",    // imgui.h:643
        [](const char * label, int current_item, const std::vector<std::string> & items, int height_in_items = -1) -> std::tuple<bool, int>
        {
            auto ListBox_adapt_modifiable_immutable_to_return = [](const char * label, int current_item, const char * const items[], int items_count, int height_in_items = -1) -> std::tuple<bool, int>
            {
                int * current_item_adapt_modifiable = & current_item;

                bool r = ImGui::ListBox(label, current_item_adapt_modifiable, items, items_count, height_in_items);
                return std::make_tuple(r, current_item);
            };
            auto ListBox_adapt_c_string_list = [&ListBox_adapt_modifiable_immutable_to_return](const char * label, int current_item, const std::vector<std::string> & items, int height_in_items = -1) -> std::tuple<bool, int>
            {
                std::vector<const char *> items_ptrs;
                for (const auto& v: items)
                    items_ptrs.push_back(v.c_str());
                int items_count = static_cast<int>(items.size());

                auto r = ListBox_adapt_modifiable_immutable_to_return(label, current_item, items_ptrs.data(), items_count, height_in_items);
                return r;
            };

            return ListBox_adapt_c_string_list(label, current_item, items, height_in_items);
        },     py::arg("label"), py::arg("current_item"), py::arg("items"), py::arg("height_in_items") = -1);

    m.def("plot_lines",    // imgui.h:648
        [](const char * label, const py::array & values, int values_offset = 0, const char * overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = -1)
        {
            auto PlotLines_adapt_c_buffers = [](const char * label, const py::array & values, int values_offset = 0, const char * overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = -1)
            {
                // convert py::array to C standard buffer (const)
                const void * values_from_pyarray = values.data();
                py::ssize_t values_count = values.shape()[0];
                char values_type = values.dtype().char_();
                if (values_type != 'f')
                    throw std::runtime_error(std::string(R"msg(
                            Bad type!  Expected a numpy array of native type:
                                        const float *
                                    Which is equivalent to
                                        f
                                    (using py::array::dtype().char_() as an id)
                        )msg"));

                // process stride default value (which was a sizeof in C++)
                int values_stride = stride;
                if (values_stride == -1)
                    values_stride = (int)values.itemsize();

                ImGui::PlotLines(label, static_cast<const float *>(values_from_pyarray), static_cast<int>(values_count), values_offset, overlay_text, scale_min, scale_max, graph_size, values_stride);
            };

            PlotLines_adapt_c_buffers(label, values, values_offset, overlay_text, scale_min, scale_max, graph_size, stride);
        },     py::arg("label"), py::arg("values"), py::arg("values_offset") = 0, py::arg("overlay_text") = py::none(), py::arg("scale_min") = FLT_MAX, py::arg("scale_max") = FLT_MAX, py::arg("graph_size") = ImVec2(0, 0), py::arg("stride") = -1);

    m.def("plot_histogram",    // imgui.h:650
        [](const char * label, const py::array & values, int values_offset = 0, const char * overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = -1)
        {
            auto PlotHistogram_adapt_c_buffers = [](const char * label, const py::array & values, int values_offset = 0, const char * overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = -1)
            {
                // convert py::array to C standard buffer (const)
                const void * values_from_pyarray = values.data();
                py::ssize_t values_count = values.shape()[0];
                char values_type = values.dtype().char_();
                if (values_type != 'f')
                    throw std::runtime_error(std::string(R"msg(
                            Bad type!  Expected a numpy array of native type:
                                        const float *
                                    Which is equivalent to
                                        f
                                    (using py::array::dtype().char_() as an id)
                        )msg"));

                // process stride default value (which was a sizeof in C++)
                int values_stride = stride;
                if (values_stride == -1)
                    values_stride = (int)values.itemsize();

                ImGui::PlotHistogram(label, static_cast<const float *>(values_from_pyarray), static_cast<int>(values_count), values_offset, overlay_text, scale_min, scale_max, graph_size, values_stride);
            };

            PlotHistogram_adapt_c_buffers(label, values, values_offset, overlay_text, scale_min, scale_max, graph_size, stride);
        },     py::arg("label"), py::arg("values"), py::arg("values_offset") = 0, py::arg("overlay_text") = py::none(), py::arg("scale_min") = FLT_MAX, py::arg("scale_max") = FLT_MAX, py::arg("graph_size") = ImVec2(0, 0), py::arg("stride") = -1);

    m.def("value",    // imgui.h:655
        py::overload_cast<const char *, bool>(ImGui::Value), py::arg("prefix"), py::arg("b"));

    m.def("value",    // imgui.h:656
        py::overload_cast<const char *, int>(ImGui::Value), py::arg("prefix"), py::arg("v"));

    m.def("value",    // imgui.h:657
        py::overload_cast<const char *, unsigned int>(ImGui::Value), py::arg("prefix"), py::arg("v"));

    m.def("value",    // imgui.h:658
        py::overload_cast<const char *, float, const char *>(ImGui::Value), py::arg("prefix"), py::arg("v"), py::arg("float_format") = py::none());

    m.def("begin_menu_bar",    // imgui.h:665
        ImGui::BeginMenuBar, "append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).");

    m.def("end_menu_bar",    // imgui.h:666
        ImGui::EndMenuBar, "only call EndMenuBar() if BeginMenuBar() returns True!");

    m.def("begin_main_menu_bar",    // imgui.h:667
        ImGui::BeginMainMenuBar, "create and append to a full screen menu-bar.");

    m.def("end_main_menu_bar",    // imgui.h:668
        ImGui::EndMainMenuBar, "only call EndMainMenuBar() if BeginMainMenuBar() returns True!");

    m.def("begin_menu",    // imgui.h:669
        ImGui::BeginMenu,
        py::arg("label"), py::arg("enabled") = true,
        "create a sub-menu entry. only call EndMenu() if this returns True!");

    m.def("end_menu",    // imgui.h:670
        ImGui::EndMenu, "only call EndMenu() if BeginMenu() returns True!");

    m.def("menu_item",    // imgui.h:671
        py::overload_cast<const char *, const char *, bool, bool>(ImGui::MenuItem),
        py::arg("label"), py::arg("shortcut") = py::none(), py::arg("selected") = false, py::arg("enabled") = true,
        "return True when activated.");

    m.def("menu_item",    // imgui.h:672
        [](const char * label, const char * shortcut, bool p_selected, bool enabled = true) -> std::tuple<bool, bool>
        {
            auto MenuItem_adapt_modifiable_immutable_to_return = [](const char * label, const char * shortcut, bool p_selected, bool enabled = true) -> std::tuple<bool, bool>
            {
                bool * p_selected_adapt_modifiable = & p_selected;

                bool r = ImGui::MenuItem(label, shortcut, p_selected_adapt_modifiable, enabled);
                return std::make_tuple(r, p_selected);
            };

            return MenuItem_adapt_modifiable_immutable_to_return(label, shortcut, p_selected, enabled);
        },
        py::arg("label"), py::arg("shortcut"), py::arg("p_selected"), py::arg("enabled") = true,
        "return True when activated + toggle (*p_selected) if p_selected != None");

    m.def("begin_tooltip",    // imgui.h:676
        ImGui::BeginTooltip, "begin/append a tooltip window. to create full-featured tooltip (with any kind of items).");

    m.def("end_tooltip",    // imgui.h:677
        ImGui::EndTooltip);

    m.def("set_tooltip",    // imgui.h:678
        [](const char * fmt)
        {
            auto SetTooltip_adapt_variadic_format = [](const char * fmt)
            {
                ImGui::SetTooltip("%s", fmt);
            };

            SetTooltip_adapt_variadic_format(fmt);
        },
        py::arg("fmt"),
        "set a text-only tooltip, typically use with ImGui::IsItemHovered(). override any previous call to SetTooltip().");

    m.def("begin_popup",    // imgui.h:693
        py::overload_cast<const char *, ImGuiWindowFlags>(ImGui::BeginPopup),
        py::arg("str_id"), py::arg("flags") = 0,
        "return True if the popup is open, and you can start outputting to it.");

    m.def("begin_popup_modal",    // imgui.h:694
        [](const char * name, std::optional<bool> p_open = std::nullopt, ImGuiWindowFlags flags = 0) -> std::tuple<bool, std::optional<bool>>
        {
            auto BeginPopupModal_adapt_modifiable_immutable_to_return = [](const char * name, std::optional<bool> p_open = std::nullopt, ImGuiWindowFlags flags = 0) -> std::tuple<bool, std::optional<bool>>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                bool r = ImGui::BeginPopupModal(name, p_open_adapt_modifiable, flags);
                return std::make_tuple(r, p_open);
            };

            return BeginPopupModal_adapt_modifiable_immutable_to_return(name, p_open, flags);
        },
        py::arg("name"), py::arg("p_open") = py::none(), py::arg("flags") = 0,
        "return True if the modal is open, and you can start outputting to it.");

    m.def("end_popup",    // imgui.h:695
        py::overload_cast<>(ImGui::EndPopup), "only call EndPopup() if BeginPopupXXX() returns True!");

    m.def("open_popup",    // imgui.h:705
        py::overload_cast<const char *, ImGuiPopupFlags>(ImGui::OpenPopup),
        py::arg("str_id"), py::arg("popup_flags") = 0,
        "call to mark popup as open (don't call every frame!).");

    m.def("open_popup",    // imgui.h:706
        py::overload_cast<ImGuiID, ImGuiPopupFlags>(ImGui::OpenPopup),
        py::arg("id_"), py::arg("popup_flags") = 0,
        "id overload to facilitate calling from nested stacks");

    m.def("open_popup_on_item_click",    // imgui.h:707
        py::overload_cast<const char *, ImGuiPopupFlags>(ImGui::OpenPopupOnItemClick),
        py::arg("str_id") = py::none(), py::arg("popup_flags") = 1,
        "helper to open popup when clicked on last item. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)");

    m.def("close_current_popup",    // imgui.h:708
        py::overload_cast<>(ImGui::CloseCurrentPopup), "manually close the popup we have begin-ed into.");

    m.def("begin_popup_context_item",    // imgui.h:715
        py::overload_cast<const char *, ImGuiPopupFlags>(ImGui::BeginPopupContextItem),
        py::arg("str_id") = py::none(), py::arg("popup_flags") = 1,
        "open+begin popup when clicked on last item. Use str_id==None to associate the popup to previous item. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!");

    m.def("begin_popup_context_window",    // imgui.h:716
        py::overload_cast<const char *, ImGuiPopupFlags>(ImGui::BeginPopupContextWindow),
        py::arg("str_id") = py::none(), py::arg("popup_flags") = 1,
        "open+begin popup when clicked on current window.");

    m.def("begin_popup_context_void",    // imgui.h:717
        py::overload_cast<const char *, ImGuiPopupFlags>(ImGui::BeginPopupContextVoid),
        py::arg("str_id") = py::none(), py::arg("popup_flags") = 1,
        "open+begin popup when clicked in None (where there are no windows).");

    m.def("is_popup_open",    // imgui.h:723
        py::overload_cast<const char *, ImGuiPopupFlags>(ImGui::IsPopupOpen),
        py::arg("str_id"), py::arg("flags") = 0,
        "return True if the popup is open.");

    m.def("begin_table",    // imgui.h:748
        ImGui::BeginTable, py::arg("str_id"), py::arg("column"), py::arg("flags") = 0, py::arg("outer_size") = ImVec2(0.0f, 0.0f), py::arg("inner_width") = 0.0f);

    m.def("end_table",    // imgui.h:749
        ImGui::EndTable, "only call EndTable() if BeginTable() returns True!");

    m.def("table_next_row",    // imgui.h:750
        py::overload_cast<ImGuiTableRowFlags, float>(ImGui::TableNextRow),
        py::arg("row_flags") = 0, py::arg("min_row_height") = 0.0f,
        "append into the first cell of a new row.");

    m.def("table_next_column",    // imgui.h:751
        py::overload_cast<>(ImGui::TableNextColumn), "append into the next column (or first column of next row if currently in last column). Return True when column is visible.");

    m.def("table_set_column_index",    // imgui.h:752
        py::overload_cast<int>(ImGui::TableSetColumnIndex),
        py::arg("column_n"),
        "append into the specified column. Return True when column is visible.");

    m.def("table_setup_column",    // imgui.h:762
        py::overload_cast<const char *, ImGuiTableColumnFlags, float, ImGuiID>(ImGui::TableSetupColumn), py::arg("label"), py::arg("flags") = 0, py::arg("init_width_or_weight") = 0.0f, py::arg("user_id") = 0);

    m.def("table_setup_scroll_freeze",    // imgui.h:763
        py::overload_cast<int, int>(ImGui::TableSetupScrollFreeze),
        py::arg("cols"), py::arg("rows"),
        "lock columns/rows so they stay visible when scrolled.");

    m.def("table_headers_row",    // imgui.h:764
        py::overload_cast<>(ImGui::TableHeadersRow), "submit all headers cells based on data provided to TableSetupColumn() + submit context menu");

    m.def("table_header",    // imgui.h:765
        py::overload_cast<const char *>(ImGui::TableHeader),
        py::arg("label"),
        "submit one header cell manually (rarely used)");

    m.def("table_get_sort_specs",    // imgui.h:773
        py::overload_cast<>(ImGui::TableGetSortSpecs),
        "get latest sort specs for the table (None if not sorting).  Lifetime: don't hold on this pointer over multiple frames or past any subsequent call to BeginTable().",
        pybind11::return_value_policy::reference);

    m.def("table_get_column_count",    // imgui.h:774
        py::overload_cast<>(ImGui::TableGetColumnCount), "return number of columns (value passed to BeginTable)");

    m.def("table_get_column_index",    // imgui.h:775
        py::overload_cast<>(ImGui::TableGetColumnIndex), "return current column index.");

    m.def("table_get_row_index",    // imgui.h:776
        py::overload_cast<>(ImGui::TableGetRowIndex), "return current row index.");

    m.def("table_get_column_name",    // imgui.h:777
        py::overload_cast<int>(ImGui::TableGetColumnName),
        py::arg("column_n") = -1,
        "return \"\" if column didn't have a name declared by TableSetupColumn(). Pass -1 to use current column.",
        pybind11::return_value_policy::reference);

    m.def("table_get_column_flags",    // imgui.h:778
        py::overload_cast<int>(ImGui::TableGetColumnFlags),
        py::arg("column_n") = -1,
        "return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass -1 to use current column.");

    m.def("table_set_column_enabled",    // imgui.h:779
        py::overload_cast<int, bool>(ImGui::TableSetColumnEnabled),
        py::arg("column_n"), py::arg("v"),
        "change user accessible enabled/disabled state of a column. Set to False to hide the column. User can use the context menu to change this themselves (right-click in headers, or right-click in columns body with ImGuiTableFlags_ContextMenuInBody)");

    m.def("table_set_bg_color",    // imgui.h:780
        py::overload_cast<ImGuiTableBgTarget, ImU32, int>(ImGui::TableSetBgColor),
        py::arg("target"), py::arg("color"), py::arg("column_n") = -1,
        "change the color of a cell, row, or column. See ImGuiTableBgTarget_ flags for details.");

    m.def("columns",    // imgui.h:784
        ImGui::Columns, py::arg("count") = 1, py::arg("id_") = py::none(), py::arg("border") = true);

    m.def("next_column",    // imgui.h:785
        ImGui::NextColumn, "next column, defaults to current row or next row if the current row is finished");

    m.def("get_column_index",    // imgui.h:786
        ImGui::GetColumnIndex, "get current column index");

    m.def("get_column_width",    // imgui.h:787
        ImGui::GetColumnWidth,
        py::arg("column_index") = -1,
        "get column width (in pixels). pass -1 to use current column");

    m.def("set_column_width",    // imgui.h:788
        ImGui::SetColumnWidth,
        py::arg("column_index"), py::arg("width"),
        "set column width (in pixels). pass -1 to use current column");

    m.def("get_column_offset",    // imgui.h:789
        ImGui::GetColumnOffset,
        py::arg("column_index") = -1,
        "get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0");

    m.def("set_column_offset",    // imgui.h:790
        ImGui::SetColumnOffset,
        py::arg("column_index"), py::arg("offset_x"),
        "set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column");

    m.def("get_columns_count",    // imgui.h:791
        ImGui::GetColumnsCount);

    m.def("begin_tab_bar",    // imgui.h:795
        ImGui::BeginTabBar,
        py::arg("str_id"), py::arg("flags") = 0,
        "create and append into a TabBar");

    m.def("end_tab_bar",    // imgui.h:796
        ImGui::EndTabBar, "only call EndTabBar() if BeginTabBar() returns True!");

    m.def("begin_tab_item",    // imgui.h:797
        [](const char * label, std::optional<bool> p_open = std::nullopt, ImGuiTabItemFlags flags = 0) -> std::tuple<bool, std::optional<bool>>
        {
            auto BeginTabItem_adapt_modifiable_immutable_to_return = [](const char * label, std::optional<bool> p_open = std::nullopt, ImGuiTabItemFlags flags = 0) -> std::tuple<bool, std::optional<bool>>
            {
                bool * p_open_adapt_modifiable = nullptr;
                if (p_open.has_value())
                    p_open_adapt_modifiable = & (*p_open);

                bool r = ImGui::BeginTabItem(label, p_open_adapt_modifiable, flags);
                return std::make_tuple(r, p_open);
            };

            return BeginTabItem_adapt_modifiable_immutable_to_return(label, p_open, flags);
        },
        py::arg("label"), py::arg("p_open") = py::none(), py::arg("flags") = 0,
        "create a Tab. Returns True if the Tab is selected.");

    m.def("end_tab_item",    // imgui.h:798
        ImGui::EndTabItem, "only call EndTabItem() if BeginTabItem() returns True!");

    m.def("tab_item_button",    // imgui.h:799
        ImGui::TabItemButton,
        py::arg("label"), py::arg("flags") = 0,
        "create a Tab behaving like a button. return True when clicked. cannot be selected in the tab bar.");

    m.def("set_tab_item_closed",    // imgui.h:800
        ImGui::SetTabItemClosed,
        py::arg("tab_or_docked_window_label"),
        "notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.");

    m.def("dock_space",    // imgui.h:815
        ImGui::DockSpace, py::arg("id_"), py::arg("size") = ImVec2(0, 0), py::arg("flags") = 0, py::arg("window_class") = py::none());

    m.def("dock_space_over_viewport",    // imgui.h:816
        ImGui::DockSpaceOverViewport, py::arg("viewport") = py::none(), py::arg("flags") = 0, py::arg("window_class") = py::none());

    m.def("set_next_window_dock_id",    // imgui.h:817
        ImGui::SetNextWindowDockID,
        py::arg("dock_id"), py::arg("cond") = 0,
        "set next window dock id");

    m.def("set_next_window_class",    // imgui.h:818
        ImGui::SetNextWindowClass,
        py::arg("window_class"),
        "set next window class (control docking compatibility + provide hints to platform backend via custom viewport flags and platform parent/child relationship)");

    m.def("get_window_dock_id",    // imgui.h:819
        ImGui::GetWindowDockID);

    m.def("is_window_docked",    // imgui.h:820
        ImGui::IsWindowDocked, "is current window docked into another window?");

    m.def("log_to_tty",    // imgui.h:824
        ImGui::LogToTTY,
        py::arg("auto_open_depth") = -1,
        "start logging to tty (stdout)");

    m.def("log_to_file",    // imgui.h:825
        ImGui::LogToFile,
        py::arg("auto_open_depth") = -1, py::arg("filename") = py::none(),
        "start logging to file");

    m.def("log_to_clipboard",    // imgui.h:826
        ImGui::LogToClipboard,
        py::arg("auto_open_depth") = -1,
        "start logging to OS clipboard");

    m.def("log_finish",    // imgui.h:827
        ImGui::LogFinish, "stop logging (close file, etc.)");

    m.def("log_buttons",    // imgui.h:828
        ImGui::LogButtons, "helper to display buttons for logging to tty/file/clipboard");

    m.def("log_text",    // imgui.h:829
        [](const char * fmt)
        {
            auto LogText_adapt_variadic_format = [](const char * fmt)
            {
                ImGui::LogText("%s", fmt);
            };

            LogText_adapt_variadic_format(fmt);
        },
        py::arg("fmt"),
        "pass text data straight to log (without being displayed)");

    m.def("begin_drag_drop_source",    // imgui.h:837
        ImGui::BeginDragDropSource,
        py::arg("flags") = 0,
        "call after submitting an item which may be dragged. when this return True, you can call SetDragDropPayload() + EndDragDropSource()");

    m.def("set_drag_drop_payload",    // imgui.h:838
        ImGui::SetDragDropPayload,
        py::arg("type"), py::arg("data"), py::arg("sz"), py::arg("cond") = 0,
        "type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear imgui internal types. Data is copied and held by imgui. Return True when payload has been accepted.");

    m.def("end_drag_drop_source",    // imgui.h:839
        ImGui::EndDragDropSource, "only call EndDragDropSource() if BeginDragDropSource() returns True!");

    m.def("begin_drag_drop_target",    // imgui.h:840
        ImGui::BeginDragDropTarget, "call after submitting an item that may receive a payload. If this returns True, you can call AcceptDragDropPayload() + EndDragDropTarget()");

    m.def("accept_drag_drop_payload",    // imgui.h:841
        ImGui::AcceptDragDropPayload,
        py::arg("type"), py::arg("flags") = 0,
        "accept contents of a given type. If ImGuiDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.",
        pybind11::return_value_policy::reference);

    m.def("end_drag_drop_target",    // imgui.h:842
        ImGui::EndDragDropTarget, "only call EndDragDropTarget() if BeginDragDropTarget() returns True!");

    m.def("get_drag_drop_payload",    // imgui.h:843
        ImGui::GetDragDropPayload,
        "peek directly into the current payload from anywhere. may return None. use ImGuiPayload::IsDataType() to test for the payload type.",
        pybind11::return_value_policy::reference);

    m.def("begin_disabled",    // imgui.h:849
        ImGui::BeginDisabled, py::arg("disabled") = true);

    m.def("end_disabled",    // imgui.h:850
        ImGui::EndDisabled);

    m.def("push_clip_rect",    // imgui.h:854
        ImGui::PushClipRect, py::arg("clip_rect_min"), py::arg("clip_rect_max"), py::arg("intersect_with_current_clip_rect"));

    m.def("pop_clip_rect",    // imgui.h:855
        ImGui::PopClipRect);

    m.def("set_item_default_focus",    // imgui.h:859
        ImGui::SetItemDefaultFocus, "make last item the default focused item of a window.");

    m.def("set_keyboard_focus_here",    // imgui.h:860
        ImGui::SetKeyboardFocusHere,
        py::arg("offset") = 0,
        "focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.");

    m.def("is_item_hovered",    // imgui.h:865
        ImGui::IsItemHovered,
        py::arg("flags") = 0,
        "is the last item hovered? (and usable, aka not blocked by a popup, etc.). See ImGuiHoveredFlags for more options.");

    m.def("is_item_active",    // imgui.h:866
        ImGui::IsItemActive, "is the last item active? (e.g. button being held, text field being edited. This will continuously return True while holding mouse button on an item. Items that don't interact will always return False)");

    m.def("is_item_focused",    // imgui.h:867
        ImGui::IsItemFocused, "is the last item focused for keyboard/gamepad navigation?");

    m.def("is_item_clicked",    // imgui.h:868
        ImGui::IsItemClicked,
        py::arg("mouse_button") = 0,
        "is the last item hovered and mouse clicked on? (**)  == IsMouseClicked(mouse_button) && IsItemHovered()Important. (**) this is NOT equivalent to the behavior of e.g. Button(). Read comments in function definition.");

    m.def("is_item_visible",    // imgui.h:869
        ImGui::IsItemVisible, "is the last item visible? (items may be out of sight because of clipping/scrolling)");

    m.def("is_item_edited",    // imgui.h:870
        ImGui::IsItemEdited, "did the last item modify its underlying value this frame? or was pressed? This is generally the same as the \"bool\" return value of many widgets.");

    m.def("is_item_activated",    // imgui.h:871
        ImGui::IsItemActivated, "was the last item just made active (item was previously inactive).");

    m.def("is_item_deactivated",    // imgui.h:872
        ImGui::IsItemDeactivated, "was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that require continuous editing.");

    m.def("is_item_deactivated_after_edit",    // imgui.h:873
        ImGui::IsItemDeactivatedAfterEdit, "was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that require continuous editing. Note that you may get False positives (some widgets such as Combo()/ListBox()/Selectable() will return True even when clicking an already selected item).");

    m.def("is_item_toggled_open",    // imgui.h:874
        ImGui::IsItemToggledOpen, "was the last item open state toggled? set by TreeNode().");

    m.def("is_any_item_hovered",    // imgui.h:875
        ImGui::IsAnyItemHovered, "is any item hovered?");

    m.def("is_any_item_active",    // imgui.h:876
        ImGui::IsAnyItemActive, "is any item active?");

    m.def("is_any_item_focused",    // imgui.h:877
        ImGui::IsAnyItemFocused, "is any item focused?");

    m.def("get_item_rect_min",    // imgui.h:878
        ImGui::GetItemRectMin, "get upper-left bounding rectangle of the last item (screen space)");

    m.def("get_item_rect_max",    // imgui.h:879
        ImGui::GetItemRectMax, "get lower-right bounding rectangle of the last item (screen space)");

    m.def("get_item_rect_size",    // imgui.h:880
        ImGui::GetItemRectSize, "get size of last item");

    m.def("set_item_allow_overlap",    // imgui.h:881
        ImGui::SetItemAllowOverlap, "allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.");

    m.def("get_main_viewport",    // imgui.h:887
        ImGui::GetMainViewport,
        "return primary/default viewport. This can never be None.",
        pybind11::return_value_policy::reference);

    m.def("get_background_draw_list",    // imgui.h:890
        py::overload_cast<>(ImGui::GetBackgroundDrawList),
        "get background draw list for the viewport associated to the current window. this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.",
        pybind11::return_value_policy::reference);

    m.def("get_foreground_draw_list",    // imgui.h:891
        py::overload_cast<>(ImGui::GetForegroundDrawList),
        "get foreground draw list for the viewport associated to the current window. this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.",
        pybind11::return_value_policy::reference);

    m.def("get_background_draw_list",    // imgui.h:892
        py::overload_cast<ImGuiViewport *>(ImGui::GetBackgroundDrawList),
        py::arg("viewport"),
        "get background draw list for the given viewport. this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.",
        pybind11::return_value_policy::reference);

    m.def("get_foreground_draw_list",    // imgui.h:893
        py::overload_cast<ImGuiViewport *>(ImGui::GetForegroundDrawList),
        py::arg("viewport"),
        "get foreground draw list for the given viewport. this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.",
        pybind11::return_value_policy::reference);

    m.def("is_rect_visible",    // imgui.h:896
        py::overload_cast<const ImVec2 &>(ImGui::IsRectVisible),
        py::arg("size"),
        "test if rectangle (of given size, starting from cursor position) is visible / not clipped.");

    m.def("is_rect_visible",    // imgui.h:897
        py::overload_cast<const ImVec2 &, const ImVec2 &>(ImGui::IsRectVisible),
        py::arg("rect_min"), py::arg("rect_max"),
        "test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.");

    m.def("get_time",    // imgui.h:898
        ImGui::GetTime, "get global imgui time. incremented by io.DeltaTime every frame.");

    m.def("get_frame_count",    // imgui.h:899
        ImGui::GetFrameCount, "get global imgui frame count. incremented by 1 every frame.");

    m.def("get_draw_list_shared_data",    // imgui.h:900
        py::overload_cast<>(ImGui::GetDrawListSharedData),
        "you may use this when creating your own ImDrawList instances.",
        pybind11::return_value_policy::reference);

    m.def("get_style_color_name",    // imgui.h:901
        ImGui::GetStyleColorName,
        py::arg("idx"),
        "get a string corresponding to the enum value (for display, saving, etc.).",
        pybind11::return_value_policy::reference);

    m.def("set_state_storage",    // imgui.h:902
        ImGui::SetStateStorage,
        py::arg("storage"),
        "replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)");

    m.def("get_state_storage",    // imgui.h:903
        ImGui::GetStateStorage, pybind11::return_value_policy::reference);

    m.def("begin_child_frame",    // imgui.h:904
        ImGui::BeginChildFrame,
        py::arg("id_"), py::arg("size"), py::arg("flags") = 0,
        "helper to create a child window / scrolling region that looks like a normal widget frame");

    m.def("end_child_frame",    // imgui.h:905
        ImGui::EndChildFrame, "always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)");

    m.def("calc_text_size",    // imgui.h:908
        ImGui::CalcTextSize,
        py::arg("text"), py::arg("text_end") = py::none(), py::arg("hide_text_after_double_hash") = false, py::arg("wrap_width") = -1.0f,
        "Text Utilities");

    m.def("color_convert_u32_to_float4",    // imgui.h:911
        ImGui::ColorConvertU32ToFloat4, py::arg("in_"));

    m.def("color_convert_float4_to_u32",    // imgui.h:912
        ImGui::ColorConvertFloat4ToU32, py::arg("in_"));

    m.def("color_convert_rgb_to_hsv",    // imgui.h:913
        [](float r, float g, float b, float out_h, float out_s, float out_v) -> std::tuple<float, float, float>
        {
            auto ColorConvertRGBtoHSV_adapt_modifiable_immutable_to_return = [](float r, float g, float b, float out_h, float out_s, float out_v) -> std::tuple<float, float, float>
            {
                float & out_h_adapt_modifiable = out_h;
                float & out_s_adapt_modifiable = out_s;
                float & out_v_adapt_modifiable = out_v;

                ImGui::ColorConvertRGBtoHSV(r, g, b, out_h_adapt_modifiable, out_s_adapt_modifiable, out_v_adapt_modifiable);
                return std::make_tuple(out_h, out_s, out_v);
            };

            return ColorConvertRGBtoHSV_adapt_modifiable_immutable_to_return(r, g, b, out_h, out_s, out_v);
        },     py::arg("r"), py::arg("g"), py::arg("b"), py::arg("out_h"), py::arg("out_s"), py::arg("out_v"));

    m.def("color_convert_hsv_to_rgb",    // imgui.h:914
        [](float h, float s, float v, float out_r, float out_g, float out_b) -> std::tuple<float, float, float>
        {
            auto ColorConvertHSVtoRGB_adapt_modifiable_immutable_to_return = [](float h, float s, float v, float out_r, float out_g, float out_b) -> std::tuple<float, float, float>
            {
                float & out_r_adapt_modifiable = out_r;
                float & out_g_adapt_modifiable = out_g;
                float & out_b_adapt_modifiable = out_b;

                ImGui::ColorConvertHSVtoRGB(h, s, v, out_r_adapt_modifiable, out_g_adapt_modifiable, out_b_adapt_modifiable);
                return std::make_tuple(out_r, out_g, out_b);
            };

            return ColorConvertHSVtoRGB_adapt_modifiable_immutable_to_return(h, s, v, out_r, out_g, out_b);
        },     py::arg("h"), py::arg("s"), py::arg("v"), py::arg("out_r"), py::arg("out_g"), py::arg("out_b"));

    m.def("is_key_down",    // imgui.h:922
        py::overload_cast<ImGuiKey>(ImGui::IsKeyDown),
        py::arg("key"),
        "is key being held.");

    m.def("is_key_pressed",    // imgui.h:923
        py::overload_cast<ImGuiKey, bool>(ImGui::IsKeyPressed),
        py::arg("key"), py::arg("repeat") = true,
        "was key pressed (went from !Down to Down)? if repeat=True, uses io.KeyRepeatDelay / KeyRepeatRate");

    m.def("is_key_released",    // imgui.h:924
        py::overload_cast<ImGuiKey>(ImGui::IsKeyReleased),
        py::arg("key"),
        "was key released (went from Down to !Down)?");

    m.def("get_key_pressed_amount",    // imgui.h:925
        ImGui::GetKeyPressedAmount,
        py::arg("key"), py::arg("repeat_delay"), py::arg("rate"),
        "uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate");

    m.def("get_key_name",    // imgui.h:926
        ImGui::GetKeyName,
        py::arg("key"),
        "[DEBUG] returns English name of the key. Those names a provided for debugging purpose and are not meant to be saved persistently not compared.",
        pybind11::return_value_policy::reference);

    m.def("set_next_frame_want_capture_keyboard",    // imgui.h:927
        ImGui::SetNextFrameWantCaptureKeyboard,
        py::arg("want_capture_keyboard"),
        "Override io.WantCaptureKeyboard flag next frame (said flag is left for your application to handle, typically when True it instructs your app to ignore inputs). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting \"io.WantCaptureKeyboard = want_capture_keyboard\"; after the next NewFrame() call.");

    m.def("is_mouse_down",    // imgui.h:933
        py::overload_cast<ImGuiMouseButton>(ImGui::IsMouseDown),
        py::arg("button"),
        "is mouse button held?");

    m.def("is_mouse_clicked",    // imgui.h:934
        py::overload_cast<ImGuiMouseButton, bool>(ImGui::IsMouseClicked),
        py::arg("button"), py::arg("repeat") = false,
        "did mouse button clicked? (went from !Down to Down). Same as GetMouseClickedCount() == 1.");

    m.def("is_mouse_released",    // imgui.h:935
        py::overload_cast<ImGuiMouseButton>(ImGui::IsMouseReleased),
        py::arg("button"),
        "did mouse button released? (went from Down to !Down)");

    m.def("is_mouse_double_clicked",    // imgui.h:936
        py::overload_cast<ImGuiMouseButton>(ImGui::IsMouseDoubleClicked),
        py::arg("button"),
        "did mouse button double-clicked? Same as GetMouseClickedCount() == 2. (note that a double-click will also report IsMouseClicked() == True)");

    m.def("get_mouse_clicked_count",    // imgui.h:937
        ImGui::GetMouseClickedCount,
        py::arg("button"),
        "return the number of successive mouse-clicks at the time where a click happen (otherwise 0).");

    m.def("is_mouse_hovering_rect",    // imgui.h:938
        py::overload_cast<const ImVec2 &, const ImVec2 &, bool>(ImGui::IsMouseHoveringRect),
        py::arg("r_min"), py::arg("r_max"), py::arg("clip") = true,
        "is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.");

    m.def("is_mouse_pos_valid",    // imgui.h:939
        py::overload_cast<const ImVec2 *>(ImGui::IsMousePosValid),
        py::arg("mouse_pos") = py::none(),
        "by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse available");

    m.def("is_any_mouse_down",    // imgui.h:940
        ImGui::IsAnyMouseDown, "[WILL OBSOLETE] is any mouse button held? This was designed for backends, but prefer having backend maintain a mask of held mouse buttons, because upcoming input queue system will make this invalid.");

    m.def("get_mouse_pos",    // imgui.h:941
        ImGui::GetMousePos, "shortcut to ImGui::GetIO().MousePos provided by user, to be consistent with other calls");

    m.def("get_mouse_pos_on_opening_current_popup",    // imgui.h:942
        py::overload_cast<>(ImGui::GetMousePosOnOpeningCurrentPopup), "retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves)");

    m.def("is_mouse_dragging",    // imgui.h:943
        py::overload_cast<ImGuiMouseButton, float>(ImGui::IsMouseDragging),
        py::arg("button"), py::arg("lock_threshold") = -1.0f,
        "is mouse dragging? (if lock_threshold < -1.0, uses io.MouseDraggingThreshold)");

    m.def("get_mouse_drag_delta",    // imgui.h:944
        ImGui::GetMouseDragDelta,
        py::arg("button") = 0, py::arg("lock_threshold") = -1.0f,
        "return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0 until the mouse moves past a distance threshold at least once (if lock_threshold < -1.0, uses io.MouseDraggingThreshold)");

    m.def("reset_mouse_drag_delta",    // imgui.h:945
        ImGui::ResetMouseDragDelta, py::arg("button") = 0);

    m.def("get_mouse_cursor",    // imgui.h:946
        ImGui::GetMouseCursor, "get desired mouse cursor shape. Important: reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you");

    m.def("set_mouse_cursor",    // imgui.h:947
        ImGui::SetMouseCursor,
        py::arg("cursor_type"),
        "set desired mouse cursor shape");

    m.def("set_next_frame_want_capture_mouse",    // imgui.h:948
        ImGui::SetNextFrameWantCaptureMouse,
        py::arg("want_capture_mouse"),
        "Override io.WantCaptureMouse flag next frame (said flag is left for your application to handle, typical when True it instucts your app to ignore inputs). This is equivalent to setting \"io.WantCaptureMouse = want_capture_mouse;\" after the next NewFrame() call.");

    m.def("get_clipboard_text",    // imgui.h:952
        ImGui::GetClipboardText, pybind11::return_value_policy::reference);

    m.def("set_clipboard_text",    // imgui.h:953
        ImGui::SetClipboardText, py::arg("text"));

    m.def("load_ini_settings_from_disk",    // imgui.h:959
        ImGui::LoadIniSettingsFromDisk,
        py::arg("ini_filename"),
        "call after CreateContext() and before the first call to NewFrame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).");

    m.def("load_ini_settings_from_memory",    // imgui.h:960
        ImGui::LoadIniSettingsFromMemory,
        py::arg("ini_data"), py::arg("ini_size") = 0,
        "call after CreateContext() and before the first call to NewFrame() to provide .ini data from your own data source.");

    m.def("save_ini_settings_to_disk",    // imgui.h:961
        ImGui::SaveIniSettingsToDisk,
        py::arg("ini_filename"),
        "this is automatically called (if io.IniFilename is not empty) a few seconds after any modification that should be reflected in the .ini file (and also by DestroyContext).");

    m.def("save_ini_settings_to_memory",    // imgui.h:962
        ImGui::SaveIniSettingsToMemory,
        py::arg("out_ini_size") = py::none(),
        "return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.",
        pybind11::return_value_policy::reference);

    m.def("debug_text_encoding",    // imgui.h:965
        ImGui::DebugTextEncoding, py::arg("text"));

    m.def("debug_check_version_and_data_layout",    // imgui.h:966
        ImGui::DebugCheckVersionAndDataLayout,
        py::arg("version_str"), py::arg("sz_io"), py::arg("sz_style"), py::arg("sz_vec2"), py::arg("sz_vec4"), py::arg("sz_drawvert"), py::arg("sz_drawidx"),
        "This is called by IMGUI_CHECKVERSION() macro.");

    m.def("get_platform_io",    // imgui.h:980
        ImGui::GetPlatformIO,
        "platform/renderer functions, for backend to setup + viewports list.",
        pybind11::return_value_policy::reference);

    m.def("update_platform_windows",    // imgui.h:981
        ImGui::UpdatePlatformWindows, "call in main loop. will call CreateWindow/ResizeWindow/etc. platform functions for each secondary viewport, and DestroyWindow for each inactive viewport.");

    m.def("render_platform_windows_default",    // imgui.h:982
        ImGui::RenderPlatformWindowsDefault,
        py::arg("platform_render_arg") = py::none(), py::arg("renderer_render_arg") = py::none(),
        "call in main loop. will call RenderWindow/SwapBuffers platform functions for each secondary viewport which doesn't have the ImGuiViewportFlags_Minimized flag set. May be reimplemented by user for custom rendering needs.");

    m.def("destroy_platform_windows",    // imgui.h:983
        ImGui::DestroyPlatformWindows, "call DestroyWindow platform functions for all viewports. call from backend Shutdown() if you need to close platform windows before imgui shutdown. otherwise will be called by DestroyContext().");

    m.def("find_viewport_by_id",    // imgui.h:984
        ImGui::FindViewportByID,
        py::arg("id_"),
        "this is a helper for backends.",
        pybind11::return_value_policy::reference);

    m.def("find_viewport_by_platform_handle",    // imgui.h:985
        ImGui::FindViewportByPlatformHandle,
        py::arg("platform_handle"),
        "this is a helper for backends. the type platform_handle is decided by the backend (e.g. HWND, MyWindow*, GLFWwindow* etc.)",
        pybind11::return_value_policy::reference);


    py::enum_<ImGuiWindowFlags_>(m, "WindowFlags_", py::arithmetic(), " Flags for ImGui::Begin()\n (Those are per-window flags. There are shared flags in ImGuiIO: io.ConfigWindowsResizeFromEdges and io.ConfigWindowsMoveFromTitleBarOnly)")    // imgui.h:995
        .value("none", ImGuiWindowFlags_None, "")
        .value("no_title_bar", ImGuiWindowFlags_NoTitleBar, "Disable title-bar")
        .value("no_resize", ImGuiWindowFlags_NoResize, "Disable user resizing with the lower-right grip")
        .value("no_move", ImGuiWindowFlags_NoMove, "Disable user moving the window")
        .value("no_scrollbar", ImGuiWindowFlags_NoScrollbar, "Disable scrollbars (window can still scroll with mouse or programmatically)")
        .value("no_scroll_with_mouse", ImGuiWindowFlags_NoScrollWithMouse, "Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.")
        .value("no_collapse", ImGuiWindowFlags_NoCollapse, "Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).")
        .value("always_auto_resize", ImGuiWindowFlags_AlwaysAutoResize, "Resize every window to its content every frame")
        .value("no_background", ImGuiWindowFlags_NoBackground, "Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0).")
        .value("no_saved_settings", ImGuiWindowFlags_NoSavedSettings, "Never load/save settings in .ini file")
        .value("no_mouse_inputs", ImGuiWindowFlags_NoMouseInputs, "Disable catching mouse, hovering test with pass through.")
        .value("menu_bar", ImGuiWindowFlags_MenuBar, "Has a menu-bar")
        .value("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, "Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0)); prior to calling Begin() to specify width. Read code in imgui_demo in the \"Horizontal Scrolling\" section.")
        .value("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, "Disable taking focus when transitioning from hidden to visible state")
        .value("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, "Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)")
        .value("always_vertical_scrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar, "Always show vertical scrollbar (even if ContentSize.y < Size.y)")
        .value("always_horizontal_scrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar, "Always show horizontal scrollbar (even if ContentSize.x < Size.x)")
        .value("always_use_window_padding", ImGuiWindowFlags_AlwaysUseWindowPadding, "Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)")
        .value("no_nav_inputs", ImGuiWindowFlags_NoNavInputs, "No gamepad/keyboard navigation within the window")
        .value("no_nav_focus", ImGuiWindowFlags_NoNavFocus, "No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)")
        .value("unsaved_document", ImGuiWindowFlags_UnsavedDocument, "Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.")
        .value("no_docking", ImGuiWindowFlags_NoDocking, "Disable docking of this window")
        .value("no_nav", ImGuiWindowFlags_NoNav, "")
        .value("no_decoration", ImGuiWindowFlags_NoDecoration, "")
        .value("no_inputs", ImGuiWindowFlags_NoInputs, "")
        .value("nav_flattened", ImGuiWindowFlags_NavFlattened, "[BETA] On child window: allow gamepad/keyboard navigation to cross over parent border to this child or between sibling child windows.")
        .value("child_window", ImGuiWindowFlags_ChildWindow, "Don't use! For internal use by BeginChild()")
        .value("tooltip", ImGuiWindowFlags_Tooltip, "Don't use! For internal use by BeginTooltip()")
        .value("popup", ImGuiWindowFlags_Popup, "Don't use! For internal use by BeginPopup()")
        .value("modal", ImGuiWindowFlags_Modal, "Don't use! For internal use by BeginPopupModal()")
        .value("child_menu", ImGuiWindowFlags_ChildMenu, "Don't use! For internal use by BeginMenu()")
        .value("dock_node_host", ImGuiWindowFlags_DockNodeHost, "Don't use! For internal use by Begin()/NewFrame()");


    py::enum_<ImGuiInputTextFlags_>(m, "InputTextFlags_", py::arithmetic(), " Flags for ImGui::InputText()\n (Those are per-item flags. There are shared flags in ImGuiIO: io.ConfigInputTextCursorBlink and io.ConfigInputTextEnterKeepActive)")    // imgui.h:1036
        .value("none", ImGuiInputTextFlags_None, "")
        .value("chars_decimal", ImGuiInputTextFlags_CharsDecimal, "Allow 0123456789.+-*/")
        .value("chars_hexadecimal", ImGuiInputTextFlags_CharsHexadecimal, "Allow 0123456789ABCDEFabcdef")
        .value("chars_uppercase", ImGuiInputTextFlags_CharsUppercase, "Turn a..z into A..Z")
        .value("chars_no_blank", ImGuiInputTextFlags_CharsNoBlank, "Filter out spaces, tabs")
        .value("auto_select_all", ImGuiInputTextFlags_AutoSelectAll, "Select entire text when first taking mouse focus")
        .value("enter_returns_true", ImGuiInputTextFlags_EnterReturnsTrue, "Return 'True' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.")
        .value("callback_completion", ImGuiInputTextFlags_CallbackCompletion, "Callback on pressing TAB (for completion handling)")
        .value("callback_history", ImGuiInputTextFlags_CallbackHistory, "Callback on pressing Up/Down arrows (for history handling)")
        .value("callback_always", ImGuiInputTextFlags_CallbackAlways, "Callback on each iteration. User code may query cursor position, modify text buffer.")
        .value("callback_char_filter", ImGuiInputTextFlags_CallbackCharFilter, "Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.")
        .value("allow_tab_input", ImGuiInputTextFlags_AllowTabInput, "Pressing TAB input a '\t' character into the text field")
        .value("ctrl_enter_for_new_line", ImGuiInputTextFlags_CtrlEnterForNewLine, "In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).")
        .value("no_horizontal_scroll", ImGuiInputTextFlags_NoHorizontalScroll, "Disable following the cursor horizontally")
        .value("always_overwrite", ImGuiInputTextFlags_AlwaysOverwrite, "Overwrite mode")
        .value("read_only", ImGuiInputTextFlags_ReadOnly, "Read-only mode")
        .value("password", ImGuiInputTextFlags_Password, "Password mode, display all characters as '*'")
        .value("no_undo_redo", ImGuiInputTextFlags_NoUndoRedo, "Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().")
        .value("chars_scientific", ImGuiInputTextFlags_CharsScientific, "Allow 0123456789.+-*/eE (Scientific notation input)")
        .value("callback_resize", ImGuiInputTextFlags_CallbackResize, "Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)")
        .value("callback_edit", ImGuiInputTextFlags_CallbackEdit, "Callback on any edit (note that InputText() already returns True on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)")
        .value("escape_clears_all", ImGuiInputTextFlags_EscapeClearsAll, "Escape key clears content if not empty, and deactivate otherwise (constrast to default behavior of Escape to revert)");


    py::enum_<ImGuiTreeNodeFlags_>(m, "TreeNodeFlags_", py::arithmetic(), "Flags for ImGui::TreeNodeEx(), ImGui::CollapsingHeader*()")    // imgui.h:1068
        .value("none", ImGuiTreeNodeFlags_None, "")
        .value("selected", ImGuiTreeNodeFlags_Selected, "Draw as selected")
        .value("framed", ImGuiTreeNodeFlags_Framed, "Draw frame with background (e.g. for CollapsingHeader)")
        .value("allow_item_overlap", ImGuiTreeNodeFlags_AllowItemOverlap, "Hit testing to allow subsequent widgets to overlap this one")
        .value("no_tree_push_on_open", ImGuiTreeNodeFlags_NoTreePushOnOpen, "Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack")
        .value("no_auto_open_on_log", ImGuiTreeNodeFlags_NoAutoOpenOnLog, "Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)")
        .value("default_open", ImGuiTreeNodeFlags_DefaultOpen, "Default node to be open")
        .value("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, "Need double-click to open node")
        .value("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, "Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.")
        .value("leaf", ImGuiTreeNodeFlags_Leaf, "No collapsing, no arrow (use as a convenience for leaf nodes).")
        .value("bullet", ImGuiTreeNodeFlags_Bullet, "Display a bullet instead of arrow")
        .value("frame_padding", ImGuiTreeNodeFlags_FramePadding, "Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().")
        .value("span_avail_width", ImGuiTreeNodeFlags_SpanAvailWidth, "Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.")
        .value("span_full_width", ImGuiTreeNodeFlags_SpanFullWidth, "Extend hit box to the left-most and right-most edges (bypass the indented area).")
        .value("nav_left_jumps_back_here", ImGuiTreeNodeFlags_NavLeftJumpsBackHere, "(WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)")
        .value("collapsing_header", ImGuiTreeNodeFlags_CollapsingHeader, "ImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible");


    py::enum_<ImGuiPopupFlags_>(m, "PopupFlags_", py::arithmetic(), " Flags for OpenPopup*(), BeginPopupContext*(), IsPopupOpen() functions.\n - To be backward compatible with older API which took an 'int mouse_button = 1' argument, we need to treat\n   small flags values as a mouse button index, so we encode the mouse button in the first few bits of the flags.\n   It is therefore guaranteed to be legal to pass a mouse button index in ImGuiPopupFlags.\n - For the same reason, we exceptionally default the ImGuiPopupFlags argument of BeginPopupContextXXX functions to 1 instead of 0.\n   IMPORTANT: because the default parameter is 1 (==ImGuiPopupFlags_MouseButtonRight), if you rely on the default parameter\n   and want to use another flag, you need to pass in the ImGuiPopupFlags_MouseButtonRight flag explicitly.\n - Multiple buttons currently cannot be combined/or-ed in those functions (we could allow it later).")    // imgui.h:1097
        .value("none", ImGuiPopupFlags_None, "")
        .value("mouse_button_left", ImGuiPopupFlags_MouseButtonLeft, "For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as ImGuiMouseButton_Left)")
        .value("mouse_button_right", ImGuiPopupFlags_MouseButtonRight, "For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as ImGuiMouseButton_Right)")
        .value("mouse_button_middle", ImGuiPopupFlags_MouseButtonMiddle, "For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as ImGuiMouseButton_Middle)")
        .value("mouse_button_mask_", ImGuiPopupFlags_MouseButtonMask_, "")
        .value("mouse_button_default_", ImGuiPopupFlags_MouseButtonDefault_, "")
        .value("no_open_over_existing_popup", ImGuiPopupFlags_NoOpenOverExistingPopup, "For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack")
        .value("no_open_over_items", ImGuiPopupFlags_NoOpenOverItems, "For BeginPopupContextWindow(): don't return True when hovering items, only when hovering empty space")
        .value("any_popup_id", ImGuiPopupFlags_AnyPopupId, "For IsPopupOpen(): ignore the ImGuiID parameter and test for any popup.")
        .value("any_popup_level", ImGuiPopupFlags_AnyPopupLevel, "For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)")
        .value("any_popup", ImGuiPopupFlags_AnyPopup, "");


    py::enum_<ImGuiSelectableFlags_>(m, "SelectableFlags_", py::arithmetic(), "Flags for ImGui::Selectable()")    // imgui.h:1113
        .value("none", ImGuiSelectableFlags_None, "")
        .value("dont_close_popups", ImGuiSelectableFlags_DontClosePopups, "Clicking this doesn't close parent popup window")
        .value("span_all_columns", ImGuiSelectableFlags_SpanAllColumns, "Selectable frame can span all columns (text will still fit in current column)")
        .value("allow_double_click", ImGuiSelectableFlags_AllowDoubleClick, "Generate press events on double clicks too")
        .value("disabled", ImGuiSelectableFlags_Disabled, "Cannot be selected, display grayed out text")
        .value("allow_item_overlap", ImGuiSelectableFlags_AllowItemOverlap, "(WIP) Hit testing to allow subsequent widgets to overlap this one");


    py::enum_<ImGuiComboFlags_>(m, "ComboFlags_", py::arithmetic(), "Flags for ImGui::BeginCombo()")    // imgui.h:1124
        .value("none", ImGuiComboFlags_None, "")
        .value("popup_align_left", ImGuiComboFlags_PopupAlignLeft, "Align the popup toward the left by default")
        .value("height_small", ImGuiComboFlags_HeightSmall, "Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()")
        .value("height_regular", ImGuiComboFlags_HeightRegular, "Max ~8 items visible (default)")
        .value("height_large", ImGuiComboFlags_HeightLarge, "Max ~20 items visible")
        .value("height_largest", ImGuiComboFlags_HeightLargest, "As many fitting items as possible")
        .value("no_arrow_button", ImGuiComboFlags_NoArrowButton, "Display on the preview box without the square arrow button")
        .value("no_preview", ImGuiComboFlags_NoPreview, "Display only a square arrow button")
        .value("height_mask_", ImGuiComboFlags_HeightMask_, "");


    py::enum_<ImGuiTabBarFlags_>(m, "TabBarFlags_", py::arithmetic(), "Flags for ImGui::BeginTabBar()")    // imgui.h:1138
        .value("none", ImGuiTabBarFlags_None, "")
        .value("reorderable", ImGuiTabBarFlags_Reorderable, "Allow manually dragging tabs to re-order them + New tabs are appended at the end of list")
        .value("auto_select_new_tabs", ImGuiTabBarFlags_AutoSelectNewTabs, "Automatically select new tabs when they appear")
        .value("tab_list_popup_button", ImGuiTabBarFlags_TabListPopupButton, "Disable buttons to open the tab list popup")
        .value("no_close_with_middle_mouse_button", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton, "Disable behavior of closing tabs (that are submitted with p_open != None) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = False.")
        .value("no_tab_list_scrolling_buttons", ImGuiTabBarFlags_NoTabListScrollingButtons, "Disable scrolling buttons (apply when fitting policy is ImGuiTabBarFlags_FittingPolicyScroll)")
        .value("no_tooltip", ImGuiTabBarFlags_NoTooltip, "Disable tooltips when hovering a tab")
        .value("fitting_policy_resize_down", ImGuiTabBarFlags_FittingPolicyResizeDown, "Resize tabs when they don't fit")
        .value("fitting_policy_scroll", ImGuiTabBarFlags_FittingPolicyScroll, "Add scroll buttons when tabs don't fit")
        .value("fitting_policy_mask_", ImGuiTabBarFlags_FittingPolicyMask_, "")
        .value("fitting_policy_default_", ImGuiTabBarFlags_FittingPolicyDefault_, "");


    py::enum_<ImGuiTabItemFlags_>(m, "TabItemFlags_", py::arithmetic(), "Flags for ImGui::BeginTabItem()")    // imgui.h:1154
        .value("none", ImGuiTabItemFlags_None, "")
        .value("unsaved_document", ImGuiTabItemFlags_UnsavedDocument, "Display a dot next to the title + tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.")
        .value("set_selected", ImGuiTabItemFlags_SetSelected, "Trigger flag to programmatically make the tab selected when calling BeginTabItem()")
        .value("no_close_with_middle_mouse_button", ImGuiTabItemFlags_NoCloseWithMiddleMouseButton, "Disable behavior of closing tabs (that are submitted with p_open != None) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = False.")
        .value("no_push_id", ImGuiTabItemFlags_NoPushId, "Don't call PushID(tab->ID)/PopID() on BeginTabItem()/EndTabItem()")
        .value("no_tooltip", ImGuiTabItemFlags_NoTooltip, "Disable tooltip for the given tab")
        .value("no_reorder", ImGuiTabItemFlags_NoReorder, "Disable reordering this tab or having another tab cross over this tab")
        .value("leading", ImGuiTabItemFlags_Leading, "Enforce the tab position to the left of the tab bar (after the tab list popup button)")
        .value("trailing", ImGuiTabItemFlags_Trailing, "Enforce the tab position to the right of the tab bar (before the scrolling buttons)");


    py::enum_<ImGuiTableFlags_>(m, "TableFlags_", py::arithmetic(), " Flags for ImGui::BeginTable()\n - Important! Sizing policies have complex and subtle side effects, much more so than you would expect.\n   Read comments/demos carefully + experiment with live demos to get acquainted with them.\n - The DEFAULT sizing policies are:\n    - Default to ImGuiTableFlags_SizingFixedFit    if ScrollX is on, or if host window has ImGuiWindowFlags_AlwaysAutoResize.\n    - Default to ImGuiTableFlags_SizingStretchSame if ScrollX is off.\n - When ScrollX is off:\n    - Table defaults to ImGuiTableFlags_SizingStretchSame -> all Columns defaults to ImGuiTableColumnFlags_WidthStretch with same weight.\n    - Columns sizing policy allowed: Stretch (default), Fixed/Auto.\n    - Fixed Columns (if any) will generally obtain their requested width (unless the table cannot fit them all).\n    - Stretch Columns will share the remaining width according to their respective weight.\n    - Mixed Fixed/Stretch columns is possible but has various side-effects on resizing behaviors.\n      The typical use of mixing sizing policies is: any number of LEADING Fixed columns, followed by one or two TRAILING Stretch columns.\n      (this is because the visible order of columns have subtle but necessary effects on how they react to manual resizing).\n - When ScrollX is on:\n    - Table defaults to ImGuiTableFlags_SizingFixedFit -> all Columns defaults to ImGuiTableColumnFlags_WidthFixed\n    - Columns sizing policy allowed: Fixed/Auto mostly.\n    - Fixed Columns can be enlarged as needed. Table will show a horizontal scrollbar if needed.\n    - When using auto-resizing (non-resizable) fixed columns, querying the content width to use item right-alignment e.g. SetNextItemWidth(-FLT_MIN) doesn't make sense, would create a feedback loop.\n    - Using Stretch columns OFTEN DOES NOT MAKE SENSE if ScrollX is on, UNLESS you have specified a value for 'inner_width' in BeginTable().\n      If you specify a value for 'inner_width' then effectively the scrolling space is known and Stretch or mixed Fixed/Stretch columns become meaningful again.\n - Read on documentation at the top of imgui_tables.cpp for details.")    // imgui.h:1189
        .value("none", ImGuiTableFlags_None, "")
        .value("resizable", ImGuiTableFlags_Resizable, "Enable resizing columns.")
        .value("reorderable", ImGuiTableFlags_Reorderable, "Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)")
        .value("hideable", ImGuiTableFlags_Hideable, "Enable hiding/disabling columns in context menu.")
        .value("sortable", ImGuiTableFlags_Sortable, "Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see ImGuiTableFlags_SortMulti and ImGuiTableFlags_SortTristate.")
        .value("no_saved_settings", ImGuiTableFlags_NoSavedSettings, "Disable persisting columns order, width and sort settings in the .ini file.")
        .value("context_menu_in_body", ImGuiTableFlags_ContextMenuInBody, "Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().")
        .value("row_bg", ImGuiTableFlags_RowBg, "Set each RowBg color with ImGuiCol_TableRowBg or ImGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with ImGuiTableBgFlags_RowBg0 on each row manually)")
        .value("borders_inner_h", ImGuiTableFlags_BordersInnerH, "Draw horizontal borders between rows.")
        .value("borders_outer_h", ImGuiTableFlags_BordersOuterH, "Draw horizontal borders at the top and bottom.")
        .value("borders_inner_v", ImGuiTableFlags_BordersInnerV, "Draw vertical borders between columns.")
        .value("borders_outer_v", ImGuiTableFlags_BordersOuterV, "Draw vertical borders on the left and right sides.")
        .value("borders_h", ImGuiTableFlags_BordersH, "Draw horizontal borders.")
        .value("borders_v", ImGuiTableFlags_BordersV, "Draw vertical borders.")
        .value("borders_inner", ImGuiTableFlags_BordersInner, "Draw inner borders.")
        .value("borders_outer", ImGuiTableFlags_BordersOuter, "Draw outer borders.")
        .value("borders", ImGuiTableFlags_Borders, "Draw all borders.")
        .value("no_borders_in_body", ImGuiTableFlags_NoBordersInBody, "[ALPHA] Disable vertical borders in columns Body (borders will always appear in Headers). -> May move to style")
        .value("no_borders_in_body_until_resize", ImGuiTableFlags_NoBordersInBodyUntilResize, "[ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers). -> May move to style")
        .value("sizing_fixed_fit", ImGuiTableFlags_SizingFixedFit, "Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.")
        .value("sizing_fixed_same", ImGuiTableFlags_SizingFixedSame, "Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Implicitly enable ImGuiTableFlags_NoKeepColumnsVisible.")
        .value("sizing_stretch_prop", ImGuiTableFlags_SizingStretchProp, "Columns default to _WidthStretch with default weights proportional to each columns contents widths.")
        .value("sizing_stretch_same", ImGuiTableFlags_SizingStretchSame, "Columns default to _WidthStretch with default weights all equal, unless overridden by TableSetupColumn().")
        .value("no_host_extend_x", ImGuiTableFlags_NoHostExtendX, "Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.")
        .value("no_host_extend_y", ImGuiTableFlags_NoHostExtendY, "Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.")
        .value("no_keep_columns_visible", ImGuiTableFlags_NoKeepColumnsVisible, "Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.")
        .value("precise_widths", ImGuiTableFlags_PreciseWidths, "Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.")
        .value("no_clip", ImGuiTableFlags_NoClip, "Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with TableSetupScrollFreeze().")
        .value("pad_outer_x", ImGuiTableFlags_PadOuterX, "Default if BordersOuterV is on. Enable outermost padding. Generally desirable if you have headers.")
        .value("no_pad_outer_x", ImGuiTableFlags_NoPadOuterX, "Default if BordersOuterV is off. Disable outermost padding.")
        .value("no_pad_inner_x", ImGuiTableFlags_NoPadInnerX, "Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).")
        .value("scroll_x", ImGuiTableFlags_ScrollX, "Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this creates a child window, ScrollY is currently generally recommended when using ScrollX.")
        .value("scroll_y", ImGuiTableFlags_ScrollY, "Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.")
        .value("sort_multi", ImGuiTableFlags_SortMulti, "Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).")
        .value("sort_tristate", ImGuiTableFlags_SortTristate, "Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).")
        .value("sizing_mask_", ImGuiTableFlags_SizingMask_, "[Internal] Combinations and masks");


    py::enum_<ImGuiTableColumnFlags_>(m, "TableColumnFlags_", py::arithmetic(), "Flags for ImGui::TableSetupColumn()")    // imgui.h:1240
        .value("none", ImGuiTableColumnFlags_None, "")
        .value("disabled", ImGuiTableColumnFlags_Disabled, "Overriding/master disable flag: hide column, won't show in context menu (unlike calling TableSetColumnEnabled() which manipulates the user accessible state)")
        .value("default_hide", ImGuiTableColumnFlags_DefaultHide, "Default as a hidden/disabled column.")
        .value("default_sort", ImGuiTableColumnFlags_DefaultSort, "Default as a sorting column.")
        .value("width_stretch", ImGuiTableColumnFlags_WidthStretch, "Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).")
        .value("width_fixed", ImGuiTableColumnFlags_WidthFixed, "Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).")
        .value("no_resize", ImGuiTableColumnFlags_NoResize, "Disable manual resizing.")
        .value("no_reorder", ImGuiTableColumnFlags_NoReorder, "Disable manual reordering this column, this will also prevent other columns from crossing over this column.")
        .value("no_hide", ImGuiTableColumnFlags_NoHide, "Disable ability to hide/disable this column.")
        .value("no_clip", ImGuiTableColumnFlags_NoClip, "Disable clipping for this column (all NoClip columns will render in a same draw command).")
        .value("no_sort", ImGuiTableColumnFlags_NoSort, "Disable ability to sort on this field (even if ImGuiTableFlags_Sortable is set on the table).")
        .value("no_sort_ascending", ImGuiTableColumnFlags_NoSortAscending, "Disable ability to sort in the ascending direction.")
        .value("no_sort_descending", ImGuiTableColumnFlags_NoSortDescending, "Disable ability to sort in the descending direction.")
        .value("no_header_label", ImGuiTableColumnFlags_NoHeaderLabel, "TableHeadersRow() will not submit label for this column. Convenient for some small columns. Name will still appear in context menu.")
        .value("no_header_width", ImGuiTableColumnFlags_NoHeaderWidth, "Disable header text width contribution to automatic column width.")
        .value("prefer_sort_ascending", ImGuiTableColumnFlags_PreferSortAscending, "Make the initial sort direction Ascending when first sorting on this column (default).")
        .value("prefer_sort_descending", ImGuiTableColumnFlags_PreferSortDescending, "Make the initial sort direction Descending when first sorting on this column.")
        .value("indent_enable", ImGuiTableColumnFlags_IndentEnable, "Use current Indent value when entering cell (default for column 0).")
        .value("indent_disable", ImGuiTableColumnFlags_IndentDisable, "Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.")
        .value("is_enabled", ImGuiTableColumnFlags_IsEnabled, "Status: is enabled == not hidden by user/api (referred to as \"Hide\" in _DefaultHide and _NoHide) flags.")
        .value("is_visible", ImGuiTableColumnFlags_IsVisible, "Status: is visible == is enabled AND not clipped by scrolling.")
        .value("is_sorted", ImGuiTableColumnFlags_IsSorted, "Status: is currently part of the sort specs")
        .value("is_hovered", ImGuiTableColumnFlags_IsHovered, "Status: is hovered by mouse")
        .value("width_mask_", ImGuiTableColumnFlags_WidthMask_, "")
        .value("indent_mask_", ImGuiTableColumnFlags_IndentMask_, "")
        .value("status_mask_", ImGuiTableColumnFlags_StatusMask_, "")
        .value("no_direct_resize_", ImGuiTableColumnFlags_NoDirectResize_, "[Internal] Disable user resizing this column directly (it may however we resized indirectly from its left edge)");


    py::enum_<ImGuiTableRowFlags_>(m, "TableRowFlags_", py::arithmetic(), "Flags for ImGui::TableNextRow()")    // imgui.h:1277
        .value("none", ImGuiTableRowFlags_None, "")
        .value("headers", ImGuiTableRowFlags_Headers, "Identify header row (set default background color + width of its contents accounted differently for auto column width)");


    py::enum_<ImGuiTableBgTarget_>(m, "TableBgTarget_", py::arithmetic(), " Enum for ImGui::TableSetBgColor()\n Background colors are rendering in 3 layers:\n  - Layer 0: draw with RowBg0 color if set, otherwise draw with ColumnBg0 if set.\n  - Layer 1: draw with RowBg1 color if set, otherwise draw with ColumnBg1 if set.\n  - Layer 2: draw with CellBg color if set.\n The purpose of the two row/columns layers is to let you decide if a background color change should override or blend with the existing color.\n When using ImGuiTableFlags_RowBg on the table, each row has the RowBg0 color automatically set for odd/even rows.\n If you set the color of RowBg0 target, your color will override the existing RowBg0 color.\n If you set the color of RowBg1 or ColumnBg1 target, your color will blend over the RowBg0 color.")    // imgui.h:1292
        .value("none", ImGuiTableBgTarget_None, "")
        .value("row_bg0", ImGuiTableBgTarget_RowBg0, "Set row background color 0 (generally used for background, automatically set when ImGuiTableFlags_RowBg is used)")
        .value("row_bg1", ImGuiTableBgTarget_RowBg1, "Set row background color 1 (generally used for selection marking)")
        .value("cell_bg", ImGuiTableBgTarget_CellBg, "Set cell background color (top-most color)");


    py::enum_<ImGuiFocusedFlags_>(m, "FocusedFlags_", py::arithmetic(), "Flags for ImGui::IsWindowFocused()")    // imgui.h:1301
        .value("none", ImGuiFocusedFlags_None, "")
        .value("child_windows", ImGuiFocusedFlags_ChildWindows, "Return True if any children of the window is focused")
        .value("root_window", ImGuiFocusedFlags_RootWindow, "Test from root window (top most parent of the current hierarchy)")
        .value("any_window", ImGuiFocusedFlags_AnyWindow, "Return True if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!")
        .value("no_popup_hierarchy", ImGuiFocusedFlags_NoPopupHierarchy, "Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)")
        .value("dock_hierarchy", ImGuiFocusedFlags_DockHierarchy, "Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)")
        .value("root_and_child_windows", ImGuiFocusedFlags_RootAndChildWindows, "");


    py::enum_<ImGuiHoveredFlags_>(m, "HoveredFlags_", py::arithmetic(), " Flags for ImGui::IsItemHovered(), ImGui::IsWindowHovered()\n Note: if you are trying to check whether your mouse should be dispatched to Dear ImGui or to your app, you should use 'io.WantCaptureMouse' instead! Please read the FAQ!\n Note: windows with the ImGuiWindowFlags_NoInputs flag are ignored by IsWindowHovered() calls.")    // imgui.h:1315
        .value("none", ImGuiHoveredFlags_None, "Return True if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.")
        .value("child_windows", ImGuiHoveredFlags_ChildWindows, "IsWindowHovered() only: Return True if any children of the window is hovered")
        .value("root_window", ImGuiHoveredFlags_RootWindow, "IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)")
        .value("any_window", ImGuiHoveredFlags_AnyWindow, "IsWindowHovered() only: Return True if any window is hovered")
        .value("no_popup_hierarchy", ImGuiHoveredFlags_NoPopupHierarchy, "IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)")
        .value("dock_hierarchy", ImGuiHoveredFlags_DockHierarchy, "IsWindowHovered() only: Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)")
        .value("allow_when_blocked_by_popup", ImGuiHoveredFlags_AllowWhenBlockedByPopup, "Return True even if a popup window is normally blocking access to this item/window")
        .value("allow_when_blocked_by_active_item", ImGuiHoveredFlags_AllowWhenBlockedByActiveItem, "Return True even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.")
        .value("allow_when_overlapped", ImGuiHoveredFlags_AllowWhenOverlapped, "IsItemHovered() only: Return True even if the position is obstructed or overlapped by another window")
        .value("allow_when_disabled", ImGuiHoveredFlags_AllowWhenDisabled, "IsItemHovered() only: Return True even if the item is disabled")
        .value("no_nav_override", ImGuiHoveredFlags_NoNavOverride, "Disable using gamepad/keyboard navigation state when active, always query mouse.")
        .value("rect_only", ImGuiHoveredFlags_RectOnly, "")
        .value("root_and_child_windows", ImGuiHoveredFlags_RootAndChildWindows, "")
        .value("delay_normal", ImGuiHoveredFlags_DelayNormal, "Return True after io.HoverDelayNormal elapsed (~0.30 sec)")
        .value("delay_short", ImGuiHoveredFlags_DelayShort, "Return True after io.HoverDelayShort elapsed (~0.10 sec)")
        .value("no_shared_delay", ImGuiHoveredFlags_NoSharedDelay, "Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)");


    py::enum_<ImGuiDockNodeFlags_>(m, "DockNodeFlags_", py::arithmetic(), " Flags for ImGui::DockSpace(), shared/inherited by child nodes.\n (Some flags can be applied to individual nodes directly)\n FIXME-DOCK: Also see ImGuiDockNodeFlagsPrivate_ which may involve using the WIP and internal DockBuilder api.")    // imgui.h:1341
        .value("none", ImGuiDockNodeFlags_None, "")
        .value("keep_alive_only", ImGuiDockNodeFlags_KeepAliveOnly, "Shared       // Don't display the dockspace node but keep it alive. Windows docked into this dockspace node won't be undocked.")
        .value("no_docking_in_central_node", ImGuiDockNodeFlags_NoDockingInCentralNode, "Shared       // Disable docking inside the Central Node, which will be always kept empty.")
        .value("passthru_central_node", ImGuiDockNodeFlags_PassthruCentralNode, "Shared       // Enable passthru dockspace: 1) DockSpace() will render a ImGuiCol_WindowBg background covering everything excepted the Central Node when empty. Meaning the host window should probably use SetNextWindowBgAlpha(0.0) prior to Begin() when using this. 2) When Central Node is empty: let inputs pass-through + won't display a DockingEmptyBg background. See demo for details.")
        .value("no_split", ImGuiDockNodeFlags_NoSplit, "Shared/Local // Disable splitting the node into smaller nodes. Useful e.g. when embedding dockspaces into a main root one (the root one may have splitting disabled to reduce confusion). Note: when turned off, existing splits will be preserved.")
        .value("no_resize", ImGuiDockNodeFlags_NoResize, "Shared/Local // Disable resizing node using the splitter/separators. Useful with programmatically setup dockspaces.")
        .value("auto_hide_tab_bar", ImGuiDockNodeFlags_AutoHideTabBar, "Shared/Local // Tab bar will automatically hide when there is a single window in the dock node.");


    py::enum_<ImGuiDragDropFlags_>(m, "DragDropFlags_", py::arithmetic(), "Flags for ImGui::BeginDragDropSource(), ImGui::AcceptDragDropPayload()")    // imgui.h:1354
        .value("none", ImGuiDragDropFlags_None, "")
        .value("source_no_preview_tooltip", ImGuiDragDropFlags_SourceNoPreviewTooltip, "By default, a successful call to BeginDragDropSource opens a tooltip so you can display a preview or description of the source contents. This flag disables this behavior.")
        .value("source_no_disable_hover", ImGuiDragDropFlags_SourceNoDisableHover, "By default, when dragging we clear data so that IsItemHovered() will return False, to avoid subsequent user code submitting tooltips. This flag disables this behavior so you can still call IsItemHovered() on the source item.")
        .value("source_no_hold_to_open_others", ImGuiDragDropFlags_SourceNoHoldToOpenOthers, "Disable the behavior that allows to open tree nodes and collapsing header by holding over them while dragging a source item.")
        .value("source_allow_null_id", ImGuiDragDropFlags_SourceAllowNullID, "Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit.")
        .value("source_extern", ImGuiDragDropFlags_SourceExtern, "External source (from outside of dear imgui), won't attempt to read current item/window info. Will always return True. Only one Extern source can be active simultaneously.")
        .value("source_auto_expire_payload", ImGuiDragDropFlags_SourceAutoExpirePayload, "Automatically expire the payload if the source cease to be submitted (otherwise payloads are persisting while being dragged)")
        .value("accept_before_delivery", ImGuiDragDropFlags_AcceptBeforeDelivery, "AcceptDragDropPayload() will returns True even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.")
        .value("accept_no_draw_default_rect", ImGuiDragDropFlags_AcceptNoDrawDefaultRect, "Do not draw the default highlight rectangle when hovering over target.")
        .value("accept_no_preview_tooltip", ImGuiDragDropFlags_AcceptNoPreviewTooltip, "Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.")
        .value("accept_peek_only", ImGuiDragDropFlags_AcceptPeekOnly, "For peeking ahead and inspecting the payload before delivery.");


    py::enum_<ImGuiDataType_>(m, "DataType_", py::arithmetic(), "A primary data type")    // imgui.h:1376
        .value("s8", ImGuiDataType_S8, "signed char / char (with sensible compilers)")
        .value("u8", ImGuiDataType_U8, "unsigned char")
        .value("s16", ImGuiDataType_S16, "short")
        .value("u16", ImGuiDataType_U16, "unsigned short")
        .value("s32", ImGuiDataType_S32, "int")
        .value("u32", ImGuiDataType_U32, "unsigned int")
        .value("s64", ImGuiDataType_S64, "long long / __int64")
        .value("u64", ImGuiDataType_U64, "unsigned long long / unsigned __int64")
        .value("float", ImGuiDataType_Float, "float")
        .value("double", ImGuiDataType_Double, "double")
        .value("count", ImGuiDataType_COUNT, "");


    py::enum_<ImGuiDir_>(m, "Dir_", py::arithmetic(), "A cardinal direction")    // imgui.h:1392
        .value("none", ImGuiDir_None, "")
        .value("left", ImGuiDir_Left, "")
        .value("right", ImGuiDir_Right, "")
        .value("up", ImGuiDir_Up, "")
        .value("down", ImGuiDir_Down, "")
        .value("count", ImGuiDir_COUNT, "");


    py::enum_<ImGuiSortDirection_>(m, "SortDirection_", py::arithmetic(), "A sorting direction")    // imgui.h:1403
        .value("none", ImGuiSortDirection_None, "")
        .value("ascending", ImGuiSortDirection_Ascending, "Ascending = 0->9, A->Z etc.")
        .value("descending", ImGuiSortDirection_Descending, "Descending = 9->0, Z->A etc.");


    py::enum_<ImGuiKey>(m, "Key", py::arithmetic(), " A key identifier (ImGuiKey_XXX or ImGuiMod_XXX value)\n All our named keys are >= 512. Keys value 0 to 511 are left unused as legacy native/opaque key values (< 1.87)\n Since >= 1.89 we increased typing (went from int to enum), some legacy code may need a cast to ImGuiKey.")    // imgui.h:1413
        .value("none", ImGuiKey_None, "")
        .value("tab", ImGuiKey_Tab, "== ImGuiKey_NamedKey_BEGIN")
        .value("left_arrow", ImGuiKey_LeftArrow, "")
        .value("right_arrow", ImGuiKey_RightArrow, "")
        .value("up_arrow", ImGuiKey_UpArrow, "")
        .value("down_arrow", ImGuiKey_DownArrow, "")
        .value("page_up", ImGuiKey_PageUp, "")
        .value("page_down", ImGuiKey_PageDown, "")
        .value("home", ImGuiKey_Home, "")
        .value("end", ImGuiKey_End, "")
        .value("insert", ImGuiKey_Insert, "")
        .value("delete", ImGuiKey_Delete, "")
        .value("backspace", ImGuiKey_Backspace, "")
        .value("space", ImGuiKey_Space, "")
        .value("enter", ImGuiKey_Enter, "")
        .value("escape", ImGuiKey_Escape, "")
        .value("left_ctrl", ImGuiKey_LeftCtrl, "")
        .value("left_shift", ImGuiKey_LeftShift, "")
        .value("left_alt", ImGuiKey_LeftAlt, "")
        .value("left_super", ImGuiKey_LeftSuper, "")
        .value("right_ctrl", ImGuiKey_RightCtrl, "")
        .value("right_shift", ImGuiKey_RightShift, "")
        .value("right_alt", ImGuiKey_RightAlt, "")
        .value("right_super", ImGuiKey_RightSuper, "")
        .value("menu", ImGuiKey_Menu, "")
        .value("_0", ImGuiKey_0, "")
        .value("_1", ImGuiKey_1, "")
        .value("_2", ImGuiKey_2, "")
        .value("_3", ImGuiKey_3, "")
        .value("_4", ImGuiKey_4, "")
        .value("_5", ImGuiKey_5, "")
        .value("_6", ImGuiKey_6, "")
        .value("_7", ImGuiKey_7, "")
        .value("_8", ImGuiKey_8, "")
        .value("_9", ImGuiKey_9, "")
        .value("a", ImGuiKey_A, "")
        .value("b", ImGuiKey_B, "")
        .value("c", ImGuiKey_C, "")
        .value("d", ImGuiKey_D, "")
        .value("e", ImGuiKey_E, "")
        .value("f", ImGuiKey_F, "")
        .value("g", ImGuiKey_G, "")
        .value("h", ImGuiKey_H, "")
        .value("i", ImGuiKey_I, "")
        .value("j", ImGuiKey_J, "")
        .value("k", ImGuiKey_K, "")
        .value("l", ImGuiKey_L, "")
        .value("m", ImGuiKey_M, "")
        .value("n", ImGuiKey_N, "")
        .value("o", ImGuiKey_O, "")
        .value("p", ImGuiKey_P, "")
        .value("q", ImGuiKey_Q, "")
        .value("r", ImGuiKey_R, "")
        .value("s", ImGuiKey_S, "")
        .value("t", ImGuiKey_T, "")
        .value("u", ImGuiKey_U, "")
        .value("v", ImGuiKey_V, "")
        .value("w", ImGuiKey_W, "")
        .value("x", ImGuiKey_X, "")
        .value("y", ImGuiKey_Y, "")
        .value("z", ImGuiKey_Z, "")
        .value("f1", ImGuiKey_F1, "")
        .value("f2", ImGuiKey_F2, "")
        .value("f3", ImGuiKey_F3, "")
        .value("f4", ImGuiKey_F4, "")
        .value("f5", ImGuiKey_F5, "")
        .value("f6", ImGuiKey_F6, "")
        .value("f7", ImGuiKey_F7, "")
        .value("f8", ImGuiKey_F8, "")
        .value("f9", ImGuiKey_F9, "")
        .value("f10", ImGuiKey_F10, "")
        .value("f11", ImGuiKey_F11, "")
        .value("f12", ImGuiKey_F12, "")
        .value("apostrophe", ImGuiKey_Apostrophe, "'")
        .value("comma", ImGuiKey_Comma, ",")
        .value("minus", ImGuiKey_Minus, "-")
        .value("period", ImGuiKey_Period, ".")
        .value("slash", ImGuiKey_Slash, "/")
        .value("semicolon", ImGuiKey_Semicolon, ";")
        .value("equal", ImGuiKey_Equal, "=")
        .value("left_bracket", ImGuiKey_LeftBracket, "[")
        .value("backslash", ImGuiKey_Backslash, "\\ (this text inhibit multiline comment caused by backslash)")
        .value("right_bracket", ImGuiKey_RightBracket, "]")
        .value("grave_accent", ImGuiKey_GraveAccent, "`")
        .value("caps_lock", ImGuiKey_CapsLock, "")
        .value("scroll_lock", ImGuiKey_ScrollLock, "")
        .value("num_lock", ImGuiKey_NumLock, "")
        .value("print_screen", ImGuiKey_PrintScreen, "")
        .value("pause", ImGuiKey_Pause, "")
        .value("keypad0", ImGuiKey_Keypad0, "")
        .value("keypad1", ImGuiKey_Keypad1, "")
        .value("keypad2", ImGuiKey_Keypad2, "")
        .value("keypad3", ImGuiKey_Keypad3, "")
        .value("keypad4", ImGuiKey_Keypad4, "")
        .value("keypad5", ImGuiKey_Keypad5, "")
        .value("keypad6", ImGuiKey_Keypad6, "")
        .value("keypad7", ImGuiKey_Keypad7, "")
        .value("keypad8", ImGuiKey_Keypad8, "")
        .value("keypad9", ImGuiKey_Keypad9, "")
        .value("keypad_decimal", ImGuiKey_KeypadDecimal, "")
        .value("keypad_divide", ImGuiKey_KeypadDivide, "")
        .value("keypad_multiply", ImGuiKey_KeypadMultiply, "")
        .value("keypad_subtract", ImGuiKey_KeypadSubtract, "")
        .value("keypad_add", ImGuiKey_KeypadAdd, "")
        .value("keypad_enter", ImGuiKey_KeypadEnter, "")
        .value("keypad_equal", ImGuiKey_KeypadEqual, "")
        .value("gamepad_start", ImGuiKey_GamepadStart, "Menu (Xbox)      + (Switch)   Start/Options (PS)")
        .value("gamepad_back", ImGuiKey_GamepadBack, "View (Xbox)      - (Switch)   Share (PS)")
        .value("gamepad_face_left", ImGuiKey_GamepadFaceLeft, "X (Xbox)         Y (Switch)   Square (PS)        // Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)")
        .value("gamepad_face_right", ImGuiKey_GamepadFaceRight, "B (Xbox)         A (Switch)   Circle (PS)        // Cancel / Close / Exit")
        .value("gamepad_face_up", ImGuiKey_GamepadFaceUp, "Y (Xbox)         X (Switch)   Triangle (PS)      // Text Input / On-screen Keyboard")
        .value("gamepad_face_down", ImGuiKey_GamepadFaceDown, "A (Xbox)         B (Switch)   Cross (PS)         // Activate / Open / Toggle / Tweak")
        .value("gamepad_dpad_left", ImGuiKey_GamepadDpadLeft, "D-pad Left                                       // Move / Tweak / Resize Window (in Windowing mode)")
        .value("gamepad_dpad_right", ImGuiKey_GamepadDpadRight, "D-pad Right                                      // Move / Tweak / Resize Window (in Windowing mode)")
        .value("gamepad_dpad_up", ImGuiKey_GamepadDpadUp, "D-pad Up                                         // Move / Tweak / Resize Window (in Windowing mode)")
        .value("gamepad_dpad_down", ImGuiKey_GamepadDpadDown, "D-pad Down                                       // Move / Tweak / Resize Window (in Windowing mode)")
        .value("gamepad_l1", ImGuiKey_GamepadL1, "L Bumper (Xbox)  L (Switch)   L1 (PS)            // Tweak Slower / Focus Previous (in Windowing mode)")
        .value("gamepad_r1", ImGuiKey_GamepadR1, "R Bumper (Xbox)  R (Switch)   R1 (PS)            // Tweak Faster / Focus Next (in Windowing mode)")
        .value("gamepad_l2", ImGuiKey_GamepadL2, "L Trig. (Xbox)   ZL (Switch)  L2 (PS) [Analog]")
        .value("gamepad_r2", ImGuiKey_GamepadR2, "R Trig. (Xbox)   ZR (Switch)  R2 (PS) [Analog]")
        .value("gamepad_l3", ImGuiKey_GamepadL3, "L Stick (Xbox)   L3 (Switch)  L3 (PS)")
        .value("gamepad_r3", ImGuiKey_GamepadR3, "R Stick (Xbox)   R3 (Switch)  R3 (PS)")
        .value("gamepad_l_stick_left", ImGuiKey_GamepadLStickLeft, "[Analog]                                         // Move Window (in Windowing mode)")
        .value("gamepad_l_stick_right", ImGuiKey_GamepadLStickRight, "[Analog]                                         // Move Window (in Windowing mode)")
        .value("gamepad_l_stick_up", ImGuiKey_GamepadLStickUp, "[Analog]                                         // Move Window (in Windowing mode)")
        .value("gamepad_l_stick_down", ImGuiKey_GamepadLStickDown, "[Analog]                                         // Move Window (in Windowing mode)")
        .value("gamepad_r_stick_left", ImGuiKey_GamepadRStickLeft, "[Analog]")
        .value("gamepad_r_stick_right", ImGuiKey_GamepadRStickRight, "[Analog]")
        .value("gamepad_r_stick_up", ImGuiKey_GamepadRStickUp, "[Analog]")
        .value("gamepad_r_stick_down", ImGuiKey_GamepadRStickDown, "[Analog]")
        .value("mouse_left", ImGuiKey_MouseLeft, " Mouse Buttons (auto-submitted from AddMouseButtonEvent() calls)\n - This is mirroring the data also written to io.MouseDown[], io.MouseWheel, in a format allowing them to be accessed via standard key API.")
        .value("mouse_right", ImGuiKey_MouseRight, "")
        .value("mouse_middle", ImGuiKey_MouseMiddle, "")
        .value("mouse_x1", ImGuiKey_MouseX1, "")
        .value("mouse_x2", ImGuiKey_MouseX2, "")
        .value("mouse_wheel_x", ImGuiKey_MouseWheelX, "")
        .value("mouse_wheel_y", ImGuiKey_MouseWheelY, "")
        .value("reserved_for_mod_ctrl", ImGuiKey_ReservedForModCtrl, "[Internal] Reserved for mod storage")
        .value("reserved_for_mod_shift", ImGuiKey_ReservedForModShift, "")
        .value("reserved_for_mod_alt", ImGuiKey_ReservedForModAlt, "")
        .value("reserved_for_mod_super", ImGuiKey_ReservedForModSuper, "")
        .value("count", ImGuiKey_COUNT, "")
        .value("im_gui_mod_none", ImGuiMod_None, "")
        .value("im_gui_mod_ctrl", ImGuiMod_Ctrl, "")
        .value("im_gui_mod_shift", ImGuiMod_Shift, "")
        .value("im_gui_mod_alt", ImGuiMod_Alt, "Option/Menu")
        .value("im_gui_mod_super", ImGuiMod_Super, "Cmd/Super/Windows")
        .value("im_gui_mod_mask_", ImGuiMod_Mask_, "")
        .value("named_key_begin", ImGuiKey_NamedKey_BEGIN, "")
        .value("named_key_end", ImGuiKey_NamedKey_END, "")
        .value("named_key_count", ImGuiKey_NamedKey_COUNT, "");


    py::enum_<ImGuiConfigFlags_>(m, "ConfigFlags_", py::arithmetic(), "Configuration flags stored in io.ConfigFlags. Set by user/application.")    // imgui.h:1554
        .value("none", ImGuiConfigFlags_None, "")
        .value("nav_enable_keyboard", ImGuiConfigFlags_NavEnableKeyboard, "Master keyboard navigation enable flag.")
        .value("nav_enable_gamepad", ImGuiConfigFlags_NavEnableGamepad, "Master gamepad navigation enable flag. Backend also needs to set ImGuiBackendFlags_HasGamepad.")
        .value("nav_enable_set_mouse_pos", ImGuiConfigFlags_NavEnableSetMousePos, "Instruct navigation to move the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is awkward. Will update io.MousePos and set io.WantSetMousePos=True. If enabled you MUST honor io.WantSetMousePos requests in your backend, otherwise ImGui will react as if the mouse is jumping around back and forth.")
        .value("nav_no_capture_keyboard", ImGuiConfigFlags_NavNoCaptureKeyboard, "Instruct navigation to not set the io.WantCaptureKeyboard flag when io.NavActive is set.")
        .value("no_mouse", ImGuiConfigFlags_NoMouse, "Instruct imgui to clear mouse position/buttons in NewFrame(). This allows ignoring the mouse information set by the backend.")
        .value("no_mouse_cursor_change", ImGuiConfigFlags_NoMouseCursorChange, "Instruct backend to not alter mouse cursor shape and visibility. Use if the backend cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from imgui by reading GetMouseCursor() yourself instead.")
        .value("docking_enable", ImGuiConfigFlags_DockingEnable, "Docking enable flags.")
        .value("viewports_enable", ImGuiConfigFlags_ViewportsEnable, "Viewport enable flags (require both ImGuiBackendFlags_PlatformHasViewports + ImGuiBackendFlags_RendererHasViewports set by the respective backends)")
        .value("dpi_enable_scale_viewports", ImGuiConfigFlags_DpiEnableScaleViewports, "[BETA: Don't use] FIXME-DPI: Reposition and resize imgui windows when the DpiScale of a viewport changed (mostly useful for the main viewport hosting other window). Note that resizing the main window itself is up to your application.")
        .value("dpi_enable_scale_fonts", ImGuiConfigFlags_DpiEnableScaleFonts, "[BETA: Don't use] FIXME-DPI: Request bitmap-scaled fonts to match DpiScale. This is a very low-quality workaround. The correct way to handle DPI is _currently_ to replace the atlas and/or fonts in the Platform_OnChangedViewport callback, but this is all early work in progress.")
        .value("is_srgb", ImGuiConfigFlags_IsSRGB, "Application is SRGB-aware.")
        .value("is_touch_screen", ImGuiConfigFlags_IsTouchScreen, "Application is using a touch screen instead of a mouse.");


    py::enum_<ImGuiBackendFlags_>(m, "BackendFlags_", py::arithmetic(), "Backend capabilities flags stored in io.BackendFlags. Set by imgui_impl_xxx or custom backend.")    // imgui.h:1579
        .value("none", ImGuiBackendFlags_None, "")
        .value("has_gamepad", ImGuiBackendFlags_HasGamepad, "Backend Platform supports gamepad and currently has one connected.")
        .value("has_mouse_cursors", ImGuiBackendFlags_HasMouseCursors, "Backend Platform supports honoring GetMouseCursor() value to change the OS cursor shape.")
        .value("has_set_mouse_pos", ImGuiBackendFlags_HasSetMousePos, "Backend Platform supports io.WantSetMousePos requests to reposition the OS mouse position (only used if ImGuiConfigFlags_NavEnableSetMousePos is set).")
        .value("renderer_has_vtx_offset", ImGuiBackendFlags_RendererHasVtxOffset, "Backend Renderer supports ImDrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bit indices.")
        .value("platform_has_viewports", ImGuiBackendFlags_PlatformHasViewports, "Backend Platform supports multiple viewports.")
        .value("has_mouse_hovered_viewport", ImGuiBackendFlags_HasMouseHoveredViewport, "Backend Platform supports calling io.AddMouseViewportEvent() with the viewport under the mouse. IF POSSIBLE, ignore viewports with the ImGuiViewportFlags_NoInputs flag (Win32 backend, GLFW 3.30+ backend can do this, SDL backend cannot). If this cannot be done, Dear ImGui needs to use a flawed heuristic to find the viewport under.")
        .value("renderer_has_viewports", ImGuiBackendFlags_RendererHasViewports, "Backend Renderer supports multiple viewports.");


    py::enum_<ImGuiCol_>(m, "Col_", py::arithmetic(), "Enumeration for PushStyleColor() / PopStyleColor()")    // imgui.h:1594
        .value("text", ImGuiCol_Text, "")
        .value("text_disabled", ImGuiCol_TextDisabled, "")
        .value("window_bg", ImGuiCol_WindowBg, "Background of normal windows")
        .value("child_bg", ImGuiCol_ChildBg, "Background of child windows")
        .value("popup_bg", ImGuiCol_PopupBg, "Background of popups, menus, tooltips windows")
        .value("border", ImGuiCol_Border, "")
        .value("border_shadow", ImGuiCol_BorderShadow, "")
        .value("frame_bg", ImGuiCol_FrameBg, "Background of checkbox, radio button, plot, slider, text input")
        .value("frame_bg_hovered", ImGuiCol_FrameBgHovered, "")
        .value("frame_bg_active", ImGuiCol_FrameBgActive, "")
        .value("title_bg", ImGuiCol_TitleBg, "")
        .value("title_bg_active", ImGuiCol_TitleBgActive, "")
        .value("title_bg_collapsed", ImGuiCol_TitleBgCollapsed, "")
        .value("menu_bar_bg", ImGuiCol_MenuBarBg, "")
        .value("scrollbar_bg", ImGuiCol_ScrollbarBg, "")
        .value("scrollbar_grab", ImGuiCol_ScrollbarGrab, "")
        .value("scrollbar_grab_hovered", ImGuiCol_ScrollbarGrabHovered, "")
        .value("scrollbar_grab_active", ImGuiCol_ScrollbarGrabActive, "")
        .value("check_mark", ImGuiCol_CheckMark, "")
        .value("slider_grab", ImGuiCol_SliderGrab, "")
        .value("slider_grab_active", ImGuiCol_SliderGrabActive, "")
        .value("button", ImGuiCol_Button, "")
        .value("button_hovered", ImGuiCol_ButtonHovered, "")
        .value("button_active", ImGuiCol_ButtonActive, "")
        .value("header", ImGuiCol_Header, "Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem")
        .value("header_hovered", ImGuiCol_HeaderHovered, "")
        .value("header_active", ImGuiCol_HeaderActive, "")
        .value("separator", ImGuiCol_Separator, "")
        .value("separator_hovered", ImGuiCol_SeparatorHovered, "")
        .value("separator_active", ImGuiCol_SeparatorActive, "")
        .value("resize_grip", ImGuiCol_ResizeGrip, "Resize grip in lower-right and lower-left corners of windows.")
        .value("resize_grip_hovered", ImGuiCol_ResizeGripHovered, "")
        .value("resize_grip_active", ImGuiCol_ResizeGripActive, "")
        .value("tab", ImGuiCol_Tab, "TabItem in a TabBar")
        .value("tab_hovered", ImGuiCol_TabHovered, "")
        .value("tab_active", ImGuiCol_TabActive, "")
        .value("tab_unfocused", ImGuiCol_TabUnfocused, "")
        .value("tab_unfocused_active", ImGuiCol_TabUnfocusedActive, "")
        .value("docking_preview", ImGuiCol_DockingPreview, "Preview overlay color when about to docking something")
        .value("docking_empty_bg", ImGuiCol_DockingEmptyBg, "Background color for empty node (e.g. CentralNode with no window docked into it)")
        .value("plot_lines", ImGuiCol_PlotLines, "")
        .value("plot_lines_hovered", ImGuiCol_PlotLinesHovered, "")
        .value("plot_histogram", ImGuiCol_PlotHistogram, "")
        .value("plot_histogram_hovered", ImGuiCol_PlotHistogramHovered, "")
        .value("table_header_bg", ImGuiCol_TableHeaderBg, "Table header background")
        .value("table_border_strong", ImGuiCol_TableBorderStrong, "Table outer and header borders (prefer using Alpha=1.0 here)")
        .value("table_border_light", ImGuiCol_TableBorderLight, "Table inner borders (prefer using Alpha=1.0 here)")
        .value("table_row_bg", ImGuiCol_TableRowBg, "Table row background (even rows)")
        .value("table_row_bg_alt", ImGuiCol_TableRowBgAlt, "Table row background (odd rows)")
        .value("text_selected_bg", ImGuiCol_TextSelectedBg, "")
        .value("drag_drop_target", ImGuiCol_DragDropTarget, "Rectangle highlighting a drop target")
        .value("nav_highlight", ImGuiCol_NavHighlight, "Gamepad/keyboard: current highlighted item")
        .value("nav_windowing_highlight", ImGuiCol_NavWindowingHighlight, "Highlight window when using CTRL+TAB")
        .value("nav_windowing_dim_bg", ImGuiCol_NavWindowingDimBg, "Darken/colorize entire screen behind the CTRL+TAB window list, when active")
        .value("modal_window_dim_bg", ImGuiCol_ModalWindowDimBg, "Darken/colorize entire screen behind a modal window, when one is active")
        .value("count", ImGuiCol_COUNT, "");


    py::enum_<ImGuiStyleVar_>(m, "StyleVar_", py::arithmetic(), " Enumeration for PushStyleVar() / PopStyleVar() to temporarily modify the ImGuiStyle structure.\n - The enum only refers to fields of ImGuiStyle which makes sense to be pushed/popped inside UI code.\n   During initialization or between frames, feel free to just poke into ImGuiStyle directly.\n - Tip: Use your programming IDE navigation facilities on the names in the _second column_ below to find the actual members and their description.\n   In Visual Studio IDE: CTRL+comma (\"Edit.GoToAll\") can follow symbols in comments, whereas CTRL+F12 (\"Edit.GoToImplementation\") cannot.\n   With Visual Assist installed: ALT+G (\"VAssistX.GoToImplementation\") can also follow symbols in comments.\n - When changing this enum, you need to update the associated internal table GStyleVarInfo[] accordingly. This is where we link enum values to members offset/type.")    // imgui.h:1661
        .value("alpha", ImGuiStyleVar_Alpha, "float     Alpha")
        .value("disabled_alpha", ImGuiStyleVar_DisabledAlpha, "float     DisabledAlpha")
        .value("window_padding", ImGuiStyleVar_WindowPadding, "ImVec2    WindowPadding")
        .value("window_rounding", ImGuiStyleVar_WindowRounding, "float     WindowRounding")
        .value("window_border_size", ImGuiStyleVar_WindowBorderSize, "float     WindowBorderSize")
        .value("window_min_size", ImGuiStyleVar_WindowMinSize, "ImVec2    WindowMinSize")
        .value("window_title_align", ImGuiStyleVar_WindowTitleAlign, "ImVec2    WindowTitleAlign")
        .value("child_rounding", ImGuiStyleVar_ChildRounding, "float     ChildRounding")
        .value("child_border_size", ImGuiStyleVar_ChildBorderSize, "float     ChildBorderSize")
        .value("popup_rounding", ImGuiStyleVar_PopupRounding, "float     PopupRounding")
        .value("popup_border_size", ImGuiStyleVar_PopupBorderSize, "float     PopupBorderSize")
        .value("frame_padding", ImGuiStyleVar_FramePadding, "ImVec2    FramePadding")
        .value("frame_rounding", ImGuiStyleVar_FrameRounding, "float     FrameRounding")
        .value("frame_border_size", ImGuiStyleVar_FrameBorderSize, "float     FrameBorderSize")
        .value("item_spacing", ImGuiStyleVar_ItemSpacing, "ImVec2    ItemSpacing")
        .value("item_inner_spacing", ImGuiStyleVar_ItemInnerSpacing, "ImVec2    ItemInnerSpacing")
        .value("indent_spacing", ImGuiStyleVar_IndentSpacing, "float     IndentSpacing")
        .value("cell_padding", ImGuiStyleVar_CellPadding, "ImVec2    CellPadding")
        .value("scrollbar_size", ImGuiStyleVar_ScrollbarSize, "float     ScrollbarSize")
        .value("scrollbar_rounding", ImGuiStyleVar_ScrollbarRounding, "float     ScrollbarRounding")
        .value("grab_min_size", ImGuiStyleVar_GrabMinSize, "float     GrabMinSize")
        .value("grab_rounding", ImGuiStyleVar_GrabRounding, "float     GrabRounding")
        .value("tab_rounding", ImGuiStyleVar_TabRounding, "float     TabRounding")
        .value("button_text_align", ImGuiStyleVar_ButtonTextAlign, "ImVec2    ButtonTextAlign")
        .value("selectable_text_align", ImGuiStyleVar_SelectableTextAlign, "ImVec2    SelectableTextAlign")
        .value("count", ImGuiStyleVar_COUNT, "");


    py::enum_<ImGuiButtonFlags_>(m, "ButtonFlags_", py::arithmetic(), "Flags for InvisibleButton() [extended in imgui_internal.h]")    // imgui.h:1693
        .value("none", ImGuiButtonFlags_None, "")
        .value("mouse_button_left", ImGuiButtonFlags_MouseButtonLeft, "React on left mouse button (default)")
        .value("mouse_button_right", ImGuiButtonFlags_MouseButtonRight, "React on right mouse button")
        .value("mouse_button_middle", ImGuiButtonFlags_MouseButtonMiddle, "React on center mouse button")
        .value("mouse_button_mask_", ImGuiButtonFlags_MouseButtonMask_, "")
        .value("mouse_button_default_", ImGuiButtonFlags_MouseButtonDefault_, "");


    py::enum_<ImGuiColorEditFlags_>(m, "ColorEditFlags_", py::arithmetic(), "Flags for ColorEdit3() / ColorEdit4() / ColorPicker3() / ColorPicker4() / ColorButton()")    // imgui.h:1706
        .value("none", ImGuiColorEditFlags_None, "")
        .value("no_alpha", ImGuiColorEditFlags_NoAlpha, "// ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).")
        .value("no_picker", ImGuiColorEditFlags_NoPicker, "// ColorEdit: disable picker when clicking on color square.")
        .value("no_options", ImGuiColorEditFlags_NoOptions, "// ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.")
        .value("no_small_preview", ImGuiColorEditFlags_NoSmallPreview, "// ColorEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)")
        .value("no_inputs", ImGuiColorEditFlags_NoInputs, "// ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square).")
        .value("no_tooltip", ImGuiColorEditFlags_NoTooltip, "// ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.")
        .value("no_label", ImGuiColorEditFlags_NoLabel, "// ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).")
        .value("no_side_preview", ImGuiColorEditFlags_NoSidePreview, "// ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.")
        .value("no_drag_drop", ImGuiColorEditFlags_NoDragDrop, "// ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.")
        .value("no_border", ImGuiColorEditFlags_NoBorder, "// ColorButton: disable border (which is enforced by default)")
        .value("alpha_bar", ImGuiColorEditFlags_AlphaBar, "// ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.")
        .value("alpha_preview", ImGuiColorEditFlags_AlphaPreview, "// ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.")
        .value("alpha_preview_half", ImGuiColorEditFlags_AlphaPreviewHalf, "// ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.")
        .value("hdr", ImGuiColorEditFlags_HDR, "// (WIP) ColorEdit: Currently only disable 0.0..1.0 limits in RGBA edition (note: you probably want to use ImGuiColorEditFlags_Float flag as well).")
        .value("display_rgb", ImGuiColorEditFlags_DisplayRGB, "[Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.")
        .value("display_hsv", ImGuiColorEditFlags_DisplayHSV, "[Display]    // \"")
        .value("display_hex", ImGuiColorEditFlags_DisplayHex, "[Display]    // \"")
        .value("uint8", ImGuiColorEditFlags_Uint8, "[DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.")
        .value("float", ImGuiColorEditFlags_Float, "[DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0..1.0 floats instead of 0..255 integers. No round-trip of value via integers.")
        .value("picker_hue_bar", ImGuiColorEditFlags_PickerHueBar, "[Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.")
        .value("picker_hue_wheel", ImGuiColorEditFlags_PickerHueWheel, "[Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.")
        .value("input_rgb", ImGuiColorEditFlags_InputRGB, "[Input]      // ColorEdit, ColorPicker: input and output data in RGB format.")
        .value("input_hsv", ImGuiColorEditFlags_InputHSV, "[Input]      // ColorEdit, ColorPicker: input and output data in HSV format.")
        .value("default_options_", ImGuiColorEditFlags_DefaultOptions_, " Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to\n override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.")
        .value("display_mask_", ImGuiColorEditFlags_DisplayMask_, "")
        .value("data_type_mask_", ImGuiColorEditFlags_DataTypeMask_, "")
        .value("picker_mask_", ImGuiColorEditFlags_PickerMask_, "")
        .value("input_mask_", ImGuiColorEditFlags_InputMask_, "");


    py::enum_<ImGuiSliderFlags_>(m, "SliderFlags_", py::arithmetic(), " Flags for DragFloat(), DragInt(), SliderFloat(), SliderInt() etc.\n We use the same sets of flags for DragXXX() and SliderXXX() functions as the features are the same and it makes it easier to swap them.\n (Those are per-item flags. There are shared flags in ImGuiIO: io.ConfigDragClickToInputText)")    // imgui.h:1752
        .value("none", ImGuiSliderFlags_None, "")
        .value("always_clamp", ImGuiSliderFlags_AlwaysClamp, "Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.")
        .value("logarithmic", ImGuiSliderFlags_Logarithmic, "Make the widget logarithmic (linear otherwise). Consider using ImGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.")
        .value("no_round_to_format", ImGuiSliderFlags_NoRoundToFormat, "Disable rounding underlying value to match precision of the display format string (e.g. %.3 values are rounded to those 3 digits)")
        .value("no_input", ImGuiSliderFlags_NoInput, "Disable CTRL+Click or Enter key allowing to input text directly into the widget")
        .value("invalid_mask_", ImGuiSliderFlags_InvalidMask_, "[Internal] We treat using those bits as being potentially a 'float power' argument from the previous API that has got miscast to this enum, and will trigger an assert if needed.");


    py::enum_<ImGuiMouseButton_>(m, "MouseButton_", py::arithmetic(), " Identify a mouse button.\n Those values are guaranteed to be stable and we frequently use 0/1 directly. Named enums provided for convenience.")    // imgui.h:1769
        .value("left", ImGuiMouseButton_Left, "")
        .value("right", ImGuiMouseButton_Right, "")
        .value("middle", ImGuiMouseButton_Middle, "")
        .value("count", ImGuiMouseButton_COUNT, "");


    py::enum_<ImGuiMouseCursor_>(m, "MouseCursor_", py::arithmetic(), " Enumeration for GetMouseCursor()\n User code may request backend to display given cursor by calling SetMouseCursor(), which is why we have some cursors that are marked unused here")    // imgui.h:1779
        .value("none", ImGuiMouseCursor_None, "")
        .value("arrow", ImGuiMouseCursor_Arrow, "")
        .value("text_input", ImGuiMouseCursor_TextInput, "When hovering over InputText, etc.")
        .value("resize_all", ImGuiMouseCursor_ResizeAll, "(Unused by Dear ImGui functions)")
        .value("resize_ns", ImGuiMouseCursor_ResizeNS, "When hovering over a horizontal border")
        .value("resize_ew", ImGuiMouseCursor_ResizeEW, "When hovering over a vertical border or a column")
        .value("resize_nesw", ImGuiMouseCursor_ResizeNESW, "When hovering over the bottom-left corner of a window")
        .value("resize_nwse", ImGuiMouseCursor_ResizeNWSE, "When hovering over the bottom-right corner of a window")
        .value("hand", ImGuiMouseCursor_Hand, "(Unused by Dear ImGui functions. Use for e.g. hyperlinks)")
        .value("not_allowed", ImGuiMouseCursor_NotAllowed, "When hovering something with disallowed interaction. Usually a crossed circle.")
        .value("count", ImGuiMouseCursor_COUNT, "");


    py::enum_<ImGuiCond_>(m, "Cond_", py::arithmetic(), " Enumeration for ImGui::SetWindow***(), SetNextWindow***(), SetNextItem***() functions\n Represent a condition.\n Important: Treat as a regular enum! Do NOT combine multiple values using binary operators! All the functions above treat 0 as a shortcut to ImGuiCond_Always.")    // imgui.h:1797
        .value("none", ImGuiCond_None, "No condition (always set the variable), same as _Always")
        .value("always", ImGuiCond_Always, "No condition (always set the variable), same as _None")
        .value("once", ImGuiCond_Once, "Set the variable once per runtime session (only the first call will succeed)")
        .value("first_use_ever", ImGuiCond_FirstUseEver, "Set the variable if the object/window has no persistently saved data (no entry in .ini file)")
        .value("appearing", ImGuiCond_Appearing, "Set the variable if the object/window is appearing after being hidden/inactive (or the first time)");


    auto pyClassImNewWrapper =
        py::class_<ImNewWrapper>    // imgui.h:1816
            (m, "ImNewWrapper", "")
        .def(py::init<>()) // implicit default constructor
        ;


    auto pyClassImGuiStyle =
        py::class_<ImGuiStyle>    // imgui.h:1909
            (m, "Style", "")
        .def_readwrite("alpha", &ImGuiStyle::Alpha, "Global alpha applies to everything in Dear ImGui.")    // imgui.h:1911
        .def_readwrite("disabled_alpha", &ImGuiStyle::DisabledAlpha, "Additional alpha multiplier applied by BeginDisabled(). Multiply over current value of Alpha.")    // imgui.h:1912
        .def_readwrite("window_padding", &ImGuiStyle::WindowPadding, "Padding within a window.")    // imgui.h:1913
        .def_readwrite("window_rounding", &ImGuiStyle::WindowRounding, "Radius of window corners rounding. Set to 0.0 to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.")    // imgui.h:1914
        .def_readwrite("window_border_size", &ImGuiStyle::WindowBorderSize, "Thickness of border around windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).")    // imgui.h:1915
        .def_readwrite("window_min_size", &ImGuiStyle::WindowMinSize, "Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().")    // imgui.h:1916
        .def_readwrite("window_title_align", &ImGuiStyle::WindowTitleAlign, "Alignment for title bar text. Defaults to (0.0,0.5) for left-aligned,vertically centered.")    // imgui.h:1917
        .def_readwrite("window_menu_button_position", &ImGuiStyle::WindowMenuButtonPosition, "Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left.")    // imgui.h:1918
        .def_readwrite("child_rounding", &ImGuiStyle::ChildRounding, "Radius of child window corners rounding. Set to 0.0 to have rectangular windows.")    // imgui.h:1919
        .def_readwrite("child_border_size", &ImGuiStyle::ChildBorderSize, "Thickness of border around child windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).")    // imgui.h:1920
        .def_readwrite("popup_rounding", &ImGuiStyle::PopupRounding, "Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)")    // imgui.h:1921
        .def_readwrite("popup_border_size", &ImGuiStyle::PopupBorderSize, "Thickness of border around popup/tooltip windows. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).")    // imgui.h:1922
        .def_readwrite("frame_padding", &ImGuiStyle::FramePadding, "Padding within a framed rectangle (used by most widgets).")    // imgui.h:1923
        .def_readwrite("frame_rounding", &ImGuiStyle::FrameRounding, "Radius of frame corners rounding. Set to 0.0 to have rectangular frame (used by most widgets).")    // imgui.h:1924
        .def_readwrite("frame_border_size", &ImGuiStyle::FrameBorderSize, "Thickness of border around frames. Generally set to 0.0 or 1.0. (Other values are not well tested and more CPU/GPU costly).")    // imgui.h:1925
        .def_readwrite("item_spacing", &ImGuiStyle::ItemSpacing, "Horizontal and vertical spacing between widgets/lines.")    // imgui.h:1926
        .def_readwrite("item_inner_spacing", &ImGuiStyle::ItemInnerSpacing, "Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).")    // imgui.h:1927
        .def_readwrite("cell_padding", &ImGuiStyle::CellPadding, "Padding within a table cell")    // imgui.h:1928
        .def_readwrite("touch_extra_padding", &ImGuiStyle::TouchExtraPadding, "Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!")    // imgui.h:1929
        .def_readwrite("indent_spacing", &ImGuiStyle::IndentSpacing, "Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).")    // imgui.h:1930
        .def_readwrite("columns_min_spacing", &ImGuiStyle::ColumnsMinSpacing, "Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).")    // imgui.h:1931
        .def_readwrite("scrollbar_size", &ImGuiStyle::ScrollbarSize, "Width of the vertical scrollbar, Height of the horizontal scrollbar.")    // imgui.h:1932
        .def_readwrite("scrollbar_rounding", &ImGuiStyle::ScrollbarRounding, "Radius of grab corners for scrollbar.")    // imgui.h:1933
        .def_readwrite("grab_min_size", &ImGuiStyle::GrabMinSize, "Minimum width/height of a grab box for slider/scrollbar.")    // imgui.h:1934
        .def_readwrite("grab_rounding", &ImGuiStyle::GrabRounding, "Radius of grabs corners rounding. Set to 0.0 to have rectangular slider grabs.")    // imgui.h:1935
        .def_readwrite("log_slider_deadzone", &ImGuiStyle::LogSliderDeadzone, "The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.")    // imgui.h:1936
        .def_readwrite("tab_rounding", &ImGuiStyle::TabRounding, "Radius of upper corners of a tab. Set to 0.0 to have rectangular tabs.")    // imgui.h:1937
        .def_readwrite("tab_border_size", &ImGuiStyle::TabBorderSize, "Thickness of border around tabs.")    // imgui.h:1938
        .def_readwrite("tab_min_width_for_close_button", &ImGuiStyle::TabMinWidthForCloseButton, "Minimum width for close button to appear on an unselected tab when hovered. Set to 0.0 to always show when hovering, set to FLT_MAX to never show close button unless selected.")    // imgui.h:1939
        .def_readwrite("color_button_position", &ImGuiStyle::ColorButtonPosition, "Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right.")    // imgui.h:1940
        .def_readwrite("button_text_align", &ImGuiStyle::ButtonTextAlign, "Alignment of button text when button is larger than text. Defaults to (0.5, 0.5) (centered).")    // imgui.h:1941
        .def_readwrite("selectable_text_align", &ImGuiStyle::SelectableTextAlign, "Alignment of selectable text. Defaults to (0.0, 0.0) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.")    // imgui.h:1942
        .def_readwrite("display_window_padding", &ImGuiStyle::DisplayWindowPadding, "Window position are clamped to be visible within the display area or monitors by at least this amount. Only applies to regular windows.")    // imgui.h:1943
        .def_readwrite("display_safe_area_padding", &ImGuiStyle::DisplaySafeAreaPadding, "If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!")    // imgui.h:1944
        .def_readwrite("mouse_cursor_scale", &ImGuiStyle::MouseCursorScale, "Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). We apply per-monitor DPI scaling over this scale. May be removed later.")    // imgui.h:1945
        .def_readwrite("anti_aliased_lines", &ImGuiStyle::AntiAliasedLines, "Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).")    // imgui.h:1946
        .def_readwrite("anti_aliased_lines_use_tex", &ImGuiStyle::AntiAliasedLinesUseTex, "Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to ImDrawList).")    // imgui.h:1947
        .def_readwrite("anti_aliased_fill", &ImGuiStyle::AntiAliasedFill, "Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to ImDrawList).")    // imgui.h:1948
        .def_readwrite("curve_tessellation_tol", &ImGuiStyle::CurveTessellationTol, "Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.")    // imgui.h:1949
        .def_readwrite("circle_tessellation_max_error", &ImGuiStyle::CircleTessellationMaxError, "Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.")    // imgui.h:1950
        .def(py::init<>())    // imgui.h:1953
        .def("scale_all_sizes",    // imgui.h:1954
            &ImGuiStyle::ScaleAllSizes, py::arg("scale_factor"))
        ;


    auto pyClassImGuiKeyData =
        py::class_<ImGuiKeyData>    // imgui.h:1966
            (m, "KeyData", " [Internal] Storage used by IsKeyDown(), IsKeyPressed() etc functions.\n If prior to 1.87 you used io.KeysDownDuration[] (which was marked as internal), you should use GetKeyData(key)->DownDuration and *NOT* io.KeysData[key]->DownDuration.")
        .def(py::init<>([](
        bool Down = bool(), float DownDuration = float(), float DownDurationPrev = float(), float AnalogValue = float())
        {
            auto r = std::make_unique<ImGuiKeyData>();
            r->Down = Down;
            r->DownDuration = DownDuration;
            r->DownDurationPrev = DownDurationPrev;
            r->AnalogValue = AnalogValue;
            return r;
        })
        , py::arg("down") = bool(), py::arg("down_duration") = float(), py::arg("down_duration_prev") = float(), py::arg("analog_value") = float()
        )
        .def_readwrite("down", &ImGuiKeyData::Down, "True for if key is down")    // imgui.h:1968
        .def_readwrite("down_duration", &ImGuiKeyData::DownDuration, "Duration the key has been down (<0.0: not pressed, 0.0: just pressed, >0.0: time held)")    // imgui.h:1969
        .def_readwrite("down_duration_prev", &ImGuiKeyData::DownDurationPrev, "Last frame duration the key has been down")    // imgui.h:1970
        .def_readwrite("analog_value", &ImGuiKeyData::AnalogValue, "0.0..1.0 for gamepad values")    // imgui.h:1971
        ;


    auto pyClassImGuiIO =
        py::class_<ImGuiIO>    // imgui.h:1974
            (m, "IO", "")
        .def_readwrite("config_flags", &ImGuiIO::ConfigFlags, "= 0              // See ImGuiConfigFlags_ enum. Set by user/application. Gamepad/keyboard navigation options, etc.")    // imgui.h:1980
        .def_readwrite("backend_flags", &ImGuiIO::BackendFlags, "= 0              // See ImGuiBackendFlags_ enum. Set by backend (imgui_impl_xxx files or custom backend) to communicate features supported by the backend.")    // imgui.h:1981
        .def_readwrite("display_size", &ImGuiIO::DisplaySize, "<unset>          // Main display size, in pixels (generally == GetMainViewport()->Size). May change every frame.")    // imgui.h:1982
        .def_readwrite("delta_time", &ImGuiIO::DeltaTime, "= 1.0/60.0     // Time elapsed since last frame, in seconds. May change every frame.")    // imgui.h:1983
        .def_readwrite("ini_saving_rate", &ImGuiIO::IniSavingRate, "= 5.0           // Minimum time between saving positions/sizes to .ini file, in seconds.")    // imgui.h:1984
        .def_readonly("ini_filename", &ImGuiIO::IniFilename, "= \"imgui.ini\"    // Path to .ini file (important: default \"imgui.ini\" is relative to current working dir!). Set None to disable automatic .ini loading/saving or if you want to manually call LoadIniSettingsXXX() / SaveIniSettingsXXX() functions.")    // imgui.h:1985
        .def_readonly("log_filename", &ImGuiIO::LogFilename, "= \"imgui_log.txt\"// Path to .log file (default parameter to ImGui::LogToFile when no file is specified).")    // imgui.h:1986
        .def_readwrite("mouse_double_click_time", &ImGuiIO::MouseDoubleClickTime, "= 0.30          // Time for a double-click, in seconds.")    // imgui.h:1987
        .def_readwrite("mouse_double_click_max_dist", &ImGuiIO::MouseDoubleClickMaxDist, "= 6.0           // Distance threshold to stay in to validate a double-click, in pixels.")    // imgui.h:1988
        .def_readwrite("mouse_drag_threshold", &ImGuiIO::MouseDragThreshold, "= 6.0           // Distance threshold before considering we are dragging.")    // imgui.h:1989
        .def_readwrite("key_repeat_delay", &ImGuiIO::KeyRepeatDelay, "= 0.275         // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).")    // imgui.h:1990
        .def_readwrite("key_repeat_rate", &ImGuiIO::KeyRepeatRate, "= 0.050         // When holding a key/button, rate at which it repeats, in seconds.")    // imgui.h:1991
        .def_readwrite("hover_delay_normal", &ImGuiIO::HoverDelayNormal, "= 0.30 sec       // Delay on hovering before IsItemHovered(ImGuiHoveredFlags_DelayNormal) returns True.")    // imgui.h:1992
        .def_readwrite("hover_delay_short", &ImGuiIO::HoverDelayShort, "= 0.10 sec       // Delay on hovering before IsItemHovered(ImGuiHoveredFlags_DelayShort) returns True.")    // imgui.h:1993
        .def_readwrite("user_data", &ImGuiIO::UserData, "= None           // Store your own data for retrieval by callbacks.")    // imgui.h:1994
        .def_readwrite("fonts", &ImGuiIO::Fonts, "<auto>           // Font atlas: load, rasterize and pack one or more fonts into a single texture.")    // imgui.h:1996
        .def_readwrite("font_global_scale", &ImGuiIO::FontGlobalScale, "= 1.0           // Global scale all fonts")    // imgui.h:1997
        .def_readwrite("font_allow_user_scaling", &ImGuiIO::FontAllowUserScaling, "= False          // Allow user scaling text of individual window with CTRL+Wheel.")    // imgui.h:1998
        .def_readwrite("font_default", &ImGuiIO::FontDefault, "= None           // Font to use on NewFrame(). Use None to uses Fonts->Fonts[0].")    // imgui.h:1999
        .def_readwrite("display_framebuffer_scale", &ImGuiIO::DisplayFramebufferScale, "= (1, 1)         // For retina display or other situations where window coordinates are different from framebuffer coordinates. This generally ends up in ImDrawData::FramebufferScale.")    // imgui.h:2000
        .def_readwrite("config_docking_no_split", &ImGuiIO::ConfigDockingNoSplit, "= False          // Simplified docking mode: disable window splitting, so docking is limited to merging multiple windows together into tab-bars.")    // imgui.h:2003
        .def_readwrite("config_docking_with_shift", &ImGuiIO::ConfigDockingWithShift, "= False          // Enable docking with holding Shift key (reduce visual noise, allows dropping in wider space)")    // imgui.h:2004
        .def_readwrite("config_docking_always_tab_bar", &ImGuiIO::ConfigDockingAlwaysTabBar, "= False          // [BETA] [FIXME: This currently creates regression with auto-sizing and general overhead] Make every single floating window display within a docking node.")    // imgui.h:2005
        .def_readwrite("config_docking_transparent_payload", &ImGuiIO::ConfigDockingTransparentPayload, "= False          // [BETA] Make window or viewport transparent when docking and only display docking boxes on the target viewport. Useful if rendering of multiple viewport cannot be synced. Best used with ConfigViewportsNoAutoMerge.")    // imgui.h:2006
        .def_readwrite("config_viewports_no_auto_merge", &ImGuiIO::ConfigViewportsNoAutoMerge, "= False;         // Set to make all floating imgui windows always create their own viewport. Otherwise, they are merged into the main host viewports when overlapping it. May also set ImGuiViewportFlags_NoAutoMerge on individual viewport.")    // imgui.h:2009
        .def_readwrite("config_viewports_no_task_bar_icon", &ImGuiIO::ConfigViewportsNoTaskBarIcon, "= False          // Disable default OS task bar icon flag for secondary viewports. When a viewport doesn't want a task bar icon, ImGuiViewportFlags_NoTaskBarIcon will be set on it.")    // imgui.h:2010
        .def_readwrite("config_viewports_no_decoration", &ImGuiIO::ConfigViewportsNoDecoration, "= True           // Disable default OS window decoration flag for secondary viewports. When a viewport doesn't want window decorations, ImGuiViewportFlags_NoDecoration will be set on it. Enabling decoration can create subsequent issues at OS levels (e.g. minimum window size).")    // imgui.h:2011
        .def_readwrite("config_viewports_no_default_parent", &ImGuiIO::ConfigViewportsNoDefaultParent, "= False          // Disable default OS parenting to main viewport for secondary viewports. By default, viewports are marked with ParentViewportId = <main_viewport>, expecting the platform backend to setup a parent/child relationship between the OS windows (some backend may ignore this). Set to True if you want the default to be 0, then all viewports will be top-level OS windows.")    // imgui.h:2012
        .def_readwrite("mouse_draw_cursor", &ImGuiIO::MouseDrawCursor, "= False          // Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor). Cannot be easily renamed to 'io.ConfigXXX' because this is frequently used by backend implementations.")    // imgui.h:2015
        .def_readwrite("config_mac_osx_behaviors", &ImGuiIO::ConfigMacOSXBehaviors, "= defined(__APPLE__) // OS X style: Text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl.")    // imgui.h:2016
        .def_readwrite("config_input_trickle_event_queue", &ImGuiIO::ConfigInputTrickleEventQueue, "= True           // Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, improving interactions with low framerates.")    // imgui.h:2017
        .def_readwrite("config_input_text_cursor_blink", &ImGuiIO::ConfigInputTextCursorBlink, "= True           // Enable blinking cursor (optional as some users consider it to be distracting).")    // imgui.h:2018
        .def_readwrite("config_input_text_enter_keep_active", &ImGuiIO::ConfigInputTextEnterKeepActive, "= False          // [BETA] Pressing Enter will keep item active and select contents (single-line only).")    // imgui.h:2019
        .def_readwrite("config_drag_click_to_input_text", &ImGuiIO::ConfigDragClickToInputText, "= False          // [BETA] Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving). Not desirable on devices without a keyboard.")    // imgui.h:2020
        .def_readwrite("config_windows_resize_from_edges", &ImGuiIO::ConfigWindowsResizeFromEdges, "= True           // Enable resizing of windows from their edges and from the lower-left corner. This requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback. (This used to be a per-window ImGuiWindowFlags_ResizeFromAnySide flag)")    // imgui.h:2021
        .def_readwrite("config_windows_move_from_title_bar_only", &ImGuiIO::ConfigWindowsMoveFromTitleBarOnly, "= False       // Enable allowing to move windows only when clicking on their title bar. Does not apply to windows without a title bar.")    // imgui.h:2022
        .def_readwrite("config_memory_compact_timer", &ImGuiIO::ConfigMemoryCompactTimer, "= 60.0          // Timer (in seconds) to free transient windows/tables memory buffers when unused. Set to -1.0 to disable.")    // imgui.h:2023
        .def_readonly("backend_platform_name", &ImGuiIO::BackendPlatformName, "= None")    // imgui.h:2031
        .def_readonly("backend_renderer_name", &ImGuiIO::BackendRendererName, "= None")    // imgui.h:2032
        .def_readwrite("backend_platform_user_data", &ImGuiIO::BackendPlatformUserData, "= None           // User data for platform backend")    // imgui.h:2033
        .def_readwrite("backend_renderer_user_data", &ImGuiIO::BackendRendererUserData, "= None           // User data for renderer backend")    // imgui.h:2034
        .def_readwrite("backend_language_user_data", &ImGuiIO::BackendLanguageUserData, "= None           // User data for non C++ programming language backend")    // imgui.h:2035
        .def_readwrite("clipboard_user_data", &ImGuiIO::ClipboardUserData, "")    // imgui.h:2041
        .def("add_key_event",    // imgui.h:2057
            &ImGuiIO::AddKeyEvent,
            py::arg("key"), py::arg("down"),
            "Queue a new key down/up event. Key should be \"translated\" (as in, generally ImGuiKey_A matches the key end-user would use to emit an 'A' character)")
        .def("add_key_analog_event",    // imgui.h:2058
            &ImGuiIO::AddKeyAnalogEvent,
            py::arg("key"), py::arg("down"), py::arg("v"),
            "Queue a new key down/up event for analog values (e.g. ImGuiKey_Gamepad_ values). Dead-zones should be handled by the backend.")
        .def("add_mouse_pos_event",    // imgui.h:2059
            &ImGuiIO::AddMousePosEvent,
            py::arg("x"), py::arg("y"),
            "Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)")
        .def("add_mouse_button_event",    // imgui.h:2060
            &ImGuiIO::AddMouseButtonEvent,
            py::arg("button"), py::arg("down"),
            "Queue a mouse button change")
        .def("add_mouse_wheel_event",    // imgui.h:2061
            &ImGuiIO::AddMouseWheelEvent,
            py::arg("wh_x"), py::arg("wh_y"),
            "Queue a mouse wheel update")
        .def("add_mouse_viewport_event",    // imgui.h:2062
            &ImGuiIO::AddMouseViewportEvent,
            py::arg("id_"),
            "Queue a mouse hovered viewport. Requires backend to set ImGuiBackendFlags_HasMouseHoveredViewport to call this (for multi-viewport support).")
        .def("add_focus_event",    // imgui.h:2063
            &ImGuiIO::AddFocusEvent,
            py::arg("focused"),
            "Queue a gain/loss of focus for the application (generally based on OS/platform focus of your window)")
        .def("add_input_character",    // imgui.h:2064
            &ImGuiIO::AddInputCharacter,
            py::arg("c"),
            "Queue a new character input")
        .def("add_input_character_utf16",    // imgui.h:2065
            &ImGuiIO::AddInputCharacterUTF16,
            py::arg("c"),
            "Queue a new character input from a UTF-16 character, it can be a surrogate")
        .def("add_input_characters_utf8",    // imgui.h:2066
            &ImGuiIO::AddInputCharactersUTF8,
            py::arg("str"),
            "Queue a new characters input from a UTF-8 string")
        .def("set_key_event_native_data",    // imgui.h:2068
            &ImGuiIO::SetKeyEventNativeData,
            py::arg("key"), py::arg("native_keycode"), py::arg("native_scancode"), py::arg("native_legacy_index") = -1,
            "[Optional] Specify index for legacy <1.87 IsKeyXXX() functions with native indices + specify native keycode, scancode.")
        .def("set_app_accepting_events",    // imgui.h:2069
            &ImGuiIO::SetAppAcceptingEvents,
            py::arg("accepting_events"),
            "Set master flag for accepting key/mouse/text events (default to True). Useful if you have native dialog boxes that are interrupting your application loop/refresh, and you want to disable events being queued while your app is frozen.")
        .def("clear_input_characters",    // imgui.h:2070
            &ImGuiIO::ClearInputCharacters, "[Internal] Clear the text input buffer manually")
        .def("clear_input_keys",    // imgui.h:2071
            &ImGuiIO::ClearInputKeys, "[Internal] Release all keys")
        .def_readwrite("want_capture_mouse", &ImGuiIO::WantCaptureMouse, "Set when Dear ImGui will use mouse inputs, in this case do not dispatch them to your main game/application (either way, always pass on mouse inputs to imgui). (e.g. unclicked mouse is hovering over an imgui window, widget is active, mouse was clicked over an imgui window, etc.).")    // imgui.h:2079
        .def_readwrite("want_capture_keyboard", &ImGuiIO::WantCaptureKeyboard, "Set when Dear ImGui will use keyboard inputs, in this case do not dispatch them to your main game/application (either way, always pass keyboard inputs to imgui). (e.g. InputText active, or an imgui window is focused and navigation is enabled, etc.).")    // imgui.h:2080
        .def_readwrite("want_text_input", &ImGuiIO::WantTextInput, "Mobile/console: when set, you may display an on-screen keyboard. This is set by Dear ImGui when it wants textual keyboard input to happen (e.g. when a InputText widget is active).")    // imgui.h:2081
        .def_readwrite("want_set_mouse_pos", &ImGuiIO::WantSetMousePos, "MousePos has been altered, backend should reposition mouse on next frame. Rarely used! Set only when ImGuiConfigFlags_NavEnableSetMousePos flag is enabled.")    // imgui.h:2082
        .def_readwrite("want_save_ini_settings", &ImGuiIO::WantSaveIniSettings, "When manual .ini load/save is active (io.IniFilename == None), this will be set to notify your application that you can call SaveIniSettingsToMemory() and save yourself. Important: clear io.WantSaveIniSettings yourself after saving!")    // imgui.h:2083
        .def_readwrite("nav_active", &ImGuiIO::NavActive, "Keyboard/Gamepad navigation is currently allowed (will handle ImGuiKey_NavXXX events) = a window is focused and it doesn't use the ImGuiWindowFlags_NoNavInputs flag.")    // imgui.h:2084
        .def_readwrite("nav_visible", &ImGuiIO::NavVisible, "Keyboard/Gamepad navigation is visible and allowed (will handle ImGuiKey_NavXXX events).")    // imgui.h:2085
        .def_readwrite("framerate", &ImGuiIO::Framerate, "Estimate of application framerate (rolling average over 60 frames, based on io.DeltaTime), in frame per second. Solely for convenience. Slow applications may not want to use a moving average or may want to reset underlying buffers occasionally.")    // imgui.h:2086
        .def_readwrite("metrics_render_vertices", &ImGuiIO::MetricsRenderVertices, "Vertices output during last call to Render()")    // imgui.h:2087
        .def_readwrite("metrics_render_indices", &ImGuiIO::MetricsRenderIndices, "Indices output during last call to Render() = number of triangles * 3")    // imgui.h:2088
        .def_readwrite("metrics_render_windows", &ImGuiIO::MetricsRenderWindows, "Number of visible windows")    // imgui.h:2089
        .def_readwrite("metrics_active_windows", &ImGuiIO::MetricsActiveWindows, "Number of active windows")    // imgui.h:2090
        .def_readwrite("metrics_active_allocations", &ImGuiIO::MetricsActiveAllocations, "Number of active allocations, updated by MemAlloc/MemFree based on current context. May be off if you have multiple imgui contexts.")    // imgui.h:2091
        .def_readwrite("mouse_delta", &ImGuiIO::MouseDelta, "")    // imgui.h:2092
        .def_readwrite("mouse_pos", &ImGuiIO::MousePos, "Mouse position, in pixels. Set to ImVec2(-FLT_MAX, -FLT_MAX) if mouse is unavailable (on another screen, etc.)")    // imgui.h:2110
        .def_property("mouse_down",    // imgui.h:2111
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<bool>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(bool)});
                return pybind11::array(dtype, {5}, {sizeof(bool)}, self.MouseDown, base);
            }, [](ImGuiIO& self) {},
            "Mouse buttons: 0=left, 1=right, 2=middle + extras (ImGuiMouseButton_COUNT == 5). Dear ImGui mostly uses left and right buttons. Other buttons allow us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.")
        .def_readwrite("mouse_wheel", &ImGuiIO::MouseWheel, "Mouse wheel Vertical: 1 unit scrolls about 5 lines text.")    // imgui.h:2112
        .def_readwrite("mouse_wheel_h", &ImGuiIO::MouseWheelH, "Mouse wheel Horizontal. Most users don't have a mouse with a horizontal wheel, may not be filled by all backends.")    // imgui.h:2113
        .def_readwrite("mouse_hovered_viewport", &ImGuiIO::MouseHoveredViewport, "(Optional) Modify using io.AddMouseViewportEvent(). With multi-viewports: viewport the OS mouse is hovering. If possible _IGNORING_ viewports with the ImGuiViewportFlags_NoInputs flag is much better (few backends can handle that). Set io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport if you can provide this info. If you don't imgui will infer the value using the rectangles and last focused time of the viewports it knows about (ignoring other OS windows).")    // imgui.h:2114
        .def_readwrite("key_ctrl", &ImGuiIO::KeyCtrl, "Keyboard modifier down: Control")    // imgui.h:2115
        .def_readwrite("key_shift", &ImGuiIO::KeyShift, "Keyboard modifier down: Shift")    // imgui.h:2116
        .def_readwrite("key_alt", &ImGuiIO::KeyAlt, "Keyboard modifier down: Alt")    // imgui.h:2117
        .def_readwrite("key_super", &ImGuiIO::KeySuper, "Keyboard modifier down: Cmd/Super/Windows")    // imgui.h:2118
        .def_readwrite("key_mods", &ImGuiIO::KeyMods, "Key mods flags (any of ImGuiMod_Ctrl/ImGuiMod_Shift/ImGuiMod_Alt/ImGuiMod_Super flags, same as io.KeyCtrl/KeyShift/KeyAlt/KeySuper but merged into flags). Read-only, updated by NewFrame()")    // imgui.h:2121
        .def_readwrite("want_capture_mouse_unless_popup_close", &ImGuiIO::WantCaptureMouseUnlessPopupClose, "Alternative to WantCaptureMouse: (WantCaptureMouse == True && WantCaptureMouseUnlessPopupClose == False) when a click over None is expected to close a popup.")    // imgui.h:2123
        .def_readwrite("mouse_pos_prev", &ImGuiIO::MousePosPrev, "Previous mouse position (note that MouseDelta is not necessary == MousePos-MousePosPrev, in case either position is invalid)")    // imgui.h:2124
        .def_property("mouse_clicked_time",    // imgui.h:2126
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<double>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(double)});
                return pybind11::array(dtype, {5}, {sizeof(double)}, self.MouseClickedTime, base);
            }, [](ImGuiIO& self) {},
            "Time of last click (used to figure out double-click)")
        .def_property("mouse_clicked",    // imgui.h:2127
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<bool>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(bool)});
                return pybind11::array(dtype, {5}, {sizeof(bool)}, self.MouseClicked, base);
            }, [](ImGuiIO& self) {},
            "Mouse button went from !Down to Down (same as MouseClickedCount[x] != 0)")
        .def_property("mouse_double_clicked",    // imgui.h:2128
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<bool>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(bool)});
                return pybind11::array(dtype, {5}, {sizeof(bool)}, self.MouseDoubleClicked, base);
            }, [](ImGuiIO& self) {},
            "Has mouse button been double-clicked? (same as MouseClickedCount[x] == 2)")
        .def_property("mouse_clicked_count",    // imgui.h:2129
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<ImU16>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(ImU16)});
                return pybind11::array(dtype, {5}, {sizeof(ImU16)}, self.MouseClickedCount, base);
            }, [](ImGuiIO& self) {},
            "== 0 (not clicked), == 1 (same as MouseClicked[]), == 2 (double-clicked), == 3 (triple-clicked) etc. when going from !Down to Down")
        .def_property("mouse_clicked_last_count",    // imgui.h:2130
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<ImU16>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(ImU16)});
                return pybind11::array(dtype, {5}, {sizeof(ImU16)}, self.MouseClickedLastCount, base);
            }, [](ImGuiIO& self) {},
            "Count successive number of clicks. Stays valid after mouse release. Reset after another click is done.")
        .def_property("mouse_released",    // imgui.h:2131
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<bool>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(bool)});
                return pybind11::array(dtype, {5}, {sizeof(bool)}, self.MouseReleased, base);
            }, [](ImGuiIO& self) {},
            "Mouse button went from Down to !Down")
        .def_property("mouse_down_owned",    // imgui.h:2132
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<bool>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(bool)});
                return pybind11::array(dtype, {5}, {sizeof(bool)}, self.MouseDownOwned, base);
            }, [](ImGuiIO& self) {},
            "Track if button was clicked inside a dear imgui window or over None blocked by a popup. We don't request mouse capture from the application if click started outside ImGui bounds.")
        .def_property("mouse_down_owned_unless_popup_close",    // imgui.h:2133
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<bool>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(bool)});
                return pybind11::array(dtype, {5}, {sizeof(bool)}, self.MouseDownOwnedUnlessPopupClose, base);
            }, [](ImGuiIO& self) {},
            "Track if button was clicked inside a dear imgui window.")
        .def_property("mouse_down_duration",    // imgui.h:2134
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<float>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(float)});
                return pybind11::array(dtype, {5}, {sizeof(float)}, self.MouseDownDuration, base);
            }, [](ImGuiIO& self) {},
            "Duration the mouse button has been down (0.0 == just clicked)")
        .def_property("mouse_down_duration_prev",    // imgui.h:2135
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<float>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(float)});
                return pybind11::array(dtype, {5}, {sizeof(float)}, self.MouseDownDurationPrev, base);
            }, [](ImGuiIO& self) {},
            "Previous time the mouse button has been down")
        .def_property("mouse_drag_max_distance_sqr",    // imgui.h:2137
            [](ImGuiIO &self) -> pybind11::array
            {
                auto dtype = pybind11::dtype(pybind11::format_descriptor<float>::format());
                auto base = pybind11::array(dtype, {5}, {sizeof(float)});
                return pybind11::array(dtype, {5}, {sizeof(float)}, self.MouseDragMaxDistanceSqr, base);
            }, [](ImGuiIO& self) {},
            "Squared maximum distance of how much mouse has traveled from the clicking point (used for moving thresholds)")
        .def_readwrite("pen_pressure", &ImGuiIO::PenPressure, "Touch/Pen pressure (0.0 to 1.0, should be >0.0 only when MouseDown[0] == True). Helper storage currently unused by Dear ImGui.")    // imgui.h:2138
        .def_readwrite("app_focus_lost", &ImGuiIO::AppFocusLost, "Only modify via AddFocusEvent()")    // imgui.h:2139
        .def_readwrite("app_accepting_events", &ImGuiIO::AppAcceptingEvents, "Only modify via SetAppAcceptingEvents()")    // imgui.h:2140
        .def_readwrite("backend_using_legacy_key_arrays", &ImGuiIO::BackendUsingLegacyKeyArrays, "-1: unknown, 0: using AddKeyEvent(), 1: using legacy io.KeysDown[]")    // imgui.h:2141
        .def_readwrite("backend_using_legacy_nav_input_array", &ImGuiIO::BackendUsingLegacyNavInputArray, "0: using AddKeyAnalogEvent(), 1: writing to legacy io.NavInputs[] directly")    // imgui.h:2142
        .def_readwrite("input_queue_surrogate", &ImGuiIO::InputQueueSurrogate, "For AddInputCharacterUTF16()")    // imgui.h:2143
        .def(py::init<>())    // imgui.h:2146
        ;


    auto pyClassImGuiInputTextCallbackData =
        py::class_<ImGuiInputTextCallbackData>    // imgui.h:2162
            (m, "InputTextCallbackData", " Shared state of InputText(), passed as an argument to your callback when a ImGuiInputTextFlags_Callback* flag is used.\n The callback function should return 0 by default.\n Callbacks (follow a flag name and see comments in ImGuiInputTextFlags_ declarations for more details)\n - ImGuiInputTextFlags_CallbackEdit:        Callback on buffer edit (note that InputText() already returns True on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)\n - ImGuiInputTextFlags_CallbackAlways:      Callback on each iteration\n - ImGuiInputTextFlags_CallbackCompletion:  Callback on pressing TAB\n - ImGuiInputTextFlags_CallbackHistory:     Callback on pressing Up/Down arrows\n - ImGuiInputTextFlags_CallbackCharFilter:  Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.\n - ImGuiInputTextFlags_CallbackResize:      Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow.")
        .def_readwrite("event_flag", &ImGuiInputTextCallbackData::EventFlag, "One ImGuiInputTextFlags_Callback*    // Read-only")    // imgui.h:2164
        .def_readwrite("flags", &ImGuiInputTextCallbackData::Flags, "What user passed to InputText()      // Read-only")    // imgui.h:2165
        .def_readwrite("user_data", &ImGuiInputTextCallbackData::UserData, "What user passed to InputText()      // Read-only")    // imgui.h:2166
        .def_readwrite("event_char", &ImGuiInputTextCallbackData::EventChar, "Character input                      // Read-write   // [CharFilter] Replace character with another one, or set to zero to drop. return 1 is equivalent to setting EventChar=0;")    // imgui.h:2171
        .def_readwrite("event_key", &ImGuiInputTextCallbackData::EventKey, "Key pressed (Up/Down/TAB)            // Read-only    // [Completion,History]")    // imgui.h:2172
        .def_readwrite("buf_text_len", &ImGuiInputTextCallbackData::BufTextLen, "Text length (in bytes)               // Read-write   // [Resize,Completion,History,Always] Exclude zero-terminator storage. In C land: == strlen(some_text), in C++ land: string.length()")    // imgui.h:2174
        .def_readwrite("buf_size", &ImGuiInputTextCallbackData::BufSize, "Buffer size (in bytes) = capacity+1  // Read-only    // [Resize,Completion,History,Always] Include zero-terminator storage. In C land == ARRAYSIZE(my_char_array), in C++ land: string.capacity()+1")    // imgui.h:2175
        .def_readwrite("buf_dirty", &ImGuiInputTextCallbackData::BufDirty, "Set if you modify Buf/BufTextLen!    // Write        // [Completion,History,Always]")    // imgui.h:2176
        .def_readwrite("cursor_pos", &ImGuiInputTextCallbackData::CursorPos, "// Read-write   // [Completion,History,Always]")    // imgui.h:2177
        .def_readwrite("selection_start", &ImGuiInputTextCallbackData::SelectionStart, "// Read-write   // [Completion,History,Always] == to SelectionEnd when no selection)")    // imgui.h:2178
        .def_readwrite("selection_end", &ImGuiInputTextCallbackData::SelectionEnd, "// Read-write   // [Completion,History,Always]")    // imgui.h:2179
        .def(py::init<>())    // imgui.h:2183
        .def("delete_chars",    // imgui.h:2184
            &ImGuiInputTextCallbackData::DeleteChars, py::arg("pos"), py::arg("bytes_count"))
        .def("insert_chars",    // imgui.h:2185
            &ImGuiInputTextCallbackData::InsertChars, py::arg("pos"), py::arg("text"), py::arg("text_end") = py::none())
        ;


    auto pyClassImGuiSizeCallbackData =
        py::class_<ImGuiSizeCallbackData>    // imgui.h:2193
            (m, "SizeCallbackData", " Resizing callback data to apply custom constraint. As enabled by SetNextWindowSizeConstraints(). Callback is called during the next Begin().\n NB: For basic min/max size constraint on each axis you don't need to use the callback! The SetNextWindowSizeConstraints() parameters are enough.")
        .def(py::init<>([](
        ImVec2 Pos = ImVec2(), ImVec2 CurrentSize = ImVec2(), ImVec2 DesiredSize = ImVec2())
        {
            auto r = std::make_unique<ImGuiSizeCallbackData>();
            r->Pos = Pos;
            r->CurrentSize = CurrentSize;
            r->DesiredSize = DesiredSize;
            return r;
        })
        , py::arg("pos") = ImVec2(), py::arg("current_size") = ImVec2(), py::arg("desired_size") = ImVec2()
        )
        .def_readwrite("user_data", &ImGuiSizeCallbackData::UserData, "Read-only.   What user passed to SetNextWindowSizeConstraints(). Generally store an integer or float in here (need reinterpret_cast<>).")    // imgui.h:2195
        .def_readwrite("pos", &ImGuiSizeCallbackData::Pos, "Read-only.   Window position, for reference.")    // imgui.h:2196
        .def_readwrite("current_size", &ImGuiSizeCallbackData::CurrentSize, "Read-only.   Current window size.")    // imgui.h:2197
        .def_readwrite("desired_size", &ImGuiSizeCallbackData::DesiredSize, "Read-write.  Desired size, based on user's mouse position. Write to this field to restrain resizing.")    // imgui.h:2198
        ;


    auto pyClassImGuiWindowClass =
        py::class_<ImGuiWindowClass>    // imgui.h:2208
            (m, "WindowClass", " [ALPHA] Rarely used / very advanced uses only. Use with SetNextWindowClass() and DockSpace() functions.\n Important: the content of this class is still highly WIP and likely to change and be refactored\n before we stabilize Docking features. Please be mindful if using this.\n Provide hints:\n - To the platform backend via altered viewport flags (enable/disable OS decoration, OS task bar icons, etc.)\n - To the platform backend for OS level parent/child relationships of viewport.\n - To the docking system for various options and filtering.")
        .def_readwrite("class_id", &ImGuiWindowClass::ClassId, "User data. 0 = Default class (unclassed). Windows of different classes cannot be docked with each others.")    // imgui.h:2210
        .def_readwrite("parent_viewport_id", &ImGuiWindowClass::ParentViewportId, "Hint for the platform backend. -1: use default. 0: request platform backend to not parent the platform. != 0: request platform backend to create a parent<>child relationship between the platform windows. Not conforming backends are free to e.g. parent every viewport to the main viewport or not.")    // imgui.h:2211
        .def_readwrite("viewport_flags_override_set", &ImGuiWindowClass::ViewportFlagsOverrideSet, "Viewport flags to set when a window of this class owns a viewport. This allows you to enforce OS decoration or task bar icon, override the defaults on a per-window basis.")    // imgui.h:2212
        .def_readwrite("viewport_flags_override_clear", &ImGuiWindowClass::ViewportFlagsOverrideClear, "Viewport flags to clear when a window of this class owns a viewport. This allows you to enforce OS decoration or task bar icon, override the defaults on a per-window basis.")    // imgui.h:2213
        .def_readwrite("tab_item_flags_override_set", &ImGuiWindowClass::TabItemFlagsOverrideSet, "[EXPERIMENTAL] TabItem flags to set when a window of this class gets submitted into a dock node tab bar. May use with ImGuiTabItemFlags_Leading or ImGuiTabItemFlags_Trailing.")    // imgui.h:2214
        .def_readwrite("dock_node_flags_override_set", &ImGuiWindowClass::DockNodeFlagsOverrideSet, "[EXPERIMENTAL] Dock node flags to set when a window of this class is hosted by a dock node (it doesn't have to be selected!)")    // imgui.h:2215
        .def_readwrite("docking_always_tab_bar", &ImGuiWindowClass::DockingAlwaysTabBar, "Set to True to enforce single floating windows of this class always having their own docking node (equivalent of setting the global io.ConfigDockingAlwaysTabBar)")    // imgui.h:2216
        .def_readwrite("docking_allow_unclassed", &ImGuiWindowClass::DockingAllowUnclassed, "Set to True to allow windows of this class to be docked/merged with an unclassed window. // FIXME-DOCK: Move to DockNodeFlags override?")    // imgui.h:2217
        .def(py::init<>())    // imgui.h:2219
        ;


    auto pyClassImGuiPayload =
        py::class_<ImGuiPayload>    // imgui.h:2223
            (m, "Payload", "Data payload for Drag and Drop operations: AcceptDragDropPayload(), GetDragDropPayload()")
        .def_readwrite("data", &ImGuiPayload::Data, "Data (copied and owned by dear imgui)")    // imgui.h:2226
        .def_readwrite("data_size", &ImGuiPayload::DataSize, "Data size")    // imgui.h:2227
        .def_readwrite("source_id", &ImGuiPayload::SourceId, "Source item id")    // imgui.h:2230
        .def_readwrite("source_parent_id", &ImGuiPayload::SourceParentId, "Source parent id (if available)")    // imgui.h:2231
        .def_readwrite("data_frame_count", &ImGuiPayload::DataFrameCount, "Data timestamp")    // imgui.h:2232
        .def_readwrite("preview", &ImGuiPayload::Preview, "Set when AcceptDragDropPayload() was called and mouse has been hovering the target item (nb: handle overlapping drag targets)")    // imgui.h:2234
        .def_readwrite("delivery", &ImGuiPayload::Delivery, "Set when AcceptDragDropPayload() was called and mouse button is released over the target item.")    // imgui.h:2235
        .def(py::init<>())    // imgui.h:2237
        ;


    auto pyClassImGuiTableColumnSortSpecs =
        py::class_<ImGuiTableColumnSortSpecs>    // imgui.h:2245
            (m, "TableColumnSortSpecs", "Sorting specification for one column of a table (sizeof == 12 bytes)")
        .def_readwrite("column_user_id", &ImGuiTableColumnSortSpecs::ColumnUserID, "User id of the column (if specified by a TableSetupColumn() call)")    // imgui.h:2247
        .def_readwrite("column_index", &ImGuiTableColumnSortSpecs::ColumnIndex, "Index of the column")    // imgui.h:2248
        .def_readwrite("sort_order", &ImGuiTableColumnSortSpecs::SortOrder, "Index within parent ImGuiTableSortSpecs (always stored in order starting from 0, tables sorted on a single criteria will always have a 0 here)")    // imgui.h:2249
        .def(py::init<>())    // imgui.h:2252
        ;


    auto pyClassImGuiTableSortSpecs =
        py::class_<ImGuiTableSortSpecs>    // imgui.h:2259
            (m, "TableSortSpecs", " Sorting specifications for a table (often handling sort specs for a single column, occasionally more)\n Obtained by calling TableGetSortSpecs().\n When 'SpecsDirty == True' you can sort your data. It will be True with sorting specs have changed since last call, or the first time.\n Make sure to set 'SpecsDirty = False' after sorting, else you may wastefully sort your data every frame!")
        .def_readonly("specs", &ImGuiTableSortSpecs::Specs, "Pointer to sort spec array.")    // imgui.h:2261
        .def_readwrite("specs_count", &ImGuiTableSortSpecs::SpecsCount, "Sort spec count. Most often 1. May be > 1 when ImGuiTableFlags_SortMulti is enabled. May be == 0 when ImGuiTableFlags_SortTristate is enabled.")    // imgui.h:2262
        .def_readwrite("specs_dirty", &ImGuiTableSortSpecs::SpecsDirty, "Set to True when specs have changed since last time! Use this to sort again, then clear the flag.")    // imgui.h:2263
        .def(py::init<>())    // imgui.h:2265
        ;


    auto pyClassImGuiOnceUponAFrame =
        py::class_<ImGuiOnceUponAFrame>    // imgui.h:2282
            (m, "OnceUponAFrame", " Helper: Execute a block of code at maximum once a frame. Convenient if you want to quickly create a UI within deep-nested code that runs multiple times every frame.\n Usage: static ImGuiOnceUponAFrame oaf; if (oaf) ImGui::Text(\"This will be called only once per frame\");")
        .def(py::init<>())    // imgui.h:2284
        .def_readwrite("ref_frame", &ImGuiOnceUponAFrame::RefFrame, "")    // imgui.h:2285
        .def("__bool__",    // imgui.h:2286
            &ImGuiOnceUponAFrame::operator bool)
        ;


    auto pyClassImGuiTextFilter =
        py::class_<ImGuiTextFilter>    // imgui.h:2290
            (m, "TextFilter", "Helper: Parse and apply text filters. In format \"aaaaa[,bbbb][,ccccc]\"");

    { // inner classes & enums of TextFilter
        auto pyClassImGuiTextFilter_ClassImGuiTextRange =
            py::class_<ImGuiTextFilter::ImGuiTextRange>    // imgui.h:2300
                (pyClassImGuiTextFilter, "TextRange", "[Internal]")
            .def_readonly("b", &ImGuiTextFilter::ImGuiTextRange::b, "")    // imgui.h:2302
            .def_readonly("e", &ImGuiTextFilter::ImGuiTextRange::e, "")    // imgui.h:2303
            .def(py::init<>())    // imgui.h:2305
            .def(py::init<const char *, const char *>(),    // imgui.h:2306
                py::arg("_b"), py::arg("_e"))
            ;
    } // end of inner classes & enums of TextFilter

    pyClassImGuiTextFilter
        .def(py::init<const char *>(),    // imgui.h:2292
            py::arg("default_filter") = "")
        .def("draw",    // imgui.h:2293
            &ImGuiTextFilter::Draw,
            py::arg("label") = "Filter (inc,-exc)", py::arg("width") = 0.0f,
            "Helper calling InputText+Build")
        .def("pass_filter",    // imgui.h:2294
            &ImGuiTextFilter::PassFilter, py::arg("text"), py::arg("text_end") = py::none())
        .def("build",    // imgui.h:2295
            &ImGuiTextFilter::Build)
        .def_readwrite("count_grep", &ImGuiTextFilter::CountGrep, "")    // imgui.h:2312
        ;


    auto pyClassImGuiTextBuffer =
        py::class_<ImGuiTextBuffer>    // imgui.h:2317
            (m, "TextBuffer", " Helper: Growable text buffer for logging/accumulating text\n (this could be called 'ImGuiTextBuilder' / 'ImGuiStringBuilder')")
        .def(py::init<>())    // imgui.h:2322
        .def("append",    // imgui.h:2331
            &ImGuiTextBuffer::append, py::arg("str"), py::arg("str_end") = py::none())
        .def("appendf",    // imgui.h:2332
            [](ImGuiTextBuffer & self, const char * fmt)
            {
                auto appendf_adapt_variadic_format = [&self](const char * fmt)
                {
                    self.appendf("%s", fmt);
                };

                appendf_adapt_variadic_format(fmt);
            },     py::arg("fmt"))
        ;


    auto pyClassImGuiStorage =
        py::class_<ImGuiStorage>    // imgui.h:2344
            (m, "Storage", " Helper: Key->Value storage\n Typically you don't have to worry about this since a storage is held within each Window.\n We use it to e.g. store collapse state for a tree (Int 0/1)\n This is optimized for efficient lookup (dichotomy into a contiguous buffer) and rare insertion (typically tied to user interactions aka max once a frame)\n You can use it as custom user storage for temporary values. Declare your own storage if, for example:\n - You want to manipulate the open/close state of a particular sub-tree in your interface (tree node uses Int 0/1 to store their state).\n - You want to store custom debug data easily without adding or editing structures in your code (probably not efficient, but convenient)\n Types are NOT stored, so it is up to you to make sure your Key don't collide with different types.");

    { // inner classes & enums of Storage
        auto pyClassImGuiStorage_ClassImGuiStoragePair =
            py::class_<ImGuiStorage::ImGuiStoragePair>    // imgui.h:2347
                (pyClassImGuiStorage, "StoragePair", "[Internal]")
            .def_readwrite("key", &ImGuiStorage::ImGuiStoragePair::key, "")    // imgui.h:2349
            .def(py::init<ImGuiID, int>(),    // imgui.h:2351
                py::arg("_key"), py::arg("_val_i"))
            .def(py::init<ImGuiID, float>(),    // imgui.h:2352
                py::arg("_key"), py::arg("_val_f"))
            .def(py::init<ImGuiID, void *>(),    // imgui.h:2353
                py::arg("_key"), py::arg("_val_p"))
            ;
    } // end of inner classes & enums of Storage

    pyClassImGuiStorage
        .def(py::init<>()) // implicit default constructor
        .def("get_int",    // imgui.h:2362
            &ImGuiStorage::GetInt, py::arg("key"), py::arg("default_val") = 0)
        .def("set_int",    // imgui.h:2363
            &ImGuiStorage::SetInt, py::arg("key"), py::arg("val"))
        .def("get_bool",    // imgui.h:2364
            &ImGuiStorage::GetBool, py::arg("key"), py::arg("default_val") = false)
        .def("set_bool",    // imgui.h:2365
            &ImGuiStorage::SetBool, py::arg("key"), py::arg("val"))
        .def("get_float",    // imgui.h:2366
            &ImGuiStorage::GetFloat, py::arg("key"), py::arg("default_val") = 0.0f)
        .def("set_float",    // imgui.h:2367
            &ImGuiStorage::SetFloat, py::arg("key"), py::arg("val"))
        .def("get_void_ptr",    // imgui.h:2368
            &ImGuiStorage::GetVoidPtr,
            py::arg("key"),
            "default_val is None",
            pybind11::return_value_policy::reference)
        .def("set_void_ptr",    // imgui.h:2369
            &ImGuiStorage::SetVoidPtr, py::arg("key"), py::arg("val"))
        .def("get_int_ref",    // imgui.h:2375
            &ImGuiStorage::GetIntRef,
            py::arg("key"), py::arg("default_val") = 0,
            pybind11::return_value_policy::reference)
        .def("get_bool_ref",    // imgui.h:2376
            &ImGuiStorage::GetBoolRef,
            py::arg("key"), py::arg("default_val") = false,
            pybind11::return_value_policy::reference)
        .def("get_float_ref",    // imgui.h:2377
            &ImGuiStorage::GetFloatRef,
            py::arg("key"), py::arg("default_val") = 0.0f,
            pybind11::return_value_policy::reference)
        .def("set_all_int",    // imgui.h:2381
            &ImGuiStorage::SetAllInt,
            py::arg("val"),
            "Use on your own storage if you know only integer are being stored (open/close all tree nodes)")
        .def("build_sort_by_key",    // imgui.h:2384
            &ImGuiStorage::BuildSortByKey, "For quicker full rebuild of a storage (instead of an incremental one), you may add all your contents and then sort once.")
        ;


    auto pyClassImGuiListClipper =
        py::class_<ImGuiListClipper>    // imgui.h:2407
            (m, "ListClipper", " Helper: Manually clip large list of items.\n If you have lots evenly spaced items and you have random access to the list, you can perform coarse\n clipping based on visibility to only submit items that are in view.\n The clipper calculates the range of visible items and advance the cursor to compensate for the non-visible items we have skipped.\n (Dear ImGui already clip items based on their bounds but: it needs to first layout the item to do so, and generally\n  fetching/submitting your own data incurs additional cost. Coarse clipping using ImGuiListClipper allows you to easily\n  scale using lists with tens of thousands of items without a problem)\n Usage:\n   ImGuiListClipper clipper;\n   clipper.Begin(1000);         // We have 1000 elements, evenly spaced.\n   while (clipper.Step())\n       for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)\n           ImGui::Text(\"line number %d\", i);\n Generally what happens is:\n - Clipper lets you process the first element (DisplayStart = 0, DisplayEnd = 1) regardless of it being visible or not.\n - User code submit that one element.\n - Clipper can measure the height of the first element\n - Clipper calculate the actual range of elements to display based on the current clipping rectangle, position the cursor before the first visible element.\n - User code submit visible elements.\n - The clipper also handles various subtleties related to keyboard/gamepad navigation, wrapping etc.")
        .def_readwrite("display_start", &ImGuiListClipper::DisplayStart, "First item to display, updated by each call to Step()")    // imgui.h:2409
        .def_readwrite("display_end", &ImGuiListClipper::DisplayEnd, "End of items to display (exclusive)")    // imgui.h:2410
        .def_readwrite("items_count", &ImGuiListClipper::ItemsCount, "[Internal] Number of items")    // imgui.h:2411
        .def_readwrite("items_height", &ImGuiListClipper::ItemsHeight, "[Internal] Height of item after a first step and item submission can calculate it")    // imgui.h:2412
        .def_readwrite("start_pos_y", &ImGuiListClipper::StartPosY, "[Internal] Cursor position at the time of Begin() or after table frozen rows are all processed")    // imgui.h:2413
        .def_readwrite("temp_data", &ImGuiListClipper::TempData, "[Internal] Internal data")    // imgui.h:2414
        .def(py::init<>(),    // imgui.h:2418
            " items_count: Use INT_MAX if you don't know how many items you have (in which case the cursor won't be advanced in the final step)\n items_height: Use -1.0 to be calculated automatically on first step. Otherwise pass in the distance between your items, typically GetTextLineHeightWithSpacing() or GetFrameHeightWithSpacing().")
        .def("begin",    // imgui.h:2420
            &ImGuiListClipper::Begin, py::arg("items_count"), py::arg("items_height") = -1.0f)
        .def("end",    // imgui.h:2421
            &ImGuiListClipper::End, "Automatically called on the last call of Step() that returns False.")
        .def("step",    // imgui.h:2422
            &ImGuiListClipper::Step, "Call until it returns False. The DisplayStart/DisplayEnd fields will be set and you can process/draw those items.")
        .def("force_display_range_by_indices",    // imgui.h:2425
            &ImGuiListClipper::ForceDisplayRangeByIndices,
            py::arg("item_min"), py::arg("item_max"),
            "Call ForceDisplayRangeByIndices() before first call to Step() if you need a range of items to be displayed regardless of visibility.")
        ;


    auto pyClassImColor =
        py::class_<ImColor>    // imgui.h:2458
            (m, "ImColor", " Helper: ImColor() implicitly converts colors to either ImU32 (packed 4x1 byte) or ImVec4 (4x1 float)\n Prefer using IM_COL32() macros if you want a guaranteed compile-time ImU32 for usage with ImDrawList API.\n **Avoid storing ImColor! Store either u32 of ImVec4. This is not a full-featured color class. MAY OBSOLETE.\n **None of the ImGui API are using ImColor directly but you can use it as a convenience to pass colors in either ImU32 or ImVec4 formats. Explicitly cast to ImU32 or ImVec4 if needed.")
        .def_readwrite("value", &ImColor::Value, "")    // imgui.h:2460
        .def(py::init<>())    // imgui.h:2462
        .def(py::init<float, float, float, float>(),    // imgui.h:2463
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a") = 1.0f)
        .def(py::init<const ImVec4 &>(),    // imgui.h:2464
            py::arg("col"))
        .def(py::init<int, int, int, int>(),    // imgui.h:2465
            py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a") = 255)
        .def(py::init<ImU32>(),    // imgui.h:2466
            py::arg("rgba"))
        ;


    auto pyClassImDrawCmd =
        py::class_<ImDrawCmd>    // imgui.h:2507
            (m, "ImDrawCmd", " Typically, 1 command = 1 GPU draw call (unless command is a callback)\n - VtxOffset: When 'io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset' is enabled,\n   this fields allow us to render meshes larger than 64K vertices while keeping 16-bit indices.\n   Backends made for <1.71. will typically ignore the VtxOffset fields.\n - The ClipRect/TextureId/VtxOffset fields must be contiguous as we memcmp() them together (this is asserted for).")
        .def_readwrite("clip_rect", &ImDrawCmd::ClipRect, "4*4  // Clipping rectangle (x1, y1, x2, y2). Subtract ImDrawData->DisplayPos to get clipping rectangle in \"viewport\" coordinates")    // imgui.h:2509
        .def_readwrite("texture_id", &ImDrawCmd::TextureId, "4-8  // User-provided texture ID. Set by user in ImfontAtlas::SetTexID() for fonts or passed to Image*() functions. Ignore if never using images or multiple fonts atlas.")    // imgui.h:2510
        .def_readwrite("vtx_offset", &ImDrawCmd::VtxOffset, "4    // Start offset in vertex buffer. ImGuiBackendFlags_RendererHasVtxOffset: always 0, otherwise may be >0 to support meshes larger than 64K vertices with 16-bit indices.")    // imgui.h:2511
        .def_readwrite("idx_offset", &ImDrawCmd::IdxOffset, "4    // Start offset in index buffer.")    // imgui.h:2512
        .def_readwrite("elem_count", &ImDrawCmd::ElemCount, "4    // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee ImDrawList's vtx_buffer[] array, indices in idx_buffer[].")    // imgui.h:2513
        .def_readwrite("user_callback_data", &ImDrawCmd::UserCallbackData, "4-8  // The draw callback code can access this.")    // imgui.h:2515
        .def(py::init<>(),    // imgui.h:2517
            "Also ensure our padding fields are zeroed")
        ;


    auto pyClassImDrawCmdHeader =
        py::class_<ImDrawCmdHeader>    // imgui.h:2540
            (m, "ImDrawCmdHeader", "[Internal] For use by ImDrawList")
        .def(py::init<>([](
        ImVec4 ClipRect = ImVec4(), ImTextureID TextureId = ImTextureID())
        {
            auto r = std::make_unique<ImDrawCmdHeader>();
            r->ClipRect = ClipRect;
            r->TextureId = TextureId;
            return r;
        })
        , py::arg("clip_rect") = ImVec4(), py::arg("texture_id") = ImTextureID()
        )
        .def_readwrite("clip_rect", &ImDrawCmdHeader::ClipRect, "")    // imgui.h:2542
        .def_readwrite("texture_id", &ImDrawCmdHeader::TextureId, "")    // imgui.h:2543
        .def_readwrite("vtx_offset", &ImDrawCmdHeader::VtxOffset, "")    // imgui.h:2544
        ;


    auto pyClassImDrawChannel =
        py::class_<ImDrawChannel>    // imgui.h:2548
            (m, "ImDrawChannel", "[Internal] For use by ImDrawListSplitter")
        .def(py::init<>()) // implicit default constructor
        ;


    auto pyClassImDrawListSplitter =
        py::class_<ImDrawListSplitter>    // imgui.h:2557
            (m, "ImDrawListSplitter", " Split/Merge functions are used to split the draw list into different layers which can be drawn into out of order.\n This is used by the Columns/Tables API, so items of each column can be batched together in a same draw call.")
        .def_readwrite("_current", &ImDrawListSplitter::_Current, "Current channel number (0)")    // imgui.h:2559
        .def_readwrite("_count", &ImDrawListSplitter::_Count, "Number of active channels (1+)")    // imgui.h:2560
        .def(py::init<>())    // imgui.h:2563
        .def("clear_free_memory",    // imgui.h:2566
            &ImDrawListSplitter::ClearFreeMemory)
        .def("split",    // imgui.h:2567
            &ImDrawListSplitter::Split, py::arg("draw_list"), py::arg("count"))
        .def("merge",    // imgui.h:2568
            &ImDrawListSplitter::Merge, py::arg("draw_list"))
        .def("set_current_channel",    // imgui.h:2569
            &ImDrawListSplitter::SetCurrentChannel, py::arg("draw_list"), py::arg("channel_idx"))
        ;


    py::enum_<ImDrawFlags_>(m, "ImDrawFlags_", py::arithmetic(), " Flags for ImDrawList functions\n (Legacy: bit 0 must always correspond to ImDrawFlags_Closed to be backward compatible with old API using a bool. Bits 1..3 must be unused)")    // imgui.h:2574
        .value("none", ImDrawFlags_None, "")
        .value("closed", ImDrawFlags_Closed, "PathStroke(), AddPolyline(): specify that shape should be closed (Important: this is always == 1 for legacy reason)")
        .value("round_corners_top_left", ImDrawFlags_RoundCornersTopLeft, "AddRect(), AddRectFilled(), PathRect(): enable rounding top-left corner only (when rounding > 0.0, we default to all corners). Was 0x01.")
        .value("round_corners_top_right", ImDrawFlags_RoundCornersTopRight, "AddRect(), AddRectFilled(), PathRect(): enable rounding top-right corner only (when rounding > 0.0, we default to all corners). Was 0x02.")
        .value("round_corners_bottom_left", ImDrawFlags_RoundCornersBottomLeft, "AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-left corner only (when rounding > 0.0, we default to all corners). Was 0x04.")
        .value("round_corners_bottom_right", ImDrawFlags_RoundCornersBottomRight, "AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-right corner only (when rounding > 0.0, we default to all corners). Wax 0x08.")
        .value("round_corners_none", ImDrawFlags_RoundCornersNone, "AddRect(), AddRectFilled(), PathRect(): disable rounding on all corners (when rounding > 0.0). This is NOT zero, NOT an implicit flag!")
        .value("round_corners_top", ImDrawFlags_RoundCornersTop, "")
        .value("round_corners_bottom", ImDrawFlags_RoundCornersBottom, "")
        .value("round_corners_left", ImDrawFlags_RoundCornersLeft, "")
        .value("round_corners_right", ImDrawFlags_RoundCornersRight, "")
        .value("round_corners_all", ImDrawFlags_RoundCornersAll, "")
        .value("round_corners_default_", ImDrawFlags_RoundCornersDefault_, "Default to ALL corners if none of the _RoundCornersXX flags are specified.")
        .value("round_corners_mask_", ImDrawFlags_RoundCornersMask_, "");


    py::enum_<ImDrawListFlags_>(m, "ImDrawListFlags_", py::arithmetic(), " Flags for ImDrawList instance. Those are set automatically by ImGui:: functions from ImGuiIO settings, and generally not manipulated directly.\n It is however possible to temporarily alter flags between calls to ImDrawList:: functions.")    // imgui.h:2594
        .value("none", ImDrawListFlags_None, "")
        .value("anti_aliased_lines", ImDrawListFlags_AntiAliasedLines, "Enable anti-aliased lines/borders (*2 the number of triangles for 1.0 wide line or lines thin enough to be drawn using textures, otherwise *3 the number of triangles)")
        .value("anti_aliased_lines_use_tex", ImDrawListFlags_AntiAliasedLinesUseTex, "Enable anti-aliased lines/borders using textures when possible. Require backend to render with bilinear filtering (NOT point/nearest filtering).")
        .value("anti_aliased_fill", ImDrawListFlags_AntiAliasedFill, "Enable anti-aliased edge around filled shapes (rounded rectangles, circles).")
        .value("allow_vtx_offset", ImDrawListFlags_AllowVtxOffset, "Can emit 'VtxOffset > 0' to allow large meshes. Set when 'ImGuiBackendFlags_RendererHasVtxOffset' is enabled.");


    auto pyClassImDrawList =
        py::class_<ImDrawList>    // imgui.h:2612
            (m, "ImDrawList", " Draw command list\n This is the low-level list of polygons that ImGui:: functions are filling. At the end of the frame,\n all command lists are passed to your ImGuiIO::RenderDrawListFn function for rendering.\n Each dear imgui window contains its own ImDrawList. You can use ImGui::GetWindowDrawList() to\n access the current window draw list and draw custom primitives.\n You can interleave normal ImGui:: calls and adding primitives to the current draw list.\n In single viewport mode, top-left is == GetMainViewport()->Pos (generally 0,0), bottom-right is == GetMainViewport()->Pos+Size (generally io.DisplaySize).\n You are totally free to apply whatever transformation matrix to want to the data (depending on the use of the transformation you may want to apply it to ClipRect as well!)\n Important: Primitives are always added to the list and not culled (culling is done at higher-level by ImGui:: functions), if you use this API a lot consider coarse culling your drawn objects.")
        .def_readwrite("flags", &ImDrawList::Flags, "Flags, you may poke into these to adjust anti-aliasing settings per-primitive.")    // imgui.h:2618
        .def_readwrite("_vtx_current_idx", &ImDrawList::_VtxCurrentIdx, "[Internal] generally == VtxBuffer.Size unless we are past 64K vertices, in which case this gets reset to 0.")    // imgui.h:2621
        .def_readwrite("_data", &ImDrawList::_Data, "Pointer to shared draw data (you can use ImGui::GetDrawListSharedData() to get the one from current ImGui context)")    // imgui.h:2622
        .def_readonly("_owner_name", &ImDrawList::_OwnerName, "Pointer to owner window's name for debugging")    // imgui.h:2623
        .def_readwrite("_vtx_write_ptr", &ImDrawList::_VtxWritePtr, "[Internal] point within VtxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)")    // imgui.h:2624
        .def_readwrite("_idx_write_ptr", &ImDrawList::_IdxWritePtr, "[Internal] point within IdxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)")    // imgui.h:2625
        .def_readwrite("_cmd_header", &ImDrawList::_CmdHeader, "[Internal] template of active commands. Fields should match those of CmdBuffer.back().")    // imgui.h:2629
        .def_readwrite("_splitter", &ImDrawList::_Splitter, "[Internal] for channels api (note: prefer using your own persistent instance of ImDrawListSplitter!)")    // imgui.h:2630
        .def_readwrite("_fringe_scale", &ImDrawList::_FringeScale, "[Internal] anti-alias fringe is scaled by this value, this helps to keep things sharp while zooming at vertex buffer content")    // imgui.h:2631
        .def(py::init<ImDrawListSharedData *>(),    // imgui.h:2634
            py::arg("shared_data"),
            "If you want to create ImDrawList instances, pass them ImGui::GetDrawListSharedData() or create and use your own ImDrawListSharedData (so you can use ImDrawList without ImGui)")
        .def("push_clip_rect",    // imgui.h:2637
            &ImDrawList::PushClipRect,
            py::arg("clip_rect_min"), py::arg("clip_rect_max"), py::arg("intersect_with_current_clip_rect") = false,
            "Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)")
        .def("push_clip_rect_full_screen",    // imgui.h:2638
            &ImDrawList::PushClipRectFullScreen)
        .def("pop_clip_rect",    // imgui.h:2639
            &ImDrawList::PopClipRect)
        .def("push_texture_id",    // imgui.h:2640
            &ImDrawList::PushTextureID, py::arg("texture_id"))
        .def("pop_texture_id",    // imgui.h:2641
            &ImDrawList::PopTextureID)
        .def("add_line",    // imgui.h:2652
            &ImDrawList::AddLine, py::arg("p1"), py::arg("p2"), py::arg("col"), py::arg("thickness") = 1.0f)
        .def("add_rect",    // imgui.h:2653
            &ImDrawList::AddRect,
            py::arg("p_min"), py::arg("p_max"), py::arg("col"), py::arg("rounding") = 0.0f, py::arg("flags") = 0, py::arg("thickness") = 1.0f,
            "a: upper-left, b: lower-right (== upper-left + size)")
        .def("add_rect_filled",    // imgui.h:2654
            &ImDrawList::AddRectFilled,
            py::arg("p_min"), py::arg("p_max"), py::arg("col"), py::arg("rounding") = 0.0f, py::arg("flags") = 0,
            "a: upper-left, b: lower-right (== upper-left + size)")
        .def("add_rect_filled_multi_color",    // imgui.h:2655
            &ImDrawList::AddRectFilledMultiColor, py::arg("p_min"), py::arg("p_max"), py::arg("col_upr_left"), py::arg("col_upr_right"), py::arg("col_bot_right"), py::arg("col_bot_left"))
        .def("add_quad",    // imgui.h:2656
            &ImDrawList::AddQuad, py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"), py::arg("col"), py::arg("thickness") = 1.0f)
        .def("add_quad_filled",    // imgui.h:2657
            &ImDrawList::AddQuadFilled, py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"), py::arg("col"))
        .def("add_triangle",    // imgui.h:2658
            &ImDrawList::AddTriangle, py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("col"), py::arg("thickness") = 1.0f)
        .def("add_triangle_filled",    // imgui.h:2659
            &ImDrawList::AddTriangleFilled, py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("col"))
        .def("add_circle",    // imgui.h:2660
            &ImDrawList::AddCircle, py::arg("center"), py::arg("radius"), py::arg("col"), py::arg("num_segments") = 0, py::arg("thickness") = 1.0f)
        .def("add_circle_filled",    // imgui.h:2661
            &ImDrawList::AddCircleFilled, py::arg("center"), py::arg("radius"), py::arg("col"), py::arg("num_segments") = 0)
        .def("add_ngon",    // imgui.h:2662
            &ImDrawList::AddNgon, py::arg("center"), py::arg("radius"), py::arg("col"), py::arg("num_segments"), py::arg("thickness") = 1.0f)
        .def("add_ngon_filled",    // imgui.h:2663
            &ImDrawList::AddNgonFilled, py::arg("center"), py::arg("radius"), py::arg("col"), py::arg("num_segments"))
        .def("add_text",    // imgui.h:2664
            py::overload_cast<const ImVec2 &, ImU32, const char *, const char *>(&ImDrawList::AddText), py::arg("pos"), py::arg("col"), py::arg("text_begin"), py::arg("text_end") = py::none())
        .def("add_text",    // imgui.h:2665
            py::overload_cast<const ImFont *, float, const ImVec2 &, ImU32, const char *, const char *, float, const ImVec4 *>(&ImDrawList::AddText), py::arg("font"), py::arg("font_size"), py::arg("pos"), py::arg("col"), py::arg("text_begin"), py::arg("text_end") = py::none(), py::arg("wrap_width") = 0.0f, py::arg("cpu_fine_clip_rect") = py::none())
        .def("add_polyline",    // imgui.h:2666
            &ImDrawList::AddPolyline, py::arg("points"), py::arg("num_points"), py::arg("col"), py::arg("flags"), py::arg("thickness"))
        .def("add_convex_poly_filled",    // imgui.h:2667
            &ImDrawList::AddConvexPolyFilled, py::arg("points"), py::arg("num_points"), py::arg("col"))
        .def("add_bezier_cubic",    // imgui.h:2668
            &ImDrawList::AddBezierCubic,
            py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"), py::arg("col"), py::arg("thickness"), py::arg("num_segments") = 0,
            "Cubic Bezier (4 control points)")
        .def("add_bezier_quadratic",    // imgui.h:2669
            &ImDrawList::AddBezierQuadratic,
            py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("col"), py::arg("thickness"), py::arg("num_segments") = 0,
            "Quadratic Bezier (3 control points)")
        .def("add_image",    // imgui.h:2675
            &ImDrawList::AddImage, py::arg("user_texture_id"), py::arg("p_min"), py::arg("p_max"), py::arg("uv_min") = ImVec2(0, 0), py::arg("uv_max") = ImVec2(1, 1), py::arg("col") = IM_COL32_WHITE)
        .def("add_image_quad",    // imgui.h:2676
            &ImDrawList::AddImageQuad, py::arg("user_texture_id"), py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"), py::arg("uv1") = ImVec2(0, 0), py::arg("uv2") = ImVec2(1, 0), py::arg("uv3") = ImVec2(1, 1), py::arg("uv4") = ImVec2(0, 1), py::arg("col") = IM_COL32_WHITE)
        .def("add_image_rounded",    // imgui.h:2677
            &ImDrawList::AddImageRounded, py::arg("user_texture_id"), py::arg("p_min"), py::arg("p_max"), py::arg("uv_min"), py::arg("uv_max"), py::arg("col"), py::arg("rounding"), py::arg("flags") = 0)
        .def("path_arc_to",    // imgui.h:2686
            &ImDrawList::PathArcTo, py::arg("center"), py::arg("radius"), py::arg("a_min"), py::arg("a_max"), py::arg("num_segments") = 0)
        .def("path_arc_to_fast",    // imgui.h:2687
            &ImDrawList::PathArcToFast,
            py::arg("center"), py::arg("radius"), py::arg("a_min_of_12"), py::arg("a_max_of_12"),
            "Use precomputed angles for a 12 steps circle")
        .def("path_bezier_cubic_curve_to",    // imgui.h:2688
            &ImDrawList::PathBezierCubicCurveTo,
            py::arg("p2"), py::arg("p3"), py::arg("p4"), py::arg("num_segments") = 0,
            "Cubic Bezier (4 control points)")
        .def("path_bezier_quadratic_curve_to",    // imgui.h:2689
            &ImDrawList::PathBezierQuadraticCurveTo,
            py::arg("p2"), py::arg("p3"), py::arg("num_segments") = 0,
            "Quadratic Bezier (3 control points)")
        .def("path_rect",    // imgui.h:2690
            &ImDrawList::PathRect, py::arg("rect_min"), py::arg("rect_max"), py::arg("rounding") = 0.0f, py::arg("flags") = 0)
        .def("add_callback",    // imgui.h:2693
            &ImDrawList::AddCallback,
            py::arg("callback"), py::arg("callback_data"),
            "Your rendering function must check for 'UserCallback' in ImDrawCmd and call the function instead of rendering triangles.")
        .def("add_draw_cmd",    // imgui.h:2694
            &ImDrawList::AddDrawCmd, "This is useful if you need to forcefully create a new draw call (to allow for dependent rendering / blending). Otherwise primitives are merged into the same draw-call as much as possible")
        .def("clone_output",    // imgui.h:2695
            &ImDrawList::CloneOutput,
            "Create a clone of the CmdBuffer/IdxBuffer/VtxBuffer.",
            pybind11::return_value_policy::reference)
        .def("prim_reserve",    // imgui.h:2710
            &ImDrawList::PrimReserve, py::arg("idx_count"), py::arg("vtx_count"))
        .def("prim_unreserve",    // imgui.h:2711
            &ImDrawList::PrimUnreserve, py::arg("idx_count"), py::arg("vtx_count"))
        .def("prim_rect",    // imgui.h:2712
            &ImDrawList::PrimRect,
            py::arg("a"), py::arg("b"), py::arg("col"),
            "Axis aligned rectangle (composed of two triangles)")
        .def("prim_rect_uv",    // imgui.h:2713
            &ImDrawList::PrimRectUV, py::arg("a"), py::arg("b"), py::arg("uv_a"), py::arg("uv_b"), py::arg("col"))
        .def("prim_quad_uv",    // imgui.h:2714
            &ImDrawList::PrimQuadUV, py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("uv_a"), py::arg("uv_b"), py::arg("uv_c"), py::arg("uv_d"), py::arg("col"))
        .def("_reset_for_new_frame",    // imgui.h:2725
            &ImDrawList::_ResetForNewFrame)
        .def("_clear_free_memory",    // imgui.h:2726
            &ImDrawList::_ClearFreeMemory)
        .def("_pop_unused_draw_cmd",    // imgui.h:2727
            &ImDrawList::_PopUnusedDrawCmd)
        .def("_try_merge_draw_cmds",    // imgui.h:2728
            &ImDrawList::_TryMergeDrawCmds)
        .def("_on_changed_clip_rect",    // imgui.h:2729
            &ImDrawList::_OnChangedClipRect)
        .def("_on_changed_texture_id",    // imgui.h:2730
            &ImDrawList::_OnChangedTextureID)
        .def("_on_changed_vtx_offset",    // imgui.h:2731
            &ImDrawList::_OnChangedVtxOffset)
        .def("_calc_circle_auto_segment_count",    // imgui.h:2732
            &ImDrawList::_CalcCircleAutoSegmentCount, py::arg("radius"))
        .def("_path_arc_to_fast_ex",    // imgui.h:2733
            &ImDrawList::_PathArcToFastEx, py::arg("center"), py::arg("radius"), py::arg("a_min_sample"), py::arg("a_max_sample"), py::arg("a_step"))
        .def("_path_arc_to_n",    // imgui.h:2734
            &ImDrawList::_PathArcToN, py::arg("center"), py::arg("radius"), py::arg("a_min"), py::arg("a_max"), py::arg("num_segments"))
        ;


    auto pyClassImDrawData =
        py::class_<ImDrawData>    // imgui.h:2740
            (m, "ImDrawData", " All draw data to render a Dear ImGui frame\n (NB: the style and the naming convention here is a little inconsistent, we currently preserve them for backward compatibility purpose,\n as this is one of the oldest structure exposed by the library! Basically, ImDrawList == CmdList)")
        .def_readwrite("valid", &ImDrawData::Valid, "Only valid after Render() is called and before the next NewFrame() is called.")    // imgui.h:2742
        .def_readwrite("cmd_lists_count", &ImDrawData::CmdListsCount, "Number of ImDrawList* to render")    // imgui.h:2743
        .def_readwrite("total_idx_count", &ImDrawData::TotalIdxCount, "For convenience, sum of all ImDrawList's IdxBuffer.Size")    // imgui.h:2744
        .def_readwrite("total_vtx_count", &ImDrawData::TotalVtxCount, "For convenience, sum of all ImDrawList's VtxBuffer.Size")    // imgui.h:2745
        .def_readwrite("display_pos", &ImDrawData::DisplayPos, "Top-left position of the viewport to render (== top-left of the orthogonal projection matrix to use) (== GetMainViewport()->Pos for the main viewport, == (0.0) in most single-viewport applications)")    // imgui.h:2747
        .def_readwrite("display_size", &ImDrawData::DisplaySize, "Size of the viewport to render (== GetMainViewport()->Size for the main viewport, == io.DisplaySize in most single-viewport applications)")    // imgui.h:2748
        .def_readwrite("framebuffer_scale", &ImDrawData::FramebufferScale, "Amount of pixels for each unit of DisplaySize. Based on io.DisplayFramebufferScale. Generally (1,1) on normal display, (2,2) on OSX with Retina display.")    // imgui.h:2749
        .def_readwrite("owner_viewport", &ImDrawData::OwnerViewport, "Viewport carrying the ImDrawData instance, might be of use to the renderer (generally not).")    // imgui.h:2750
        .def(py::init<>(),    // imgui.h:2753
            "Functions")
        .def("de_index_all_buffers",    // imgui.h:2755
            &ImDrawData::DeIndexAllBuffers, "Helper to convert all buffers from indexed to non-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!")
        .def("scale_clip_rects",    // imgui.h:2756
            &ImDrawData::ScaleClipRects,
            py::arg("fb_scale"),
            "Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than Dear ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.")
        ;


    auto pyClassImFontConfig =
        py::class_<ImFontConfig>    // imgui.h:2763
            (m, "ImFontConfig", "")
        .def_readwrite("font_data", &ImFontConfig::FontData, "// TTF/OTF data")    // imgui.h:2765
        .def_readwrite("font_data_size", &ImFontConfig::FontDataSize, "// TTF/OTF data size")    // imgui.h:2766
        .def_readwrite("font_data_owned_by_atlas", &ImFontConfig::FontDataOwnedByAtlas, "True     // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).")    // imgui.h:2767
        .def_readwrite("font_no", &ImFontConfig::FontNo, "0        // Index of font within TTF/OTF file")    // imgui.h:2768
        .def_readwrite("size_pixels", &ImFontConfig::SizePixels, "// Size in pixels for rasterizer (more or less maps to the resulting font height).")    // imgui.h:2769
        .def_readwrite("oversample_h", &ImFontConfig::OversampleH, "3        // Rasterize at higher quality for sub-pixel positioning. Note the difference between 2 and 3 is minimal so you can reduce this to 2 to save memory. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.")    // imgui.h:2770
        .def_readwrite("oversample_v", &ImFontConfig::OversampleV, "1        // Rasterize at higher quality for sub-pixel positioning. This is not really useful as we don't use sub-pixel positions on the Y axis.")    // imgui.h:2771
        .def_readwrite("pixel_snap_h", &ImFontConfig::PixelSnapH, "False    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.")    // imgui.h:2772
        .def_readwrite("glyph_extra_spacing", &ImFontConfig::GlyphExtraSpacing, "0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.")    // imgui.h:2773
        .def_readwrite("glyph_offset", &ImFontConfig::GlyphOffset, "0, 0     // Offset all glyphs from this font input.")    // imgui.h:2774
        .def_readwrite("glyph_min_advance_x", &ImFontConfig::GlyphMinAdvanceX, "0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font")    // imgui.h:2776
        .def_readwrite("glyph_max_advance_x", &ImFontConfig::GlyphMaxAdvanceX, "FLT_MAX  // Maximum AdvanceX for glyphs")    // imgui.h:2777
        .def_readwrite("merge_mode", &ImFontConfig::MergeMode, "False    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.")    // imgui.h:2778
        .def_readwrite("font_builder_flags", &ImFontConfig::FontBuilderFlags, "0        // Settings for custom font builder. THIS IS BUILDER IMPLEMENTATION DEPENDENT. Leave as zero if unsure.")    // imgui.h:2779
        .def_readwrite("rasterizer_multiply", &ImFontConfig::RasterizerMultiply, "1.0     // Brighten (>1.0) or darken (<1.0) font output. Brightening small fonts may be a good workaround to make them more readable.")    // imgui.h:2780
        .def_readwrite("ellipsis_char", &ImFontConfig::EllipsisChar, "-1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.")    // imgui.h:2781
        .def_readwrite("dst_font", &ImFontConfig::DstFont, "")    // imgui.h:2785
        .def(py::init<>())    // imgui.h:2787
        ;


    auto pyClassImFontGlyph =
        py::class_<ImFontGlyph>    // imgui.h:2792
            (m, "ImFontGlyph", " Hold rendering data for one glyph.\n (Note: some language parsers may fail to convert the 31+1 bitfield members, in this case maybe drop store a single u32 or we can rework this)")
        .def(py::init<>([](
        float AdvanceX = float(), float X0 = float(), float Y0 = float(), float X1 = float(), float Y1 = float(), float U0 = float(), float V0 = float(), float U1 = float(), float V1 = float())
        {
            auto r = std::make_unique<ImFontGlyph>();
            r->AdvanceX = AdvanceX;
            r->X0 = X0;
            r->Y0 = Y0;
            r->X1 = X1;
            r->Y1 = Y1;
            r->U0 = U0;
            r->V0 = V0;
            r->U1 = U1;
            r->V1 = V1;
            return r;
        })
        , py::arg("advance_x") = float(), py::arg("x0") = float(), py::arg("y0") = float(), py::arg("x1") = float(), py::arg("y1") = float(), py::arg("u0") = float(), py::arg("v0") = float(), py::arg("u1") = float(), py::arg("v1") = float()
        )
        .def_readwrite("advance_x", &ImFontGlyph::AdvanceX, "Distance to next character (= data from font + ImFontConfig::GlyphExtraSpacing.x baked in)")    // imgui.h:2797
        .def_readwrite("x0", &ImFontGlyph::X0, "Glyph corners")    // imgui.h:2798
        .def_readwrite("y0", &ImFontGlyph::Y0, "Glyph corners")    // imgui.h:2798
        .def_readwrite("x1", &ImFontGlyph::X1, "Glyph corners")    // imgui.h:2798
        .def_readwrite("y1", &ImFontGlyph::Y1, "Glyph corners")    // imgui.h:2798
        .def_readwrite("u0", &ImFontGlyph::U0, "Texture coordinates")    // imgui.h:2799
        .def_readwrite("v0", &ImFontGlyph::V0, "Texture coordinates")    // imgui.h:2799
        .def_readwrite("u1", &ImFontGlyph::U1, "Texture coordinates")    // imgui.h:2799
        .def_readwrite("v1", &ImFontGlyph::V1, "Texture coordinates")    // imgui.h:2799
        ;


    auto pyClassImFontGlyphRangesBuilder =
        py::class_<ImFontGlyphRangesBuilder>    // imgui.h:2804
            (m, "ImFontGlyphRangesBuilder", " Helper to build glyph ranges from text/string data. Feed your application strings/characters to it then call BuildRanges().\n This is essentially a tightly packed of vector of 64k booleans = 8KB storage.")
        .def(py::init<>())    // imgui.h:2808
        .def("add_text",    // imgui.h:2813
            &ImFontGlyphRangesBuilder::AddText,
            py::arg("text"), py::arg("text_end") = py::none(),
            "Add string (each character of the UTF-8 string are added)")
        .def("add_ranges",    // imgui.h:2814
            &ImFontGlyphRangesBuilder::AddRanges,
            py::arg("ranges"),
            "Add ranges, e.g. builder.AddRanges(ImFontAtlas::GetGlyphRangesDefault()) to force add all of ASCII/Latin+Ext")
        .def("build_ranges",    // imgui.h:2815
            &ImFontGlyphRangesBuilder::BuildRanges,
            py::arg("out_ranges"),
            "Output new ranges")
        ;


    auto pyClassImFontAtlasCustomRect =
        py::class_<ImFontAtlasCustomRect>    // imgui.h:2819
            (m, "ImFontAtlasCustomRect", "See ImFontAtlas::AddCustomRectXXX functions.")
        .def_readwrite("width", &ImFontAtlasCustomRect::Width, "Input    // Desired rectangle dimension")    // imgui.h:2821
        .def_readwrite("height", &ImFontAtlasCustomRect::Height, "Input    // Desired rectangle dimension")    // imgui.h:2821
        .def_readwrite("x", &ImFontAtlasCustomRect::X, "Output   // Packed position in Atlas")    // imgui.h:2822
        .def_readwrite("y", &ImFontAtlasCustomRect::Y, "Output   // Packed position in Atlas")    // imgui.h:2822
        .def_readwrite("glyph_id", &ImFontAtlasCustomRect::GlyphID, "Input    // For custom font glyphs only (ID < 0x110000)")    // imgui.h:2823
        .def_readwrite("glyph_advance_x", &ImFontAtlasCustomRect::GlyphAdvanceX, "Input    // For custom font glyphs only: glyph xadvance")    // imgui.h:2824
        .def_readwrite("glyph_offset", &ImFontAtlasCustomRect::GlyphOffset, "Input    // For custom font glyphs only: glyph display offset")    // imgui.h:2825
        .def_readwrite("font", &ImFontAtlasCustomRect::Font, "Input    // For custom font glyphs only: target font")    // imgui.h:2826
        .def(py::init<>())    // imgui.h:2827
        ;


    py::enum_<ImFontAtlasFlags_>(m, "ImFontAtlasFlags_", py::arithmetic(), "Flags for ImFontAtlas build")    // imgui.h:2832
        .value("none", ImFontAtlasFlags_None, "")
        .value("no_power_of_two_height", ImFontAtlasFlags_NoPowerOfTwoHeight, "Don't round the height to next power of two")
        .value("no_mouse_cursors", ImFontAtlasFlags_NoMouseCursors, "Don't build software mouse cursors into the atlas (save a little texture memory)")
        .value("no_baked_lines", ImFontAtlasFlags_NoBakedLines, "Don't build thick line textures into the atlas (save a little texture memory, allow support for point/nearest filtering). The AntiAliasedLinesUseTex features uses them, otherwise they will be rendered using polygons (more expensive for CPU/GPU).");


    auto pyClassImFontAtlas =
        py::class_<ImFontAtlas>    // imgui.h:2857
            (m, "ImFontAtlas", " Load and rasterize multiple TTF/OTF fonts into a same texture. The font atlas will build a single texture holding:\n  - One or more fonts.\n  - Custom graphics data needed to render the shapes needed by Dear ImGui.\n  - Mouse cursor shapes for software cursor rendering (unless setting 'Flags |= ImFontAtlasFlags_NoMouseCursors' in the font atlas).\n It is the user-code responsibility to setup/build the atlas, then upload the pixel data into a texture accessible by your graphics api.\n  - Optionally, call any of the AddFont*** functions. If you don't call any, the default font embedded in the code will be loaded for you.\n  - Call GetTexDataAsAlpha8() or GetTexDataAsRGBA32() to build and retrieve pixels data.\n  - Upload the pixels data into a texture within your graphics system (see imgui_impl_xxxx.cpp examples)\n  - Call SetTexID(my_tex_id); and pass the pointer/identifier to your texture in a format natural to your graphics API.\n    This value will be passed back to you during rendering to identify the texture. Read FAQ entry about ImTextureID for more details.\n Common pitfalls:\n - If you pass a 'glyph_ranges' array to AddFont*** functions, you need to make sure that your array persist up until the\n   atlas is build (when calling GetTexData*** or Build()). We only copy the pointer, not the data.\n - Important: By default, AddFontFromMemoryTTF() takes ownership of the data. Even though we are not writing to it, we will free the pointer on destruction.\n   You can set font_cfg->FontDataOwnedByAtlas=False to keep ownership of your data and it won't be freed,\n - Even though many functions are suffixed with \"TTF\", OTF data is supported just as well.\n - This is an old API and it is currently awkward for those and various other reasons! We will address them in the future!")
        .def(py::init<>())    // imgui.h:2859
        .def("add_font",    // imgui.h:2861
            &ImFontAtlas::AddFont,
            py::arg("font_cfg"),
            pybind11::return_value_policy::reference)
        .def("add_font_default",    // imgui.h:2862
            &ImFontAtlas::AddFontDefault,
            py::arg("font_cfg") = py::none(),
            pybind11::return_value_policy::reference)
        .def("add_font_from_memory_ttf",    // imgui.h:2864
            &ImFontAtlas::AddFontFromMemoryTTF,
            py::arg("font_data"), py::arg("font_size"), py::arg("size_pixels"), py::arg("font_cfg") = py::none(), py::arg("glyph_ranges") = py::none(),
            "Note: Transfer ownership of 'ttf_data' to ImFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=False to keep ownership of your data and it won't be freed.",
            pybind11::return_value_policy::reference)
        .def("add_font_from_memory_compressed_ttf",    // imgui.h:2865
            &ImFontAtlas::AddFontFromMemoryCompressedTTF,
            py::arg("compressed_font_data"), py::arg("compressed_font_size"), py::arg("size_pixels"), py::arg("font_cfg") = py::none(), py::arg("glyph_ranges") = py::none(),
            "'compressed_font_data' still owned by caller. Compress with binary_to_compressed_c.cpp.",
            pybind11::return_value_policy::reference)
        .def("add_font_from_memory_compressed_base85_ttf",    // imgui.h:2866
            &ImFontAtlas::AddFontFromMemoryCompressedBase85TTF,
            py::arg("compressed_font_data_base85"), py::arg("size_pixels"), py::arg("font_cfg") = py::none(), py::arg("glyph_ranges") = py::none(),
            "'compressed_font_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 parameter.",
            pybind11::return_value_policy::reference)
        .def("clear_input_data",    // imgui.h:2867
            &ImFontAtlas::ClearInputData, "Clear input data (all ImFontConfig structures including sizes, TTF data, glyph ranges, etc.) = all the data used to build the texture and fonts.")
        .def("clear_tex_data",    // imgui.h:2868
            &ImFontAtlas::ClearTexData, "Clear output texture data (CPU side). Saves RAM once the texture has been copied to graphics memory.")
        .def("clear_fonts",    // imgui.h:2869
            &ImFontAtlas::ClearFonts, "Clear output font data (glyphs storage, UV coordinates).")
        .def("clear",    // imgui.h:2870
            &ImFontAtlas::Clear, "Clear all input and output.")
        .def("build",    // imgui.h:2877
            &ImFontAtlas::Build, "Build pixels data. This is called automatically for you by the GetTexData*** functions.")
        .def("get_glyph_ranges_default",    // imgui.h:2890
            &ImFontAtlas::GetGlyphRangesDefault,
            "Basic Latin, Extended Latin",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_greek",    // imgui.h:2891
            &ImFontAtlas::GetGlyphRangesGreek,
            "Default + Greek and Coptic",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_korean",    // imgui.h:2892
            &ImFontAtlas::GetGlyphRangesKorean,
            "Default + Korean characters",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_japanese",    // imgui.h:2893
            &ImFontAtlas::GetGlyphRangesJapanese,
            "Default + Hiragana, Katakana, Half-Width, Selection of 2999 Ideographs",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_chinese_full",    // imgui.h:2894
            &ImFontAtlas::GetGlyphRangesChineseFull,
            "Default + Half-Width + Japanese Hiragana/Katakana + full set of about 21000 CJK Unified Ideographs",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_chinese_simplified_common",    // imgui.h:2895
            &ImFontAtlas::GetGlyphRangesChineseSimplifiedCommon,
            "Default + Half-Width + Japanese Hiragana/Katakana + set of 2500 CJK Unified Ideographs for common simplified Chinese",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_cyrillic",    // imgui.h:2896
            &ImFontAtlas::GetGlyphRangesCyrillic,
            "Default + about 400 Cyrillic characters",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_thai",    // imgui.h:2897
            &ImFontAtlas::GetGlyphRangesThai,
            "Default + Thai characters",
            pybind11::return_value_policy::reference)
        .def("get_glyph_ranges_vietnamese",    // imgui.h:2898
            &ImFontAtlas::GetGlyphRangesVietnamese,
            "Default + Vietnamese characters",
            pybind11::return_value_policy::reference)
        .def("add_custom_rect_regular",    // imgui.h:2911
            &ImFontAtlas::AddCustomRectRegular, py::arg("width"), py::arg("height"))
        .def("add_custom_rect_font_glyph",    // imgui.h:2912
            &ImFontAtlas::AddCustomRectFontGlyph, py::arg("font"), py::arg("id_"), py::arg("width"), py::arg("height"), py::arg("advance_x"), py::arg("offset") = ImVec2(0, 0))
        .def("calc_custom_rect_uv",    // imgui.h:2916
            &ImFontAtlas::CalcCustomRectUV, py::arg("rect"), py::arg("out_uv_min"), py::arg("out_uv_max"))
        .def("get_mouse_cursor_tex_data",    // imgui.h:2917
            &ImFontAtlas::GetMouseCursorTexData, py::arg("cursor"), py::arg("out_offset"), py::arg("out_size"), py::arg("out_uv_border"), py::arg("out_uv_fill"))
        .def_readwrite("flags", &ImFontAtlas::Flags, "Build flags (see ImFontAtlasFlags_)")    // imgui.h:2923
        .def_readwrite("tex_id", &ImFontAtlas::TexID, "User data to refer to the texture once it has been uploaded to user's graphic systems. It is passed back to you during rendering via the ImDrawCmd structure.")    // imgui.h:2924
        .def_readwrite("tex_desired_width", &ImFontAtlas::TexDesiredWidth, "Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.")    // imgui.h:2925
        .def_readwrite("tex_glyph_padding", &ImFontAtlas::TexGlyphPadding, "Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0 (will also need to set AntiAliasedLinesUseTex = False).")    // imgui.h:2926
        .def_readwrite("locked", &ImFontAtlas::Locked, "Marked as Locked by ImGui::NewFrame() so attempt to modify the atlas will assert.")    // imgui.h:2927
        .def_readwrite("tex_ready", &ImFontAtlas::TexReady, "Set when texture was built matching current font input")    // imgui.h:2931
        .def_readwrite("tex_pixels_use_colors", &ImFontAtlas::TexPixelsUseColors, "Tell whether our texture data is known to use colors (rather than just alpha channel), in order to help backend select a format.")    // imgui.h:2932
        .def_readwrite("tex_width", &ImFontAtlas::TexWidth, "Texture width calculated during Build().")    // imgui.h:2935
        .def_readwrite("tex_height", &ImFontAtlas::TexHeight, "Texture height calculated during Build().")    // imgui.h:2936
        .def_readwrite("tex_uv_scale", &ImFontAtlas::TexUvScale, "= (1.0/TexWidth, 1.0/TexHeight)")    // imgui.h:2937
        .def_readwrite("tex_uv_white_pixel", &ImFontAtlas::TexUvWhitePixel, "Texture coordinates to a white pixel")    // imgui.h:2938
        .def_readonly("font_builder_io", &ImFontAtlas::FontBuilderIO, "Opaque interface to a font builder (default to stb_truetype, can be changed to use FreeType by defining IMGUI_ENABLE_FREETYPE).")    // imgui.h:2945
        .def_readwrite("font_builder_flags", &ImFontAtlas::FontBuilderFlags, "Shared flags (for all fonts) for custom font builder. THIS IS BUILD IMPLEMENTATION DEPENDENT. Per-font override is also available in ImFontConfig.")    // imgui.h:2946
        .def_readwrite("pack_id_mouse_cursors", &ImFontAtlas::PackIdMouseCursors, "Custom texture rectangle ID for white pixel and mouse cursors")    // imgui.h:2949
        .def_readwrite("pack_id_lines", &ImFontAtlas::PackIdLines, "Custom texture rectangle ID for baked anti-aliased lines")    // imgui.h:2950
        ;


    auto pyClassImFont =
        py::class_<ImFont>    // imgui.h:2959
            (m, "ImFont", " Font runtime data and rendering\n ImFontAtlas automatically loads a default embedded font for you when you call GetTexDataAsAlpha8() or GetTexDataAsRGBA32().")
        .def_readwrite("fallback_advance_x", &ImFont::FallbackAdvanceX, "4     // out // = FallbackGlyph->AdvanceX")    // imgui.h:2963
        .def_readwrite("font_size", &ImFont::FontSize, "4     // in  //            // Height of characters/line, set during loading (don't change after loading)")    // imgui.h:2964
        .def_readonly("fallback_glyph", &ImFont::FallbackGlyph, "4-8   // out // = FindGlyph(FontFallbackChar)")    // imgui.h:2969
        .def_readwrite("container_atlas", &ImFont::ContainerAtlas, "4-8   // out //            // What we has been loaded into")    // imgui.h:2972
        .def_readonly("config_data", &ImFont::ConfigData, "4-8   // in  //            // Pointer within ContainerAtlas->ConfigData")    // imgui.h:2973
        .def_readwrite("config_data_count", &ImFont::ConfigDataCount, "2     // in  // ~ 1        // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.")    // imgui.h:2974
        .def_readwrite("fallback_char", &ImFont::FallbackChar, "2     // out // = FFFD/'?' // Character used if a glyph isn't found.")    // imgui.h:2975
        .def_readwrite("ellipsis_char", &ImFont::EllipsisChar, "2     // out // = '...'    // Character used for ellipsis rendering.")    // imgui.h:2976
        .def_readwrite("dot_char", &ImFont::DotChar, "2     // out // = '.'      // Character used for ellipsis rendering (if a single '...' character isn't found)")    // imgui.h:2977
        .def_readwrite("dirty_lookup_tables", &ImFont::DirtyLookupTables, "1     // out //")    // imgui.h:2978
        .def_readwrite("scale", &ImFont::Scale, "4     // in  // = 1.      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()")    // imgui.h:2979
        .def_readwrite("ascent", &ImFont::Ascent, "4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]")    // imgui.h:2980
        .def_readwrite("descent", &ImFont::Descent, "4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]")    // imgui.h:2980
        .def_readwrite("metrics_total_surface", &ImFont::MetricsTotalSurface, "4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)")    // imgui.h:2981
        .def(py::init<>(),    // imgui.h:2985
            "Methods")
        .def("find_glyph",    // imgui.h:2987
            &ImFont::FindGlyph,
            py::arg("c"),
            pybind11::return_value_policy::reference)
        .def("find_glyph_no_fallback",    // imgui.h:2988
            &ImFont::FindGlyphNoFallback,
            py::arg("c"),
            pybind11::return_value_policy::reference)
        .def("calc_word_wrap_position_a",    // imgui.h:2996
            &ImFont::CalcWordWrapPositionA,
            py::arg("scale"), py::arg("text"), py::arg("text_end"), py::arg("wrap_width"),
            pybind11::return_value_policy::reference)
        .def("render_char",    // imgui.h:2997
            &ImFont::RenderChar, py::arg("draw_list"), py::arg("size"), py::arg("pos"), py::arg("col"), py::arg("c"))
        .def("render_text",    // imgui.h:2998
            &ImFont::RenderText, py::arg("draw_list"), py::arg("size"), py::arg("pos"), py::arg("col"), py::arg("clip_rect"), py::arg("text_begin"), py::arg("text_end"), py::arg("wrap_width") = 0.0f, py::arg("cpu_fine_clip") = false)
        .def("build_lookup_table",    // imgui.h:3001
            &ImFont::BuildLookupTable)
        .def("clear_output_data",    // imgui.h:3002
            &ImFont::ClearOutputData)
        .def("grow_index",    // imgui.h:3003
            &ImFont::GrowIndex, py::arg("new_size"))
        .def("add_glyph",    // imgui.h:3004
            &ImFont::AddGlyph, py::arg("src_cfg"), py::arg("c"), py::arg("x0"), py::arg("y0"), py::arg("x1"), py::arg("y1"), py::arg("u0"), py::arg("v0"), py::arg("u1"), py::arg("v1"), py::arg("advance_x"))
        .def("add_remap_char",    // imgui.h:3005
            &ImFont::AddRemapChar,
            py::arg("dst"), py::arg("src"), py::arg("overwrite_dst") = true,
            "Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.")
        .def("set_glyph_visible",    // imgui.h:3006
            &ImFont::SetGlyphVisible, py::arg("c"), py::arg("visible"))
        .def("is_glyph_range_unused",    // imgui.h:3007
            &ImFont::IsGlyphRangeUnused, py::arg("c_begin"), py::arg("c_last"))
        ;


    py::enum_<ImGuiViewportFlags_>(m, "ViewportFlags_", py::arithmetic(), "Flags stored in ImGuiViewport::Flags, giving indications to the platform backends.")    // imgui.h:3015
        .value("none", ImGuiViewportFlags_None, "")
        .value("is_platform_window", ImGuiViewportFlags_IsPlatformWindow, "Represent a Platform Window")
        .value("is_platform_monitor", ImGuiViewportFlags_IsPlatformMonitor, "Represent a Platform Monitor (unused yet)")
        .value("owned_by_app", ImGuiViewportFlags_OwnedByApp, "Platform Window: is created/managed by the application (rather than a dear imgui backend)")
        .value("no_decoration", ImGuiViewportFlags_NoDecoration, "Platform Window: Disable platform decorations: title bar, borders, etc. (generally set all windows, but if ImGuiConfigFlags_ViewportsDecoration is set we only set this on popups/tooltips)")
        .value("no_task_bar_icon", ImGuiViewportFlags_NoTaskBarIcon, "Platform Window: Disable platform task bar icon (generally set on popups/tooltips, or all windows if ImGuiConfigFlags_ViewportsNoTaskBarIcon is set)")
        .value("no_focus_on_appearing", ImGuiViewportFlags_NoFocusOnAppearing, "Platform Window: Don't take focus when created.")
        .value("no_focus_on_click", ImGuiViewportFlags_NoFocusOnClick, "Platform Window: Don't take focus when clicked on.")
        .value("no_inputs", ImGuiViewportFlags_NoInputs, "Platform Window: Make mouse pass through so we can drag this window while peaking behind it.")
        .value("no_renderer_clear", ImGuiViewportFlags_NoRendererClear, "Platform Window: Renderer doesn't need to clear the framebuffer ahead (because we will fill it entirely).")
        .value("top_most", ImGuiViewportFlags_TopMost, "Platform Window: Display on top (for tooltips only).")
        .value("minimized", ImGuiViewportFlags_Minimized, "Platform Window: Window is minimized, can skip render. When minimized we tend to avoid using the viewport pos/size for clipping window or testing if they are contained in the viewport.")
        .value("no_auto_merge", ImGuiViewportFlags_NoAutoMerge, "Platform Window: Avoid merging this window into another host window. This can only be set via ImGuiWindowClass viewport flags override (because we need to now ahead if we are going to create a viewport in the first place!).")
        .value("can_host_other_windows", ImGuiViewportFlags_CanHostOtherWindows, "Main viewport: can host multiple imgui windows (secondary viewports are associated to a single window).");


    auto pyClassImGuiViewport =
        py::class_<ImGuiViewport>    // imgui.h:3040
            (m, "Viewport", " - Currently represents the Platform Window created by the application which is hosting our Dear ImGui windows.\n - With multi-viewport enabled, we extend this concept to have multiple active viewports.\n - In the future we will extend this concept further to also represent Platform Monitor and support a \"no main platform window\" operation mode.\n - About Main Area vs Work Area:\n   - Main Area = entire viewport.\n   - Work Area = entire viewport minus sections used by main menu bars (for platform windows), or by task bar (for platform monitor).\n   - Windows are generally trying to stay within the Work Area of their host viewport.")
        .def_readwrite("id_", &ImGuiViewport::ID, "Unique identifier for the viewport")    // imgui.h:3042
        .def_readwrite("flags", &ImGuiViewport::Flags, "See ImGuiViewportFlags_")    // imgui.h:3043
        .def_readwrite("pos", &ImGuiViewport::Pos, "Main Area: Position of the viewport (Dear ImGui coordinates are the same as OS desktop/native coordinates)")    // imgui.h:3044
        .def_readwrite("size", &ImGuiViewport::Size, "Main Area: Size of the viewport.")    // imgui.h:3045
        .def_readwrite("work_pos", &ImGuiViewport::WorkPos, "Work Area: Position of the viewport minus task bars, menus bars, status bars (>= Pos)")    // imgui.h:3046
        .def_readwrite("work_size", &ImGuiViewport::WorkSize, "Work Area: Size of the viewport minus task bars, menu bars, status bars (<= Size)")    // imgui.h:3047
        .def_readwrite("dpi_scale", &ImGuiViewport::DpiScale, "1.0 = 96 DPI = No extra scale.")    // imgui.h:3048
        .def_readwrite("parent_viewport_id", &ImGuiViewport::ParentViewportId, "(Advanced) 0: no parent. Instruct the platform backend to setup a parent/child relationship between platform windows.")    // imgui.h:3049
        .def_readwrite("draw_data", &ImGuiViewport::DrawData, "The ImDrawData corresponding to this viewport. Valid after Render() and until the next call to NewFrame().")    // imgui.h:3050
        .def_readwrite("renderer_user_data", &ImGuiViewport::RendererUserData, "None* to hold custom data structure for the renderer (e.g. swap chain, framebuffers etc.). generally set by your Renderer_CreateWindow function.")    // imgui.h:3057
        .def_readwrite("platform_user_data", &ImGuiViewport::PlatformUserData, "None* to hold custom data structure for the OS / platform (e.g. windowing info, render context). generally set by your Platform_CreateWindow function.")    // imgui.h:3058
        .def_readwrite("platform_handle", &ImGuiViewport::PlatformHandle, "None* for FindViewportByPlatformHandle(). (e.g. suggested to use natural platform handle such as HWND, GLFWWindow*, SDL_Window*)")    // imgui.h:3059
        .def_readwrite("platform_handle_raw", &ImGuiViewport::PlatformHandleRaw, "None* to hold lower-level, platform-native window handle (under Win32 this is expected to be a HWND, unused for other platforms), when using an abstraction layer like GLFW or SDL (where PlatformHandle would be a SDL_Window*)")    // imgui.h:3060
        .def_readwrite("platform_window_created", &ImGuiViewport::PlatformWindowCreated, "Platform window has been created (Platform_CreateWindow() has been called). This is False during the first frame where a viewport is being created.")    // imgui.h:3061
        .def_readwrite("platform_request_move", &ImGuiViewport::PlatformRequestMove, "Platform window requested move (e.g. window was moved by the OS / host window manager, authoritative position will be OS window position)")    // imgui.h:3062
        .def_readwrite("platform_request_resize", &ImGuiViewport::PlatformRequestResize, "Platform window requested resize (e.g. window was resized by the OS / host window manager, authoritative size will be OS window size)")    // imgui.h:3063
        .def_readwrite("platform_request_close", &ImGuiViewport::PlatformRequestClose, "Platform window requested closure (e.g. window was moved by the OS / host window manager, e.g. pressing ALT-F4)")    // imgui.h:3064
        .def(py::init<>())    // imgui.h:3066
        ;


    auto pyClassImGuiPlatformIO =
        py::class_<ImGuiPlatformIO>    // imgui.h:3124
            (m, "PlatformIO", "(Optional) Access via ImGui::GetPlatformIO()")
        .def(py::init<>(),    // imgui.h:3182
            "Zero clear")
        ;


    auto pyClassImGuiPlatformMonitor =
        py::class_<ImGuiPlatformMonitor>    // imgui.h:3187
            (m, "PlatformMonitor", " (Optional) This is required when enabling multi-viewport. Represent the bounds of each connected monitor/display and their DPI.\n We use this information for multiple DPI support + clamping the position of popups and tooltips so they don't straddle multiple monitors.")
        .def_readwrite("main_pos", &ImGuiPlatformMonitor::MainPos, "Coordinates of the area displayed on this monitor (Min = upper left, Max = bottom right)")    // imgui.h:3189
        .def_readwrite("main_size", &ImGuiPlatformMonitor::MainSize, "Coordinates of the area displayed on this monitor (Min = upper left, Max = bottom right)")    // imgui.h:3189
        .def_readwrite("work_pos", &ImGuiPlatformMonitor::WorkPos, "Coordinates without task bars / side bars / menu bars. Used to avoid positioning popups/tooltips inside this region. If you don't have this info, please copy the value for MainPos/MainSize.")    // imgui.h:3190
        .def_readwrite("work_size", &ImGuiPlatformMonitor::WorkSize, "Coordinates without task bars / side bars / menu bars. Used to avoid positioning popups/tooltips inside this region. If you don't have this info, please copy the value for MainPos/MainSize.")    // imgui.h:3190
        .def_readwrite("dpi_scale", &ImGuiPlatformMonitor::DpiScale, "1.0 = 96 DPI")    // imgui.h:3191
        .def(py::init<>())    // imgui.h:3192
        ;


    auto pyClassImGuiPlatformImeData =
        py::class_<ImGuiPlatformImeData>    // imgui.h:3196
            (m, "PlatformImeData", "(Optional) Support for IME (Input Method Editor) via the io.SetPlatformImeDataFn() function.")
        .def_readwrite("want_visible", &ImGuiPlatformImeData::WantVisible, "A widget wants the IME to be visible")    // imgui.h:3198
        .def_readwrite("input_pos", &ImGuiPlatformImeData::InputPos, "Position of the input cursor")    // imgui.h:3199
        .def_readwrite("input_line_height", &ImGuiPlatformImeData::InputLineHeight, "Line height")    // imgui.h:3200
        .def(py::init<>())    // imgui.h:3202
        ;
    ////////////////////    </generated_from:imgui.h>    ////////////////////


    ////////////////////    <generated_from:imgui_toggle.h>    ////////////////////
    py::enum_<ImGuiToggleFlags_>(m, "ToggleFlags_", py::arithmetic(), "")    // imgui_toggle.h:5
        .value("none", ImGuiToggleFlags_None, "")
        .value("animated", ImGuiToggleFlags_Animated, "The toggle should be animated. Mutually exclusive with ImGuiToggleFlags_Static.")
        .value("static", ImGuiToggleFlags_Static, "The toggle should not animate. Mutually exclusive with ImGuiToggleFlags_Animated.")
        .value("bordered_frame", ImGuiToggleFlags_BorderedFrame, "The toggle should have a border drawn on the frame.")
        .value("bordered_knob", ImGuiToggleFlags_BorderedKnob, "The toggle should have a border drawn on the knob.")
        .value("bordered", ImGuiToggleFlags_Bordered, "Shorthand for bordered frame and knob.")
        .value("default", ImGuiToggleFlags_Default, "The default flags used when no ImGuiToggleFlags_ are specified.");


    m.def("toggle",    // imgui_toggle.h:29
        [](const char * label, bool v) -> std::tuple<bool, bool>
        {
            auto Toggle_adapt_modifiable_immutable_to_return = [](const char * label, bool v) -> std::tuple<bool, bool>
            {
                bool * v_adapt_modifiable = & v;

                bool r = ImGui::Toggle(label, v_adapt_modifiable);
                return std::make_tuple(r, v);
            };

            return Toggle_adapt_modifiable_immutable_to_return(label, v);
        },     py::arg("label"), py::arg("v"));

    m.def("toggle",    // imgui_toggle.h:30
        [](const char * label, bool v, ImGuiToggleFlags flags) -> std::tuple<bool, bool>
        {
            auto Toggle_adapt_modifiable_immutable_to_return = [](const char * label, bool v, ImGuiToggleFlags flags) -> std::tuple<bool, bool>
            {
                bool * v_adapt_modifiable = & v;

                bool r = ImGui::Toggle(label, v_adapt_modifiable, flags);
                return std::make_tuple(r, v);
            };

            return Toggle_adapt_modifiable_immutable_to_return(label, v, flags);
        },     py::arg("label"), py::arg("v"), py::arg("flags"));

    m.def("toggle",    // imgui_toggle.h:31
        [](const char * label, bool v, ImGuiToggleFlags flags, float speed) -> std::tuple<bool, bool>
        {
            auto Toggle_adapt_modifiable_immutable_to_return = [](const char * label, bool v, ImGuiToggleFlags flags, float speed) -> std::tuple<bool, bool>
            {
                bool * v_adapt_modifiable = & v;

                bool r = ImGui::Toggle(label, v_adapt_modifiable, flags, speed);
                return std::make_tuple(r, v);
            };

            return Toggle_adapt_modifiable_immutable_to_return(label, v, flags, speed);
        },     py::arg("label"), py::arg("v"), py::arg("flags"), py::arg("speed"));

    m.def("toggle",    // imgui_toggle.h:32
        [](const char * label, bool v, ImGuiToggleFlags flags, float frame_rounding, float knob_rounding) -> std::tuple<bool, bool>
        {
            auto Toggle_adapt_modifiable_immutable_to_return = [](const char * label, bool v, ImGuiToggleFlags flags, float frame_rounding, float knob_rounding) -> std::tuple<bool, bool>
            {
                bool * v_adapt_modifiable = & v;

                bool r = ImGui::Toggle(label, v_adapt_modifiable, flags, frame_rounding, knob_rounding);
                return std::make_tuple(r, v);
            };

            return Toggle_adapt_modifiable_immutable_to_return(label, v, flags, frame_rounding, knob_rounding);
        },     py::arg("label"), py::arg("v"), py::arg("flags"), py::arg("frame_rounding"), py::arg("knob_rounding"));

    m.def("toggle",    // imgui_toggle.h:33
        [](const char * label, bool v, ImGuiToggleFlags flags, float speed, float frame_rounding, float knob_rounding) -> std::tuple<bool, bool>
        {
            auto Toggle_adapt_modifiable_immutable_to_return = [](const char * label, bool v, ImGuiToggleFlags flags, float speed, float frame_rounding, float knob_rounding) -> std::tuple<bool, bool>
            {
                bool * v_adapt_modifiable = & v;

                bool r = ImGui::Toggle(label, v_adapt_modifiable, flags, speed, frame_rounding, knob_rounding);
                return std::make_tuple(r, v);
            };

            return Toggle_adapt_modifiable_immutable_to_return(label, v, flags, speed, frame_rounding, knob_rounding);
        },     py::arg("label"), py::arg("v"), py::arg("flags"), py::arg("speed"), py::arg("frame_rounding"), py::arg("knob_rounding"));
    ////////////////////    </generated_from:imgui_toggle.h>    ////////////////////


    ////////////////////    <generated_from:imgui_stdlib.h>    ////////////////////
    m.def("input_text",    // imgui_stdlib.h:15
        [](const char * label, std::string str, ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> std::tuple<bool, std::string>
        {
            auto InputText_adapt_exclude_params = [](const char * label, std::string * str, ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> bool
            {
                auto r = ImGui::InputText(label, str, flags, NULL, user_data);
                return r;
            };
            auto InputText_adapt_modifiable_immutable_to_return = [&InputText_adapt_exclude_params](const char * label, std::string str, ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> std::tuple<bool, std::string>
            {
                std::string * str_adapt_modifiable = & str;

                bool r = InputText_adapt_exclude_params(label, str_adapt_modifiable, flags, user_data);
                return std::make_tuple(r, str);
            };

            return InputText_adapt_modifiable_immutable_to_return(label, str, flags, user_data);
        },     py::arg("label"), py::arg("str"), py::arg("flags") = 0, py::arg("user_data") = py::none());

    m.def("input_text_multiline",    // imgui_stdlib.h:16
        [](const char * label, std::string str, const ImVec2 & size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> std::tuple<bool, std::string>
        {
            auto InputTextMultiline_adapt_exclude_params = [](const char * label, std::string * str, const ImVec2 & size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> bool
            {
                auto r = ImGui::InputTextMultiline(label, str, size, flags, NULL, user_data);
                return r;
            };
            auto InputTextMultiline_adapt_modifiable_immutable_to_return = [&InputTextMultiline_adapt_exclude_params](const char * label, std::string str, const ImVec2 & size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> std::tuple<bool, std::string>
            {
                std::string * str_adapt_modifiable = & str;

                bool r = InputTextMultiline_adapt_exclude_params(label, str_adapt_modifiable, size, flags, user_data);
                return std::make_tuple(r, str);
            };

            return InputTextMultiline_adapt_modifiable_immutable_to_return(label, str, size, flags, user_data);
        },     py::arg("label"), py::arg("str"), py::arg("size") = ImVec2(0, 0), py::arg("flags") = 0, py::arg("user_data") = py::none());

    m.def("input_text_with_hint",    // imgui_stdlib.h:17
        [](const char * label, const char * hint, std::string str, ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> std::tuple<bool, std::string>
        {
            auto InputTextWithHint_adapt_exclude_params = [](const char * label, const char * hint, std::string * str, ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> bool
            {
                auto r = ImGui::InputTextWithHint(label, hint, str, flags, NULL, user_data);
                return r;
            };
            auto InputTextWithHint_adapt_modifiable_immutable_to_return = [&InputTextWithHint_adapt_exclude_params](const char * label, const char * hint, std::string str, ImGuiInputTextFlags flags = 0, void * user_data = NULL) -> std::tuple<bool, std::string>
            {
                std::string * str_adapt_modifiable = & str;

                bool r = InputTextWithHint_adapt_exclude_params(label, hint, str_adapt_modifiable, flags, user_data);
                return std::make_tuple(r, str);
            };

            return InputTextWithHint_adapt_modifiable_immutable_to_return(label, hint, str, flags, user_data);
        },     py::arg("label"), py::arg("hint"), py::arg("str"), py::arg("flags") = 0, py::arg("user_data") = py::none());
    ////////////////////    </generated_from:imgui_stdlib.h>    ////////////////////

    // </litgen_pydef> // Autogenerated code end
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
