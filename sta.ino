#include <Wire.h>       //Wire库提供了 Arduino上的I2C总线通信功能,用于有线连接mpu6050
#include <WiFiUdp.h>    //udp库
#include <ESP8266WiFi.h>//WiFi库
#include <MPU6050.h>    //mpu6050库

//创建一个mpu6050对象
MPU6050 mpu;

// 创建UDP实例
WiFiUDP udp;

// WiFi的相关信息
const char* ssid = "esp8266";       //AP的WiFi名
const char* password = "12345678";  //WiFi密码
const char* apIP = "192.168.4.1";   //AP的ip地址

void setup() {
  Serial.begin(115200);  //设置波特率，用于串口通信

  // 连接到AP
  WiFi.mode(WIFI_STA);  //设置为sta模式
  WiFi.begin(ssid, password);
  Serial.println("连接到WiFi...");

  // 等待连接
  while (WiFi.status() != WL_CONNECTED) { //WiFi.status()返回当前WiFi连接的状态
    delay(1000);
    Serial.println("连接中...");
  }
  Serial.println("已连接到WiFi");

  // 初始化MPU6050
  Wire.begin();
  mpu.initialize();
}

void loop() {
  // 读取MPU6050数据
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // 发送传感器数据到AP
  sendSensorDataToAP(ax, ay, az);

  delay(80); // 延迟80毫秒
}

void sendSensorDataToAP(int16_t ax, int16_t ay, int16_t az) {
  // 发送数据包到AP
  udp.beginPacket(apIP, 1234); //开始，1234是选择的ap端口号
  udp.write((uint8_t*)&ax, sizeof(ax));
  udp.write((uint8_t*)&ay, sizeof(ay));
  udp.write((uint8_t*)&az, sizeof(az));
  udp.endPacket();             //结束
}
