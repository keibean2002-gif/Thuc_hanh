#define LDR_PIN 2  // D0 nối GPIO2

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  int ldrState = digitalRead(LDR_PIN);

  if (ldrState == HIGH) {
    Serial.println("Khong co anh sang");
  } else {
    Serial.println("Co anh sang");
  }

  delay(500);
}