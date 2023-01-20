# Part of ImGui Bundle - MIT License - Copyright (c) 2022-2023 Pascal Thomet - https://github.com/pthom/imgui_bundle
import importlib
import sys
import os

sys.path.append(os.path.dirname(__file__) + "/..")
from bindings_generation import all_external_libraries


def autogenerate_all():
    for lib in all_external_libraries.published_libs():
        generator_script_name = lib.generator_script_name()
        bindings_folder = lib.bindings_folder_abs_path()

        sys.path.append(bindings_folder)
        generator_module = importlib.import_module(generator_script_name)
        generator_module.main()


def write_cmake_all_pybind_files():
    pybind_files = []
    for lib in all_external_libraries.published_libs():
        pybind_files += lib.cpp_pybind_files()

    pybind_files = ["    ${external_dir}/" + f for f in pybind_files]

    cmake_template = """#
# autogenerated file! See autogenerate_all.py
#

set(external_dir ${IMGUIBUNDLE_PATH}/external)

set(all_pybind_files
_FILELIST_
    )
    """
    filelist = "\n".join(pybind_files)

    cmake_content = cmake_template.replace("_FILELIST_", filelist)
    this_dir = os.path.dirname(__file__)
    cmake_file = this_dir + "/cpp/all_pybind_files.cmake"
    with open(cmake_file, "w") as f:
        f.write(cmake_content)


def main():
    autogenerate_all()
    write_cmake_all_pybind_files()


if __name__ == "__main__":
    main()
