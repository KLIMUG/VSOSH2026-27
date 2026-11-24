"""Запуск распознавания на камере или на одном кадре.

    python main.py            # живая камера
    python main.py foto.png   # один файл
    python main.py 1          # камера с индексом 1

Клавиши: ESC — выход, S — сохранить текущий кадр в snapshot.png.
"""

import sys

import cv2

import field
import pieces


def draw(frame, markers, M, found):
    cv2.aruco.drawDetectedMarkers(frame, [c for c in markers.values()])
    for mid, corners in markers.items():
        center = tuple(map(int, corners.mean(axis=0)))
        cv2.putText(frame, str(mid), center, cv2.FONT_HERSHEY_SIMPLEX,
                    0.7, (0, 255, 0), 2)
    for item in found:
        cv2.circle(frame, tuple(map(int, item["px"])), 14, (255, 255, 255), 2)
        label = f'{item["color"]} {item["cell"] or "?"}'
        cv2.putText(frame, label, tuple(map(int, item["px"])),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)


def process(frame, detector):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    markers = field.read_markers(detector, gray)
    M, err = field.field_transform(markers)
    found = []
    if M is not None:
        found = pieces.find_pieces(frame, M)
    draw(frame, markers, M, found)
    # TODO: подписать невязку на кадре, чтобы видеть, когда привязка “поехала”
    print(f"метки={sorted(markers)} невязка={err} шашек={len(found)}")
    return frame


def main():
    source = sys.argv[1] if len(sys.argv) > 1 else "0"
    detector = field.make_detector()

    if not source.isdigit():
        frame = cv2.imread(source)
        if frame is None:
            raise SystemExit(f"не читается файл: {source}")
        cv2.imshow("vision", process(frame, detector))
        cv2.waitKey(0)
        return

    cap = cv2.VideoCapture(int(source), cv2.CAP_DSHOW)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
    while True:
        ok, frame = cap.read()
        if not ok:
            break
        cv2.imshow("vision", process(frame, detector))
        key = cv2.waitKey(1) & 0xFF
        if key == 27:
            break
        if key == ord("s"):
            cv2.imwrite("snapshot.png", frame)
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
