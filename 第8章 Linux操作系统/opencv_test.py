
import cv2
import numpy as np		# 科学计算库

cap=cv2.VideoCapture(0)
while(1):
    ret,frame = cap.read()					# 获取图像
    hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)   # 转换到 HSV
    lower_blue = np.array([110,50,50])		# 设定蓝色阈值下限
    upper_blue = np.array([150,255,255])	# 设定蓝色阈值的上限
    mask = cv2.inRange(hsv, lower_blue, upper_blue) 	# 得到所有符合条件的像素
    res = cv2.bitwise_and(frame, frame, mask=mask)		# 与运算，得到的结果
    cv2.imshow('frame', frame)	# 显示原始图片
    cv2.imshow('mask', mask)	# 显示掩码
    cv2.imshow('res', res)		# 显示结果
    k = cv2.waitKey(500)&0xFF		# 等待
    if k == 27:   # 按下 Esc 键退出程序
        break
cv2.destroyAllWindows()			# 关闭窗口



