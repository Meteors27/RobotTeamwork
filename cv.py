import cv2
import numpy as np
import picamera
from time import sleep
import picamera.array


# camera = PiCamera()
# camera.resolution = (1024, 768)

ball_color = 'green'

color_dist = {'red': {'Lower': np.array([0, 60, 60]), 'Upper': np.array([6, 255, 255])},
              'blue': {'Lower': np.array([100, 80, 46]), 'Upper': np.array([124, 255, 255])},
              'green': {'Lower': np.array([35, 43, 35]), 'Upper': np.array([90, 255, 255])},
              }

cap = cv2.VideoCapture(0)
cv2.namedWindow('camera', cv2.WINDOW_AUTOSIZE)

while cap.isOpened():
    ret, frame = cap.read()
    if ret:
        if frame is not None:
            gs_frame = cv2.GaussianBlur(
                frame, (5, 5), 0)                              # 高斯模糊
            hsv = cv2.cvtColor(gs_frame, cv2.COLOR_BGR2HSV)    # 转化成HSV图像
            erode_hsv = cv2.erode(
                hsv, None, iterations=2)                       # 腐蚀 粗的变细
            inRange_hsv = cv2.inRange(
                erode_hsv, color_dist[ball_color]['Lower'], color_dist[ball_color]['Upper'])
            cnts = cv2.findContours(
                inRange_hsv.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
            if cnts != ():
                print(type(cnts))
                c = max(cnts, key=cv2.contourArea)
                rect = cv2.minAreaRect(c)
                box = cv2.boxPoints(rect)
                cv2.drawContours(frame, [np.int0(box)], -1, (0, 255, 255), 2)

            cv2.imshow('camera', frame)
            cv2.waitKey(1)
        else:
            print("无画面")
    else:
        print("无法读取摄像头！")

cap.release()
cv2.waitKey(0)
cv2.destroyAllWindows()
