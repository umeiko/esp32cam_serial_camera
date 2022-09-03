# esp32-Cam模块 串口摄像头
通过串口控制拍摄并传输图像信息
## 安装使用
1. 安装所需的库
```
pip install -r requirements.txt
```

2. 将 serialCamArduino 中的代码烧录到esp32-cam模块中。
```
PORT = "COMXX"  # 换成正确的串口号
```

3. 在任务管理器中查看模块的串口号，并在 main.py 中修改。
3. 运行脚本，脚本会拍摄一张照片并保存。