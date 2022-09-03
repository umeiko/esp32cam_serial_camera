import serial
import time
from PIL import Image
import io

PORT = "COM15"    # 换成正确的串口号
BAUDRATE = 500000 # 尝试不同的波特率

def init_camera():
	"""启动摄像头模块"""
	cam_con = serial.Serial(PORT)
	cam_con.baudrate = BAUDRATE
	time.sleep(0.1)
	cam_con.write(b'e')
	time.sleep(0.1)
	msg = cam_con.read_all()
	print(msg)
	return cam_con

def deinit_camera(cam_con):
	"""关闭摄像头模块"""
	cam_con.write(b'w')
	time.sleep(0.001)
	print(cam_con.read_all())
	cam_con.close()

def preheat_camera(cam_con, times=2):
	"""预热摄像头模块"""
	for _ in range(times):
		cam_con.write(b't')
		time.sleep(1)
		msg = cam_con.read_all()
		print(msg)


def take_a_photo(cam_con):
	"""拍摄一张照片"""
	cam_con.write(b'v')
	a = b""
	while True:
		cam_con.write(b'r')
		time.sleep(0.001)
		temp = cam_con.read_all()
		if temp:
			a += temp
		else:
			break
	print(len(a))
	img = a[:-4]
	picture_stream = io.BytesIO(img)
	picture = Image.open(picture_stream)
	return picture

def save_photo(picture, name:str=None):
	if name == None:
		name = f"{time.time_ns()}"
	picture.save(f"{name}.jpg")

if __name__ == "__main__":
	cam = init_camera()
	try:
		preheat_camera(cam)
		picture = take_a_photo(cam)
		save_photo(picture)
	except Exception as e:
		print(e)
	finally:
		deinit_camera(cam)