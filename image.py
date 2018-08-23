import socket
import os
from PIL import Image
from assets.font import numbers_15x28

if socket.gethostname() == "rpiv2":
    import cili9486
    # width, height, SPI, SPEED, CS, RST, RS
    lcd = cili9486.ili(320, 480, 0, 3200000, 8, 25, 24)
else:
    import cili9325
    import RPi.GPIO  # pylint: disable=I0011,F0401
    RPi.GPIO.setmode(RPi.GPIO.BCM)
    RPi.GPIO.setup(6, RPi.GPIO.OUT)
    RPi.GPIO.output(6, 1)
    lcd = cili9325.ili(320, 240, 18, 27, 17, 25, 22, 23, 24, 5, 12, 16, 20, 21)

lcd.rotation = 0
lcd.init()
FONTS = {
    '15x28': numbers_15x28.Numbers(),
}
#print(lcd.transparency_color );
# image_file = Image.open("assets/japan_temple_240x320.jpg")
# lcd.draw_image(0, 0, image_file)

lcd.draw_image(0, 0, "assets/japan_temple_240x320.jpg")
lcd.transparency_color = (255, 255, 255)
# lcd.transparency_color = ((1, 1, 1), (9, 9, 9))
lcd.draw_image(10, 10, "assets/numbers.jpg")

# lcd.transparency_color = (0, 0, 0)
# lcd.draw_image(10, 0, "assets/dsp2017_101_64.png")

numbers_image = Image.open("assets/dsp2017_101_64.png")
lcd.transparency_color = (0, 0, 0)
# print(lcd.transparency_color)
lcd.draw_image(10, 100, numbers_image)
# lcd.transparency_color = ((4,4,4),(1, 1, 1), (9, 9, 9))
#lcd.transparency_color = None
lcd.transparency_color = FONTS['15x28'].get_transparency()
lcd.draw_image(
    10,
    10,
    FONTS['15x28'].get(0)
)
lcd.draw_image(
    30,
    10,
    FONTS['15x28'].get(1)
)
lcd.draw_image(
    50,
    10,
    FONTS['15x28'].get(9)
)



print("end")