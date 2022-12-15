"""ImmApp: Immediate App Toolkit for ImGui Bundle
"""

from typing import Tuple, Optional, Callable
import numpy as np

from imgui_bundle import imgui_md, hello_imgui, ImVec2
from imgui_bundle.imgui_node_editor import Config as NodeEditorConfig, EditorContext as NodeEditorContext


ImGuiMd = imgui_md
HelloImGui = hello_imgui

VoidFunction = Callable[[None], None]
ScreenSize = Tuple[int, int]
DefaultScreenSize = (800, 600)



# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# <litgen_stub> // Autogenerated code below! Do not edit!
####################    <generated_from:immapp.h>    ####################
####################    </generated_from:immapp.h>    ####################


####################    <generated_from:runner.h>    ####################
""" namespace ImmApp"""

class AddOnsParams:
    """AddOnParams: require specific ImGuiBundle packages (markdown, node editor, texture viewer)
    to be initialized at startup.
    """

    # Set withImplot=True if you need to plot graphs
    with_implot: bool = False

    # Set withMarkdown=True if you need to render Markdown
    # (alternatively, you can set withMarkdownOptions)
    with_markdown: bool = False

    # Set withNodeEditor=True if you need to render a node editor
    # (alternatively, you can set withNodeEditorConfig)
    with_node_editor: bool = False

    # Set withTexInspect=True if you need to use imgui_tex_inspect
    with_tex_inspect: bool = False

    # You can tweak NodeEditorConfig (but this is optional)
    with_node_editor_config: Optional[NodeEditorConfig] = None

    # You can tweak MarkdownOptions (but this is optional)
    with_markdown_options: Optional[ImGuiMd.MarkdownOptions] = None
    def __init__(
        self,
        with_implot: bool = False,
        with_markdown: bool = False,
        with_node_editor: bool = False,
        with_tex_inspect: bool = False,
        with_node_editor_config: Optional[NodeEditorConfig] = None,
        with_markdown_options: Optional[ImGuiMd.MarkdownOptions] = None,
    ) -> None:
        """Auto-generated default constructor with named params"""
        pass

def run(
    runner_params: HelloImGui.RunnerParams,
    add_ons_params: AddOnsParams = AddOnsParams(),
) -> None:
    pass

def run(
    simple_params: HelloImGui.SimpleRunnerParams,
    add_ons_params: AddOnsParams = AddOnsParams(),
) -> None:
    pass

# ///////////////////////////////////////////////////////////////////////////////////////
#
# Helpers to run an app from Python (using named parameters)
#
# /////////////////////////////////////////////////////////////////////////////////////

def run(
    gui_function: VoidFunction,
    window_title: str = "",
    window_size_auto: bool = False,
    window_restore_previous_geometry: bool = False,
    window_size: ScreenSize = DefaultScreenSize,
    fps_idle: float = 10.0,
    with_implot: bool = False,
    with_markdown: bool = False,
    with_node_editor: bool = False,
    with_tex_inspect: bool = False,
    with_node_editor_config: Optional[NodeEditorConfig] = None,
    with_markdown_options: Optional[ImGuiMd.MarkdownOptions] = None,
) -> None:
    """Helper to run an app inside imgui_bundle, using HelloImGui:

    (HelloImGui::SimpleRunnerParams)
        - `guiFunction`: the function that will render the ImGui widgets
        - `windowTitle`: title of the window
        - `windowSizeAuto`: if True, autosize the window from its inner widgets
        - `windowRestorePreviousGeometry`: if True, restore window size and position from last run
        - `windowSize`: size of the window
        - `fpsIdle`: fps of the application when idle

    (ImmApp::AddOnsParams)
        - `with_implot`: if True, then a context for implot will be created/destroyed automatically
        - `with_markdown` / `with_markdown_options`: if specified, then  the markdown context will be initialized
          (i.e. required fonts will be loaded)
        - `with_node_editor` / `with_node_editor_config`: if specified, then a context for imgui_node_editor
          will be created automatically.
    """
    pass

def run_with_markdown(
    gui_function: VoidFunction,
    window_title: str = "",
    window_size_auto: bool = False,
    window_restore_previous_geometry: bool = False,
    window_size: ScreenSize = DefaultScreenSize,
    fps_idle: float = 10.0,
    with_implot: bool = False,
    with_node_editor: bool = False,
    with_tex_inspect: bool = False,
    with_node_editor_config: Optional[NodeEditorConfig] = None,
    with_markdown_options: Optional[ImGuiMd.MarkdownOptions] = None,
) -> None:
    """Helper for C++ to run an app with markdown (Python can use Run() with named params)"""
    pass

def em_size() -> float:
    """EmSize() returns the visible font size on the screen. For good results on HighDPI screens, always scale your
    widgets and windows relatively to this size.
    It is somewhat comparable to the [em CSS Unit](https://lyty.dev/css/css-unit.html).
    EmSize() = ImGui::GetFontSize()
    """
    pass

def em_size(nb_lines: float) -> float:
    """EmSize(nbLines) returns a size corresponding to nbLines text lines"""
    pass

# EmToVec2() returns an ImVec2 that you can use to size or place your widgets in a DPI independent way
def em_to_vec2(x: float, y: float) -> ImVec2:
    pass

def em_to_vec2(v: ImVec2) -> ImVec2:
    pass
####################    </generated_from:runner.h>    ####################


####################    <generated_from:utils.h>    ####################
""" namespace ImmApp"""

def clock_seconds() -> float:
    """Chronometer in seconds"""
    pass

def current_node_editor_context() -> NodeEditorContext:
    pass

def suspend_node_editor_canvas() -> None:
    pass

def resume_node_editor_canvas() -> None:
    pass
####################    </generated_from:utils.h>    ####################

# </litgen_stub> // Autogenerated code end!
