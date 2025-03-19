#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "Robot Car";
const char* password = "123456789";
String data = "";
const int trigPin = D4; 
const int echoPin = D5; 
const int temperaturePin = A0; 
const int metalSensorPin1 = D6; 
const int metalSensorPin2 = D7; 
void setup()
{
 pinMode(D0, OUTPUT);
 pinMode(D1, OUTPUT);
 pinMode(D2, OUTPUT);
 pinMode(D3, OUTPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 digitalWrite(LED_BUILTIN, HIGH);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(metalSensorPin1, INPUT);
 pinMode(metalSensorPin2, INPUT);
 Serial.begin(115200);
 connect_wifi();
 server.begin();
}
void loop()
{
 client = server.available();
 if (!client) return;
 data = getclientdata();
 if (data == "F")
 {
 int distance = getDistance();
 Serial.print("Distance: ");
 Serial.println(distance);
 float temperature = getTemperature();
 Serial.print("Temperature: ");
 Serial.println(temperature);
 int metalValue1 = digitalRead(metalSensorPin1);
 int metalValue2 = digitalRead(metalSensorPin2);
 
 if (distance < 30 && temperature > 30) 
 {
 stopCar();
 }
 else
 {
 digitalWrite(D0, HIGH);
 digitalWrite(D1, LOW);
 digitalWrite(D2, HIGH);
 digitalWrite(D3, LOW);
 }
 }
 else if (data == "B")
 {
 digitalWrite(D0, LOW);
 digitalWrite(D1, HIGH);
 digitalWrite(D2, LOW);
 digitalWrite(D3, HIGH);
 Serial.println("Metal detected!");}
 else if (data == "S")
 
{
 stopCar();
 
}
 else if (data == "R")
 
{
 digitalWrite(D0, HIGH);
 digitalWrite(D1, LOW);
 digitalWrite(D2, LOW);
 digitalWrite(D3, LOW);
 
}
 else if (data == "L")
 
{
 digitalWrite(D0, LOW);
 digitalWrite(D1, LOW);
 digitalWrite(D2, HIGH);
 digitalWrite(D3, LOW);
 
}
 else
 
{
 stopCar();
 
}
}
void stopCar() {
 digitalWrite(D0, LOW);
 digitalWrite(D1, LOW);
 digitalWrite(D2, LOW);
 digitalWrite(D3, LOW); }
int getDistance() {
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 int duration = pulseIn(echoPin, HIGH);
 int distance = duration * 0.034 / 2;
 return distance; }
 float getTemperature() {
 int rawValue = analogRead(temperaturePin);
 float voltage = (rawValue / 1024.0) * 3.3; 
 float temperature = (voltage - 0.6) * 100.0; 
 return temperature;
}
void connect_wifi()
{
 WiFi.begin(ssid, password);
 Serial.println("Connecting...");
 while (WiFi.status() != WL_CONNECTED)
 {
 delay(200);
 Serial.print(".");
 }
 Serial.println("\n\n");
 Serial.println("WiFi connected");
 Serial.print("NodeMCU Local IP: ");
 Serial.println(WiFi.localIP());
}
String getclientdata()
{
 while (!client.available())
 {
 delay(1);
 }
 String data = client.readStringUntil('\r');
 Serial.println("");
 Serial.print("INCOMING DATA: ");
 Serial.println(data);
 Serial.print("length: ");
 Serial.println(data.length());
 Serial.println("END OF DATA");
 data.remove(0, 12);
 data.remove(1, 9);
 Serial.print("Parsed data:");
 Serial.println(data);
 Serial.print("parsed data length: ");
 Serial.println(data.length());
 return data;
}
