
#include <WiFi.h>
#include <FirebaseESP32.h>
//#include <Firebase_ESP_Client.h>

// Firebase プロジェクトの設定
#define WIFI_SSID "abcd"
#define WIFI_PASSWORD "123456789"

// 電磁弁を制御するピン
#define VALVE1_PIN 14  
#define VALVE2_PIN 27
#define VALVE3_PIN 26
#define VALVE4_PIN 25

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

// Firebase Realtime Database のパスを定義
const String userId = "opqrstu";
const String deviceId = "vwxyz"; // Flog
const String valvePath1 = "/devices/" + userId + "/" + deviceId + "/valves/valve1/isOn";
const String valvePath2 = "/devices/" + userId + "/" + deviceId + "/valves/valve2/isOn";
const String valvePath3 = "/devices/" + userId + "/" + deviceId + "/valves/valve3/isOn";
const String valvePath4 = "/devices/" + userId + "/" + deviceId + "/valves/valve4/isOn";

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");

  // ピンのモードを設定
  pinMode(VALVE1_PIN, OUTPUT);
  pinMode(VALVE2_PIN, OUTPUT);
  pinMode(VALVE3_PIN, OUTPUT);
  pinMode(VALVE4_PIN, OUTPUT);

  // Wi-Fi 接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");

  // Firebase 設定
  config.host = "abcdapp-default-rtdb.asia-southeast1.firebasedatabase.app";
  config.api_key = "hijk_lmn";
  auth.user.email = "app@lunchtown.jp"; // Firebase Authentication で登録したユーザーのメールアドレス
  auth.user.password = "123456"; // ユーザーのパスワード

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  if (Firebase.beginStream(firebaseData, ("/devices/" + userId + "/" + deviceId + "/valves").c_str())) {
    Serial.println("Stream started successfully");
  } else {
    Serial.println("Could not start stream");
    Serial.println("REASON: " + firebaseData.errorReason());
  }

  Firebase.setStreamCallback(firebaseData, streamCallback, streamTimeoutCallback);
}

void loop() {
  // メインループでは特に何もしない
}

  void streamCallback(StreamData data) {
  Serial.println("Stream callback");
  Serial.print("Data path: ");
  Serial.println(data.dataPath());
  Serial.print("Data type: ");
  Serial.println(data.dataType());
  Serial.print("Event type: ");
  Serial.println(data.eventType());
  
  String path = data.dataPath();
    
  if (data.dataType() == "json") {
    FirebaseJson& json = data.jsonObject();
    FirebaseJsonData jsonData;
    json.toString(Serial, true);

    if (path == "/valve1") {
      if (json.get (jsonData, "isOn")) {
      bool valveState = jsonData.boolValue;
      digitalWrite(VALVE1_PIN, valveState ? HIGH : LOW);
      Serial.println("Valve 1 state changed.");
      Serial.println(valveState ? "ON" : "OFF");
      } else {
                Serial.println("Failed to get 'isON' for valve1");
      }
    } else if (path == "/valve2") {
      if (json.get(jsonData, "isOn")) {
      bool valveState = jsonData.boolValue;
      digitalWrite(VALVE2_PIN, valveState ? HIGH : LOW);
      Serial.println("Valve 2 state changed.");
      Serial.println(valveState ? "ON" : "OFF");
      } else {
                Serial.println("Failed to get 'isON' for valve2");
      }
    } else if (path == "/valve3") {
      if (json.get(jsonData, "isOn")) {
      bool valveState = jsonData.boolValue;
      digitalWrite(VALVE3_PIN, valveState ? HIGH : LOW);
      Serial.println("Valve 3 state changed.");
      Serial.println(valveState ? "ON" : "OFF");
      } else {
                Serial.println("Failed to get 'isON' for valve3");
      }
    } else if (path == "/valve4") {
      if (json.get(jsonData, "isOn")) {
      bool valveState = jsonData.boolValue;
      digitalWrite(VALVE4_PIN, valveState ? HIGH : LOW);
      Serial.println("Valve 4 state changed.");
      Serial.println(valveState ? "ON" : "OFF");
      } else {
                Serial.println("Failed to get 'isON' for valve4");
      }
    }
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("Stream timeout, reconnecting...");
    Firebase.beginStream(firebaseData, ("/devices/" + userId + "/" + deviceId + "/valves").c_str());//(firebaseData, "devices/userId/deviceId/valves");
  }
}



