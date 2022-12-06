from imgui_bundle.demos.api_demos import *
from imgui_bundle import imgui, immvision


def main():
    from imgui_bundle import run
    import cv2

    image = cv2.imread(demos_assets_folder() + "/images/house.jpg")
    image_params = immvision.ImageParams()
    image_params.image_display_size = (600, 400)

    def gui():
        immvision.image("House", image, image_params)

    run(gui, with_implot=True, with_markdown=True)


if __name__ == "__main__":
    main()
