"""Demo curve editing with ImGuizmo
See equivalent C++ program: demos_cpp/demos_imguizmo/demo_guizmo_curve_edit.main.cpp
"""
from typing import List

import imgui_bundle
from imgui_bundle import imguizmo, ImVec2, imgui
from imgui_bundle.demos.api_demos import GuiFunction


im_curve_edit = imguizmo.im_curve_edit


class RampEdit(im_curve_edit.DelegatePure):
    points: List[List[ImVec2]]
    visible: List[bool]
    min: ImVec2
    max: ImVec2

    def __init__(self):
        im_curve_edit.DelegatePure.__init__(self)

        self.points = []
        self.points.append(
            [
                (-10.0, 0),
                (20.0, 0.6),
                (25.0, 0.2),
                (70.0, 0.4),
                (120.0, 1.0),
            ]
        )

        self.points.append(
            [
                (-50.0, 0.2),
                (33.0, 0.7),
                (80.0, 0.2),
                (82.0, 0.8),
            ]
        )

        self.points.append(
            [
                (40.0, 0),
                (60.0, 0.1),
                (90.0, 0.82),
                (150.0, 0.24),
                (200.0, 0.34),
                (250.0, 0.12),
            ]
        )

        self.visible = [True, True, True]

        self.max = (1.0, 1.0)
        self.min = (0.0, 0.0)

    def get_curve_count(self) -> int:
        return len(self.points)

    def is_visible(self, idx: int) -> bool:
        return self.visible[idx]

    def get_curve_color(self, curve_index: int) -> int:  # overridable (pure virtual)
        cols = [0xFF0000FF, 0xFF00FF00, 0xFFFF0000]
        return cols[curve_index]

    def get_points_list(self, curve_index: int) -> List[ImVec2]:  # overridable (pure virtual)
        return self.points[curve_index]

    def get_curve_type(self, param_0: int) -> im_curve_edit.CurveType:
        return im_curve_edit.CurveType.curve_smooth

    def edit_point(self, curve_index: int, point_index: int, value: ImVec2) -> int:  # overridable (pure virtual)
        self.points[curve_index][point_index] = value
        self._sort_values(curve_index)

        for i, point in enumerate(self.points[curve_index]):
            if point.x == value.x:
                return i
        return point_index

    def add_point(self, curve_index: int, value: ImVec2) -> None:  # overridable (pure virtual)
        self.points[curve_index].append(value)
        self._sort_values(curve_index)

    def _sort_values(self, curve_index: int):
        def cmp(a: ImVec2, b: ImVec2) -> bool:
            return a.x < b.x

        curve = self.points[curve_index]
        curve.sort(key=lambda point: point.x)

        # curve = self.points[curve_index]
        # sorted_curve = sort(curve, key=lambda point: point.x)
        # self.points[curve_index] = sorted_curve

    def get_max(self) -> ImVec2:
        return self.max

    def get_min(self) -> ImVec2:
        return self.min

    def get_background_color(self) -> int:
        return 0


def make_closure_demo_guizmo_curve_edit() -> GuiFunction:
    # ramp_edit will be encapsulated in the closure
    ramp_edit = RampEdit()
    ramp_edit.min = (-100.0, 0.0)
    ramp_edit.max = (300.0, 1.0)

    def gui():
        for i in range(len(ramp_edit.points)):
            label = f"Visible {i}"
            _, ramp_edit.visible[i] = imgui.checkbox(label, ramp_edit.visible[i])

            imgui.same_line()
        imgui.new_line()

        ramp_edit_size = (800.0, 400.0)
        ramp_edit_id = 1
        r = im_curve_edit.edit_pure(ramp_edit, ramp_edit_size, ramp_edit_id)

    return gui


def main():
    gui = make_closure_demo_guizmo_curve_edit()

    # Run app
    imgui_bundle.run(gui, with_markdown=True, window_size=(850, 600))


if __name__ == "__main__":
    main()
