"""Привязка кадра к системе координат поля по ArUco-меткам DICT_4X4_50.

Готово: чтение меток 27/28/29 и пересчёт "пиксели -> миллиметры поля".
Не готово (твоя часть): цвета шашек, стопки, логика задач.

Зависимости: pip install opencv-contrib-python numpy
(contrib обязателен, cv2.aruco живёт только там)
"""

import cv2
import numpy as np

STEP = 28.0        # шаг сетки поля, мм
MARKER_MM = 18.0   # сторона метки, мм

# Центры меток в координатах поля, мм.
# 27 — начало координат, 28 — по OX на уровне столбца 8, 29 — отбой, строка Е.
# ОБЯЗАТЕЛЬНО проверь линейкой на своём листе и поправь: от этих чисел зависит
# масштаб и поворот всей картины.
MARKERS_MM = {
    27: (0.0, 0.0),
    28: (8 * STEP, 0.0),
    29: (9 * STEP, 5 * STEP),
}

# Порядок углов, который отдаёт ArUco: TL, TR, BR, BL (в пикселях, Y вниз).
# В мире Y направлена вверх, поэтому знаки другие.
CORNER_SIGNS = ((-1, 1), (1, 1), (1, -1), (-1, -1))


def make_detector():
    """Детектор под штатные метки ЕТЗ. Параметры можно подкручивать под камеру."""
    dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
    params = cv2.aruco.DetectorParameters()
    params.cornerRefinementMethod = cv2.aruco.CORNER_REFINE_SUBPIX
    params.minMarkerPerimeterRate = 0.02   # метка занимает малую часть кадра
    params.maxMarkerPerimeterRate = 0.30
    return cv2.aruco.ArucoDetector(dictionary, params)


def read_markers(detector, gray):
    """-> {id: углы (4, 2) в пикселях} только для меток из MARKERS_MM."""
    corners, ids, _rejected = detector.detectMarkers(gray)
    if ids is None:
        return {}
    return {
        int(i): c.reshape(4, 2).astype(np.float64)
        for c, i in zip(corners, ids.ravel())
        if int(i) in MARKERS_MM
    }


def field_transform(markers):
    """Аффинное преобразование пиксели -> мм по углам всех видимых меток.

    -> (M 3x2/2x3, максимальная невязка в мм) или (None, None), если меток < 2.
    Невязка больше ~1.5 мм значит: метки напечатаны повёрнуто, кадр зеркален
    или лист сильно наклонён к камере.
    """
    if len(markers) < 2:
        return None, None

    src, dst = [], []
    for mid, corners in markers.items():
        cx, cy = MARKERS_MM[mid]
        for k, (sx, sy) in enumerate(CORNER_SIGNS):
            src.append(corners[k])
            dst.append((cx + sx * MARKER_MM / 2, cy + sy * MARKER_MM / 2))

    src = np.float32(src)
    dst = np.float32(dst)
    M, _inliers = cv2.estimateAffinePartial2D(src, dst, method=cv2.LMEDS)
    if M is None:
        return None, None

    projected = src @ M[:, :2].T + M[:, 2]
    err = np.linalg.norm(projected - dst, axis=1)
    return M, float(err.max())


def to_mm(M, px):
    """Точка кадра -> (X, Y) в мм поля."""
    x = M[0, 0] * px[0] + M[0, 1] * px[1] + M[0, 2]
    y = M[1, 0] * px[0] + M[1, 1] * px[1] + M[1, 2]
    return float(x), float(y)


def cell_of(x_mm, y_mm):
    """(X, Y) мм -> 'Б3' или None, если вне поля. А = 1 ... Е = 6."""
    n, k = round(x_mm / STEP), round(y_mm / STEP)
    if not (0 <= n <= 8 and 1 <= k <= 6):
        return None
    return f"{'АБВГДЕ'[k - 1]}{n}"
