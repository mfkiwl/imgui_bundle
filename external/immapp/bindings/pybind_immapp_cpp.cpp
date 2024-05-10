// Part of ImGui Bundle - MIT License - Copyright (c) 2022-2024 Pascal Thomet - https://github.com/pthom/imgui_bundle
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <pybind11/stl_bind.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "immapp/immapp.h"
#include "immapp/code_utils.h"
#include "immapp/snippets.h"
#include "immapp/immapp_widgets.h"
#include "imgui-node-editor/imgui_node_editor_internal.h"

namespace py = pybind11;


namespace ax
{
    namespace NodeEditor
    {
        // using EditorContext = Detail::EditorContext;
        struct EditorContext: public Detail::EditorContext {};
    }
}




// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// <litgen_glue_code>  // Autogenerated code below! Do not edit!

// </litgen_glue_code> // Autogenerated code end
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#include <string>
#include <vector>


void py_init_module_immapp_cpp(py::module& m)
{
    using namespace ImmApp;
    using namespace Snippets;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // <litgen_pydef> // Autogenerated code below! Do not edit!
    ////////////////////    <generated_from:immapp.h>    ////////////////////
    ////////////////////    </generated_from:immapp.h>    ////////////////////


    ////////////////////    <generated_from:immapp_widgets.h>    ////////////////////
    m.def("begin_plot_in_node_editor",
        ImmApp::BeginPlotInNodeEditor, py::arg("title_id"), py::arg("size") = ImVec2(-1,0), py::arg("flags") = 0);

    m.def("end_plot_in_node_editor",
        ImmApp::EndPlotInNodeEditor);

    m.def("show_resizable_plot_in_node_editor",
        ImmApp::ShowResizablePlotInNodeEditor,
        py::arg("title_id"), py::arg("size_pixels"), py::arg("plot_function"), py::arg("flags") = 0, py::arg("resize_handle_size_em") = 1.0f,
        " ShowResizablePlotInNodeEditor: shows a resizable plot inside a node\n Returns the new size of the plot");

    m.def("show_resizable_plot_in_node_editor_em",
        ImmApp::ShowResizablePlotInNodeEditor_Em,
        py::arg("title_id"), py::arg("size_em"), py::arg("plot_function"), py::arg("flags") = 0, py::arg("resize_handle_size_em") = 1.0f,
        " ShowResizablePlotInNodeEditor_Em: shows a resizable plot inside a node\n Returns the new size of the plot. Units are in em.");

    m.def("widget_with_resize_handle_in_node_editor",
        ImmApp::WidgetWithResizeHandle_InNodeEditor,
        py::arg("gui_function"), py::arg("resize_handle_size_em") = 1.0f,
        " WidgetWithResizeHandle_InNodeEditor: shows a resizable widget inside a node\n Returns the new size of the widget.");

    m.def("widget_with_resize_handle_in_node_editor_em",
        ImmApp::WidgetWithResizeHandle_InNodeEditor_Em,
        py::arg("gui_function"), py::arg("resize_handle_size_em") = 1.0f,
        " WidgetWithResizeHandle_InNodeEditor_Em: shows a resizable widget inside a node\n Returns the new size of the widget. Size is in em.");
    ////////////////////    </generated_from:immapp_widgets.h>    ////////////////////


    ////////////////////    <generated_from:runner.h>    ////////////////////
    // #ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
    //
    // #endif
    //


    // #ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
    //
    // #endif
    //


    auto pyClassAddOnsParams =
        py::class_<ImmApp::AddOnsParams>
            (m, "AddOnsParams", "///////////////////////////////////////////////////////////////////////////////////////\n\n AddOnParams: require specific ImGuiBundle packages (markdown, node editor, texture viewer)\n to be initialized at startup.\n\n/////////////////////////////////////////////////////////////////////////////////////")
        .def(py::init<>([](
        bool withImplot = false, bool withMarkdown = false, bool withNodeEditor = false, bool withTexInspect = false, std::optional<NodeEditorConfig> withNodeEditorConfig = std::nullopt, std::optional<ImGuiMd::MarkdownOptions> withMarkdownOptions = std::nullopt)
        {
            auto r = std::make_unique<ImmApp::AddOnsParams>();
            r->withImplot = withImplot;
            r->withMarkdown = withMarkdown;
            r->withNodeEditor = withNodeEditor;
            r->withTexInspect = withTexInspect;
            r->withNodeEditorConfig = withNodeEditorConfig;
            r->withMarkdownOptions = withMarkdownOptions;
            return r;
        })
        , py::arg("with_implot") = false, py::arg("with_markdown") = false, py::arg("with_node_editor") = false, py::arg("with_tex_inspect") = false, py::arg("with_node_editor_config") = py::none(), py::arg("with_markdown_options") = py::none()
        )
        .def_readwrite("with_implot", &ImmApp::AddOnsParams::withImplot, "Set withImplot=True if you need to plot graphs")
        .def_readwrite("with_markdown", &ImmApp::AddOnsParams::withMarkdown, " Set withMarkdown=True if you need to render Markdown\n (alternatively, you can set withMarkdownOptions)")
        .def_readwrite("with_node_editor", &ImmApp::AddOnsParams::withNodeEditor, " Set withNodeEditor=True if you need to render a node editor\n (alternatively, you can set withNodeEditorConfig)")
        .def_readwrite("with_tex_inspect", &ImmApp::AddOnsParams::withTexInspect, "Set withTexInspect=True if you need to use imgui_tex_inspect")
        // #ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
        //
        .def_readwrite("with_node_editor_config", &ImmApp::AddOnsParams::withNodeEditorConfig, "You can tweak NodeEditorConfig (but this is optional)")
        // #endif
        //
        .def_readwrite("with_markdown_options", &ImmApp::AddOnsParams::withMarkdownOptions, "You can tweak MarkdownOptions (but this is optional)")
        ;


    m.def("run",
        py::overload_cast<HelloImGui::RunnerParams &, const ImmApp::AddOnsParams &>(ImmApp::Run), py::arg("runner_params"), py::arg("add_ons_params") = ImmApp::AddOnsParams());

    m.def("run",
        py::overload_cast<const HelloImGui::SimpleRunnerParams &, const ImmApp::AddOnsParams &>(ImmApp::Run), py::arg("simple_params"), py::arg("add_ons_params") = ImmApp::AddOnsParams());

    m.def("run",
        py::overload_cast<const VoidFunction &, const std::string &, bool, bool, const ScreenSize &, float, bool, bool, bool, bool, const std::optional<NodeEditorConfig> &, const std::optional<ImGuiMd::MarkdownOptions> &>(ImmApp::Run),
        py::arg("gui_function"), py::arg("window_title") = "", py::arg("window_size_auto") = false, py::arg("window_restore_previous_geometry") = false, py::arg("window_size") = DefaultWindowSize, py::arg("fps_idle") = 10.f, py::arg("with_implot") = false, py::arg("with_markdown") = false, py::arg("with_node_editor") = false, py::arg("with_tex_inspect") = false, py::arg("with_node_editor_config") = py::none(), py::arg("with_markdown_options") = py::none(),
        "///////////////////////////////////////////////////////////////////////////////////////\n\n Helpers to run an app from Python (using named parameters)\n\n/////////////////////////////////////////////////////////////////////////////////////\n Helper to run an app inside imgui_bundle, using HelloImGui:\n\n (HelloImGui::SimpleRunnerParams)\n     - `guiFunction`: the function that will render the ImGui widgets\n     - `windowTitle`: title of the window\n     - `windowSizeAuto`: if True, autosize the window from its inner widgets\n     - `windowRestorePreviousGeometry`: if True, restore window size and position from last run\n     - `windowSize`: size of the window\n     - `fpsIdle`: fps of the application when idle\n\n (ImmApp::AddOnsParams)\n     - `with_implot`: if True, then a context for implot will be created/destroyed automatically\n     - `with_markdown` / `with_markdown_options`: if specified, then  the markdown context will be initialized\n       (i.e. required fonts will be loaded)\n     - `with_node_editor` / `with_node_editor_config`: if specified, then a context for imgui_node_editor\n       will be created automatically.");

    m.def("run_with_markdown",
        ImmApp::RunWithMarkdown,
        py::arg("gui_function"), py::arg("window_title") = "", py::arg("window_size_auto") = false, py::arg("window_restore_previous_geometry") = false, py::arg("window_size") = DefaultWindowSize, py::arg("fps_idle") = 10.f, py::arg("with_implot") = false, py::arg("with_node_editor") = false, py::arg("with_tex_inspect") = false, py::arg("with_node_editor_config") = py::none(), py::arg("with_markdown_options") = py::none(),
        "Run an application with markdown");

    m.def("em_size",
        py::overload_cast<>(ImmApp::EmSize), " EmSize() returns the visible font size on the screen. For good results on HighDPI screens, always scale your\n widgets and windows relatively to this size.\n It is somewhat comparable to the [em CSS Unit](https://lyty.dev/css/css-unit.html).\n EmSize() = ImGui::GetFontSize()");

    m.def("em_size",
        py::overload_cast<float>(ImmApp::EmSize),
        py::arg("nb_lines"),
        "EmSize(nbLines) returns a size corresponding to nbLines text lines");

    m.def("em_to_vec2",
        py::overload_cast<float, float>(ImmApp::EmToVec2), py::arg("x"), py::arg("y"));

    m.def("em_to_vec2",
        py::overload_cast<ImVec2>(ImmApp::EmToVec2), py::arg("v"));

    m.def("pixels_to_em",
        ImmApp::PixelsToEm,
        py::arg("pixels"),
        "PixelsToEm() converts a Vec2 in pixels to a Vec2 in em");

    m.def("pixel_size_to_em",
        ImmApp::PixelSizeToEm,
        py::arg("pixel_size"),
        "PixelSizeToEm() converts a size in pixels to a size in em");
    // #ifdef IMGUI_BUNDLE_WITH_IMGUI_NODE_EDITOR
    //

    m.def("default_node_editor_context",
        ImmApp::DefaultNodeEditorContext, pybind11::return_value_policy::reference);

    m.def("default_node_editor_config",
        ImmApp::DefaultNodeEditorConfig, pybind11::return_value_policy::reference);

    m.def("node_editor_settings_location",
        ImmApp::NodeEditorSettingsLocation,
        py::arg("runner_params"),
        "NodeEditorSettingsLocation returns the path to the json file for the node editor settings.");

    m.def("has_node_editor_settings",
        ImmApp::HasNodeEditorSettings,
        py::arg("runner_params"),
        "HasNodeEditorSettings returns True if the json file for the node editor settings exists.");

    m.def("delete_node_editor_settings",
        ImmApp::DeleteNodeEditorSettings,
        py::arg("runner_params"),
        "DeleteNodeEditorSettings deletes the json file for the node editor settings.");
    // #endif
    // }
    ////////////////////    </generated_from:runner.h>    ////////////////////


    ////////////////////    <generated_from:clock.h>    ////////////////////
    m.def("clock_seconds",
        ImmApp::ClockSeconds, "Chronometer in seconds");
    ////////////////////    </generated_from:clock.h>    ////////////////////


    ////////////////////    <generated_from:code_utils.h>    ////////////////////

    { // <namespace CodeUtils>
        py::module_ pyNsCodeUtils = m.def_submodule("code_utils", "namespace CodeUtils");
        pyNsCodeUtils.def("unindent",
            CodeUtils::Unindent, py::arg("code"), py::arg("is_markdown"));

        pyNsCodeUtils.def("unindent_code",
            CodeUtils::UnindentCode, py::arg("code"));

        pyNsCodeUtils.def("unindent_markdown",
            CodeUtils::UnindentMarkdown, py::arg("code"));
    } // </namespace CodeUtils>
    ////////////////////    </generated_from:code_utils.h>    ////////////////////


    ////////////////////    <generated_from:snippets.h>    ////////////////////

    { // <namespace Snippets>
        py::module_ pyNsSnippets = m.def_submodule("snippets", "");
        py::enum_<Snippets::SnippetLanguage>(pyNsSnippets, "SnippetLanguage", py::arithmetic(), "")
            .value("cpp", Snippets::SnippetLanguage::Cpp, "")
            .value("hlsl", Snippets::SnippetLanguage::Hlsl, "")
            .value("glsl", Snippets::SnippetLanguage::Glsl, "")
            .value("c", Snippets::SnippetLanguage::C, "")
            .value("sql", Snippets::SnippetLanguage::Sql, "")
            .value("angel_script", Snippets::SnippetLanguage::AngelScript, "")
            .value("lua", Snippets::SnippetLanguage::Lua, "")
            .value("python", Snippets::SnippetLanguage::Python, "");


        py::enum_<Snippets::SnippetTheme>(pyNsSnippets, "SnippetTheme", py::arithmetic(), "")
            .value("dark", Snippets::SnippetTheme::Dark, "")
            .value("light", Snippets::SnippetTheme::Light, "")
            .value("retro_blue", Snippets::SnippetTheme::RetroBlue, "")
            .value("mariana", Snippets::SnippetTheme::Mariana, "");


        pyNsSnippets.def("default_snippet_language",
            Snippets::DefaultSnippetLanguage, "DefaultSnippetLanguage will be Cpp or Python if using python bindings.");


        auto pyNsSnippets_ClassSnippetData =
            py::class_<Snippets::SnippetData>
                (pyNsSnippets, "SnippetData", "")
            .def(py::init<>([](
            std::string Code = "", Snippets::SnippetLanguage Language = Snippets::DefaultSnippetLanguage(), Snippets::SnippetTheme Palette = Snippets::SnippetTheme::Light, bool ShowCopyButton = true, bool ShowCursorPosition = true, std::string DisplayedFilename = {}, int HeightInLines = 0, int MaxHeightInLines = 40, bool ReadOnly = false, bool Border = false, bool DeIndentCode = true)
            {
                auto r = std::make_unique<Snippets::SnippetData>();
                r->Code = Code;
                r->Language = Language;
                r->Palette = Palette;
                r->ShowCopyButton = ShowCopyButton;
                r->ShowCursorPosition = ShowCursorPosition;
                r->DisplayedFilename = DisplayedFilename;
                r->HeightInLines = HeightInLines;
                r->MaxHeightInLines = MaxHeightInLines;
                r->ReadOnly = ReadOnly;
                r->Border = Border;
                r->DeIndentCode = DeIndentCode;
                return r;
            })
            , py::arg("code") = "", py::arg("language") = Snippets::DefaultSnippetLanguage(), py::arg("palette") = Snippets::SnippetTheme::Light, py::arg("show_copy_button") = true, py::arg("show_cursor_position") = true, py::arg("displayed_filename") = std::string{}, py::arg("height_in_lines") = 0, py::arg("max_height_in_lines") = 40, py::arg("read_only") = false, py::arg("border") = false, py::arg("de_indent_code") = true
            )
            .def_readwrite("code", &Snippets::SnippetData::Code, "")
            .def_readwrite("language", &Snippets::SnippetData::Language, "")
            .def_readwrite("palette", &Snippets::SnippetData::Palette, "")
            .def_readwrite("show_copy_button", &Snippets::SnippetData::ShowCopyButton, "Displayed on top of the editor (Top Right corner)")
            .def_readwrite("show_cursor_position", &Snippets::SnippetData::ShowCursorPosition, "Show line and column number")
            .def_readwrite("displayed_filename", &Snippets::SnippetData::DisplayedFilename, "Displayed on top of the editor")
            .def_readwrite("height_in_lines", &Snippets::SnippetData::HeightInLines, "Number of visible lines in the editor")
            .def_readwrite("max_height_in_lines", &Snippets::SnippetData::MaxHeightInLines, "If the number of lines in the code exceeds this, the editor will scroll. Set to 0 to disable.")
            .def_readwrite("read_only", &Snippets::SnippetData::ReadOnly, "Snippets are read-only by default")
            .def_readwrite("border", &Snippets::SnippetData::Border, "Draw a border around the editor")
            .def_readwrite("de_indent_code", &Snippets::SnippetData::DeIndentCode, "Keep the code indentation, but remove main indentation,")
            ;


        pyNsSnippets.def("show_code_snippet",
            Snippets::ShowCodeSnippet, py::arg("snippet_data"), py::arg("width") = 0.f, py::arg("override_height_in_lines") = 0);

        pyNsSnippets.def("show_side_by_side_snippets",
            py::overload_cast<const Snippets::SnippetData &, const Snippets::SnippetData &, bool, bool>(Snippets::ShowSideBySideSnippets), py::arg("snippet1"), py::arg("snippet2"), py::arg("hide_if_empty") = true, py::arg("equal_visible_lines") = true);

        pyNsSnippets.def("show_side_by_side_snippets",
            py::overload_cast<const std::vector<Snippets::SnippetData> &, bool, bool>(Snippets::ShowSideBySideSnippets), py::arg("snippets"), py::arg("hide_if_empty") = true, py::arg("equal_visible_lines") = true);
    } // </namespace Snippets>
    ////////////////////    </generated_from:snippets.h>    ////////////////////

    // </litgen_pydef> // Autogenerated code end
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
