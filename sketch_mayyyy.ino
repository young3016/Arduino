#include <DHT.h>

#define LIGHT_PIN A0   // 조도 센서 핀 (A0)
#define DHT_PIN 8      // DHT11 센서 핀
#define DHTTYPE DHT11  // DHT11 센서 타입

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
    Serial.begin(9600);  // ESP8266과 동일한 시리얼 통신 속도 설정
    dht.begin();
    pinMode(GAS_DIGITAL_PIN, INPUT);
}

void loop() {
    int light = analogRead(LIGHT_PIN);         // 조도 센서 데이터 읽기
    float temperature = dht.readTemperature(); // 온도 데이터 읽기
    float humidity = dht.readHumidity();       // 습도 데이터 읽기

    // 데이터가 정상적으로 읽혔는지 확인
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // JSON 형식으로 데이터를 시리얼로 전송
    Serial.print("{\"l\": ");
    Serial.print(light);
    Serial.print(", \"l\": ");
    Serial.print(temperature);
    Serial.print(", \"h\": ");
    Serial.print(humidity);
    Serial.println("}");

    delay(1000);  // 1초마다 데이터 전송
}
