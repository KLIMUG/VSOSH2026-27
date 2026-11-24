import turtle as t
from math import sin, cos

while True:
    r = 100
    for angle in range(360):
        move_x_s = 0
        move_y_s = 0
        move_x_a = cos(angle * 3.1415 / 180) * r
        move_y_a = sin(angle * 3.1415 / 180) * r
        for i in range(1, 10):
            if move_x_a % i == 0:
                move_x_s += 1
            if move_y_a % i == 0:
                move_y_s += 1

        t.goto(move_x_s, move_y_s)

t.exitonclick()