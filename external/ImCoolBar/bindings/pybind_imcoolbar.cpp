// Part of ImGui Bundle - MIT License - Copyright (c) 2022-2023 Pascal Thomet - https://github.com/pthom/imgui_bundle
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include "ImCoolBar/ImCoolbar.h"  // Change this include to the library you are binding

namespace py = pybind11;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// <litgen_glue_code>  // Autogenerated code below! Do not edit!

// </litgen_glue_code> // Autogenerated code end
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void py_init_module_imcoolbar(py::module& m)
{
    using namespace ImGui;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // <litgen_pydef> // Autogenerated code below! Do not edit!
    ////////////////////    <generated_from:ImCoolbar.h>    ////////////////////
    py::enum_<ImCoolBarFlags_>(m, "ImCoolBarFlags_", py::arithmetic(), "")
        .value("none", ImCoolBarFlags_None, "")
        .value("vertical", ImCoolBarFlags_Vertical, "")
        .value("horizontal", ImCoolBarFlags_Horizontal, "");


    auto pyClassImCoolBarConfig =
        py::class_<ImGui::ImCoolBarConfig>
            (m, "ImCoolBarConfig", "")
        .def_readwrite("anchor", &ImCoolBarConfig::anchor, "")
        .def_readwrite("normal_size", &ImCoolBarConfig::normal_size, "")
        .def_readwrite("hovered_size", &ImCoolBarConfig::hovered_size, "")
        .def_readwrite("anim_step", &ImCoolBarConfig::anim_step, "")
        .def_readwrite("effect_strength", &ImCoolBarConfig::effect_strength, "")
        .def(py::init<const ImVec2, const float &, const float &, const float &, const float &>(),
            py::arg("v_anchor") = ImVec2(-1.0f, -1.0f), py::arg("v_normal_size") = 40.0f, py::arg("v_hovered_size") = 60.0f, py::arg("v_anim_step") = 0.15f, py::arg("v_effect_strength") = 0.5f,
            "//")
        ;


    m.def("begin_cool_bar",
        ImGui::BeginCoolBar, py::arg("v_label"), py::arg("v_cb_flags") = ImCoolBarFlags_Vertical, py::arg("v_config") = ImGui::ImCoolBarConfig{}, py::arg("v_flags") = ImGuiWindowFlags_None);

    m.def("end_cool_bar",
        ImGui::EndCoolBar);

    m.def("cool_bar_item",
        ImGui::CoolBarItem);

    m.def("get_cool_bar_item_width",
        ImGui::GetCoolBarItemWidth);

    m.def("get_cool_bar_item_scale",
        ImGui::GetCoolBarItemScale);
    ////////////////////    </generated_from:ImCoolbar.h>    ////////////////////

    // </litgen_pydef> // Autogenerated code end
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  AUTOGENERATED CODE END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}