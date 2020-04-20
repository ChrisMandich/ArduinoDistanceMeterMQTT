/*
    This sketch establishes wifi connection to specified SSID / password.
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

const char* sensor_name = "MQTTDistSensor";

// defines pins numbers
const int echoPin_ato_dist = 5;  //D1
const int trigPin_ato_dist = 4;  //D2
const int input_pin = 13;  //D7

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
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

void setup_sensor(){
  pinMode(trigPin_ato_dist, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin_ato_dist, INPUT); // Sets the echoPin as an Input  
}

void setup_mqtt(){
  client.setServer(mqtt_server, 1883);
  connect_mqtt();
}

void setup_liquid_sensor(){
  pinMode(input_pin, INPUT);
}

void connect_mqtt(){
  // Loop until we're reconnected
  while (!client.connected()) {
    if(client.connect(sensor_name, mqtt_user, mqtt_password)){
      Serial.println();
      Serial.println();
      Serial.println("MQTT Connection Success");
    }
    else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void get_liquid_sensor_status(){
  int val;
  char* mqtt_topic = "topic_liquid_sensor";
  val = digitalRead(input_pin);

  // Print Update to console.
  Serial.print("Liqud Sensor Status: ");
  Serial.println(String(val));

  // Send Update to MQTT
  if(val == 1){
    client.publish(mqtt_topic, "true", true);
  }
  else{
    client.publish(mqtt_topic, "false", true);
  }
}

void sensor_ato_dist() {
  // Define variables
  long duration;
  int payload_ato_percent;
  float payload_ato_dist;
  float ato_resorvoir_height = 33.55; // 14 inch - 1 cm converted to cm. 
  char* topic_ato_dist = "topic_ato_dist_perc";

  // Clears the trigPin
  digitalWrite(trigPin_ato_dist, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin_ato_dist, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_ato_dist, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin_ato_dist, HIGH);

  // Calculating the distance
  payload_ato_dist = duration * 0.034 / 2;
  if (payload_ato_dist < ato_resorvoir_height){
    payload_ato_percent = 100 - ((payload_ato_dist * 100) / ato_resorvoir_height);
    // Prints the distance on the Serial Monitor
    Serial.print("ATO Distance: ");
    Serial.print(payload_ato_dist);
    Serial.println(" CM");
    Serial.print("ATO Percent: ");
    Serial.print(payload_ato_percent);
    Serial.println(" %");
    client.publish(topic_ato_dist, String(payload_ato_percent).c_str(), true);
  }
  else{
    Serial.println("Distance to far for measurement.");
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  setup_mqtt();
  setup_sensor();
  setup_liquid_sensor();
}

void loop() {
  if (!client.connected()) {
    connect_mqtt();
  }
  Serial.println("");
  Serial.println("");
  Serial.println("XXXXXXXXXXXXXX");  
  Serial.println("XXXXXXXXXXXXXX");  
  Serial.println("");
  Serial.println("Loop Initialized.");
  
  sensor_ato_dist();
  get_liquid_sensor_status();
  
  delay(15000);
  Serial.println("Loop Completed.");  
}
