
//Library
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <time.h>                   // time() ctime()
#include <WiFiUdp.h>
#include <FirebaseESP8266.h>

//Definisi koneksi wifi
#define WIFI_SSID     "wifik"
#define WIFI_PASSWORD "bebas123"


// Replace with your Firebase project credentials
#define FIREBASE_HOST "https://smart-f8e37-default-rtdb.asia-southeast1.firebasedatabase.app/"  
#define FIREBASE_AUTH "22xwMCqerhkpU929kwfxWWH67cknzcYsb20Qoj3C" 

FirebaseData firebaseData;

//Definisi variabel
float v_Speed;



const int trigPin = D4;
const int echoPin = D3;
int ENA = D5;
int IN1 = D6;
int IN2 = D7;



#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
float Distance;
int Speed;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ENA, OUTPUT); // Sets enable driver motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    yield();
    }
  Serial.println("");
  Serial.print("Terhubung dengan: ");
  Serial.println(WIFI_SSID);

    // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Set the database read timeout
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60); // 1 minute timeout
    Serial.print("wifibro ");

}



void loop() {
  Distance = hcsr04();
  if (Distance < 100){
      digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
    Speed = 0;
        changeSpeed(Speed);

  }
  else if (Distance < 150){
    Speed = map(Distance, 100, 150, 0, 20);
      changeSpeed(Speed);
  }
  else if (Distance > 150) {
    Speed = 20;
      changeSpeed(Speed);
  }
  v_Speed = Distance;
  Firebase.setDouble(firebaseData, "/speed", v_Speed);
  ;

}

void changeSpeed(int kecepatan) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(ENA, kecepatan);
//  for (int i = 0; i < 256; i++) {
//    digitalWrite(ENA, i);
//    delay(20);
//  }
//  
//  for (int i = 255; i >= 0; --i) {
//    digitalWrite(ENA, i);
//    delay(20);
//  }



}


int hcsr04(){
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
   
  duration = pulseIn(echoPin, HIGH);
   
  distanceCm = duration * SOUND_VELOCITY/2;
  
  
  delay (100);
  return(distanceCm);
}
