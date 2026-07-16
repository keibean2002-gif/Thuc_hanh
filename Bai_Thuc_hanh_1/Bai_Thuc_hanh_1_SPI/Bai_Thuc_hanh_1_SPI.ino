#include <SPI.h>
#define SCK   4
#define MOSI  6
#define MISO  5
#define CS    7
SPIClass spi(FSPI);
// Ghi thanh ghi
void writeReg(byte reg, byte data)
{
  digitalWrite(CS, LOW);
  spi.transfer(reg);
  spi.transfer(data);
  digitalWrite(CS, HIGH);
}
// Đọc 16 bit
int16_t read16(byte reg)
{
  digitalWrite(CS, LOW);
  spi.transfer(reg | 0x80);
  byte l = spi.transfer(0);
  byte h = spi.transfer(0);
  digitalWrite(CS, HIGH);
  return (int16_t)((h << 8) | l);
}
void setup()
{
  Serial.begin(115200);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  spi.begin(SCK, MISO, MOSI, CS);
  // Bật Accelerometer BMI160
  writeReg(0x7E, 0x11);
  delay(100);
  // Range ±2g
  writeReg(0x41, 0x03);
  delay(100);
  Serial.println("BMI160 READY");
}
void loop()
{
  float ax = 0;
  float ay = 0;
  float az = 0;
  // Lấy trung bình 20 mẫu
  for(int i = 0; i < 20; i++)
  {
    ax += read16(0x12);
    ay += read16(0x14);
    az += read16(0x16);
    delay(5);
  }
  ax /= 20.0;
  ay /= 20.0;
  az /= 20.0;
  // đổi sang m/s2
  ax = ax * 9.80665 / 16384.0;
  ay = ay * 9.80665 / 16384.0;
  az = az * 9.80665 / 16384.0;
  // ===== HIỆU CHỈNH OFFSET CỦA MODULE =====
  ax += 0.39;
  ay += 0.71;
  az -= 0.30;
  Serial.print("AX: ");
  Serial.print(ax,3);
  Serial.print("  AY: ");
  Serial.print(ay,3);
  Serial.print("  AZ: ");
  Serial.print(az,3);
  Serial.println(" m/s2");
  delay(1000);
}
