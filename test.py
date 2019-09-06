import socket

# if socket.gethostname() == "rpiv2":
import gfxcili.ili9486
    # width, height, SPI, SPEED, CS, RST, RS
lcd = gfxcili.ili9486.ili9486(320, 480, 0, 3200000, 8, 25, 24)
# else:
#     import gfxcili.ili9325
#     import RPi.GPIO  # pylint: disable=I0011,F0401
#     RPi.GPIO.setmode(RPi.GPIO.BCM)
#     RPi.GPIO.setup(6, RPi.GPIO.OUT)
#     RPi.GPIO.output(6, 1)
#     lcd = gfxcili.ili9325.ili9325(320, 240, 18, 27, 17, 25, 22, 23, 24, 5, 12, 16, 20, 21)

lcd.rotation = 270
print(lcd.width, lcd.height, lcd.rotation)
lcd.init()

lcd.color = (0,0,0)
lcd.background_color = (255,255,255)
lcd.fill_rect(0,0,10,10)

lcd.background_color = (255,255,0)
lcd.fill_rect(0,0,100,200)
lcd.background_color = (128,255,128)
lcd.background_color = (0,255,255)
lcd.fill_rect(0,0,240-1,320-1)
lcd.background_color = (255,0,0)
lcd.fill_rect(0,0,10,10)
lcd.fill_rect(229,0,239,10)
lcd.fill_rect(0,309,10,319)
lcd.fill_rect(229,309,239,319)

lcd.draw_circle(131, 232, 31)
lcd.draw_circle(119, 222, 7)
lcd.draw_circle(143, 222, 7)
lcd.draw_arc(131, 232, 20, 45, 135)
lcd.draw_line(131, 227, 127, 238)
lcd.draw_line(131, 227, 135, 238)
lcd.draw_arc(131, 235, 5, 45, 135)
lcd.fill_rect(195, 204, 205, 210)
lcd.draw_rect(180, 210, 220, 225)
lcd.fill_rect(180, 226, 220, 259)
