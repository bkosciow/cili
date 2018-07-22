from distutils.core import setup, Extension

setup(
    name="ili9325",
    ext_modules=[
        Extension(
            "cili9325",
            ["_cili9325.c", "_cili.c", "ili.c", "cili9325.c", "interface.c", "spi.c", "gpio.c"],
            extra_link_args=['-lwiringPi', '-ljpeg', '-lpng']
        ),
        # Extension(
        #     "cili9486",
        #     ["_cili9486.c", "_cili.c", "ili.c", "cili9486.c", "interface.c", "spi.c", "gpio.c"],
        #     extra_link_args=['-lwiringPi', '-ljpeg', '-lpng']
        # ),
    ],
)

