from distutils.core import setup, Extension

setup(
    name="cili",
    version="0.1",
    url="https://github.com/bkosciow/cili",
    license = "MIT",
    author="Bartosz Kościów",
    author_email="kosci1@gmail.com",
    description="GfxLCD in C",

    ext_modules=[
        Extension(
            "cili.ili9325",
            ["_cili9325.c", "_cili.c", "ili.c", "cili9325.c", "interface.c", "spi.c", "gpio.c"],
            extra_link_args=['-lwiringPi', '-ljpeg', '-lpng']
        ),
        Extension(
            "cili.ili9486",
            ["_cili9486.c", "_cili.c", "ili.c", "cili9486.c", "interface.c", "spi.c", "gpio.c"],
            extra_link_args=['-lwiringPi', '-ljpeg', '-lpng']
        ),
    ],
)

