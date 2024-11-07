#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

const char* ssid = "iptime8503";
const char* password = "IIClab8503";
const char* mqtt_server = "192.168.0.7";
const char* mqtt_topic = "/home/sensors";

// 소프트웨어 시리얼 설정 (GPIO2을 RX로 설정)
SoftwareSerial softSerial(2, -1);  // GPIO2을 RX로 설정, TX는 사용 안 함

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(9600);       // USB 디버깅용 시리얼
    softSerial.begin(9600);   // 아두이노로부터 데이터 수신용
    setup_wifi();
    client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // 소프트웨어 시리얼을 통해 아두이노로부터 데이터 읽기
    if (softSerial.available() > 0) {
        String sensorData = softSerial.readStringUntil('\n');  // 아두이노로부터 받은 데이터
        sensorData.trim();  // 공백 제거

        // MQTT로 전송
        client.publish(mqtt_topic, sensorData.c_str());
        Serial.print("Published message: ");
        Serial.println(sensorData);
    }

    delay(1000);  // 1초마다 데이터 전송
}








#include <DHT.h>

#define LIGHT_PIN A0   // 조도 센서 핀 (A0)
#define GAS_ANALOG_PIN A1  // 가스 센서 아날로그 핀 (A1)
#define GAS_DIGITAL_PIN 9  // 가스 센서 디지털 핀 (디지털 9번)
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
    int gasAnalog = analogRead(GAS_ANALOG_PIN); // 가스 센서 아날로그 데이터 읽기
    int gasDigital = digitalRead(GAS_DIGITAL_PIN); // 가스 센서 디지털 데이터 읽기
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
    Serial.print(", \"g\": ");
    Serial.print(gasAnalog);
    Serial.print(", \"gd\": ");
    Serial.print(temperature);
    Serial.print(", \"h\": ");
    Serial.print(humidity);
    Serial.println("}");

    delay(1000);  // 1초마다 데이터 전송
}













