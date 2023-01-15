## 模块使用
- HC-05蓝牙模块；
- L298N电机驱动;
- SG-90舵机*2;
- MG310电机;
- 3S 1100maH锂电池；
- LED模块；
- CH340 USB转TTL模块；
- ESP8266-01Swifi模块；
- A4988电机驱动*2；
- Nema17 步进电机*2；
- 0.96寸OLED显示屏（128×64 I2C）
- 剪线钳DL383005；
- Arduino uno r3板*2；
## 核心算法
在本次硬件设计中，所使用的对本项目较为重要的代码有：
1.PID位移式算法实现电机的闭环控制。
2.蓝牙的AT指令调试相关代码。
3.ESP8266-01S的AT指令设置WiFi信息，因为某些原因，这些指令需要以代码形式烧录进入控制板。
4.手机APP上蓝牙权限申请，WiFi连接相关，以及单片机接收解码相关代码。
