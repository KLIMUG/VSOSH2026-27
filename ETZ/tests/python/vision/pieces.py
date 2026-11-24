"""Каркас распознавания шашек: цвет по HSV + координаты через field.py.

Здесь только скелет — дырки помечены TODO, их заполняешь сам:
  * HSV-диапазоны своих шашек (подобрать под освещение полигона);
  * фильтр по размеру/круглости под своё разрешение камеры;
  * стопки на ЗЭ (сейчас берётся только верхний контур).
"""

import cv2
import numpy as np

import field

# TODO: подобрать под свои шашки. Формат: имя -> список (low, high).
# H в OpenCV 0..179, S и V 0..255. Красный всегда режется на два куска у нуля.
HSV_RANGES = {
    "red": [((0, 120, 80), (10, 255, 255)), ((170, 120, 80), (179, 255, 255))],
    "green": [((40, 90, 60), (85, 255, 255))],
    "blue": [((95, 120, 60), (130, 255, 255))],
    "yellow": [((20, 120, 100), (35, 255, 255))],
    "purple": [((135, 80, 60), (165, 255, 255))],
}

# TODO: пересчитать под своё разрешение. Шашка диаметром 26 мм при ~2 px/мм
# даёт ~50 px диаметра, то есть площадь ~2000 px.
MIN_AREA = 400
ROUNDNESS = (0.75, 1.02)


def color_mask(hsv, color):
    """Бинарная маска одного цвета по HSV_RANGES."""
    mask = np.zeros(hsv.shape[:2], np.uint8)
    for low, high in HSV_RANGES[color]:
        mask |= cv2.inRange(hsv, np.array(low), np.array(high))
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    return cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)


def find_pieces(frame, M):
    """-> [{'color', 'px', 'mm', 'cell'}] для найденных шашек."""
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    pieces = []
    for color in HSV_RANGES:
        contours = cv2.findContours(
            color_mask(hsv, color), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )[0]
        for contour in contours:
            area = cv2.contourArea(contour)
            if area < MIN_AREA:
                continue
            (cx, cy), radius = cv2.minEnclosingCircle(contour)
            fill = area / (np.pi * radius * radius)
            if not (ROUNDNESS[0] < fill <= ROUNDNESS[1]):
                continue
            x_mm, y_mm = field.to_mm(M, (cx, cy))
            pieces.append({
                "color": color,
                "px": (cx, cy),
                "mm": (x_mm, y_mm),
                "cell": field.cell_of(x_mm, y_mm),
            })
    # TODO: склеить дубли (красный даёт два контура), отбросить шашки вне поля,
    # TODO: разобрать стопки — нижняя шашка задаёт положение, верхняя — цвет.
    return pieces
