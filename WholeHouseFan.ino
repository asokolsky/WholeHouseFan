/**
 *  MQTT Whole House Fan Controller based on ESP8266
 *  
 */
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <PubSubClient.h>
#include "Trace.h"
#include "Fan.h"
//#include "Led.h"
#include "PCB.h"


/** Update these with values suitable for your network. */

const char ssid[] = "........";
const char password[] = "........";
const IPAddress mqtt_server(192, 168, 1, 9);
const uint16_t mqtt_port = 1883;

const char g_szOutTopic[] = "WholeHouse/fan/outTopic";
const char g_szInTopic[] = "WholeHouse/fan/setPoint";

/** how frequently (in ms) to contact MQTT broker */
const unsigned long ulMQTTRefresh = 2000;

//
// nothing to customize below
//

WiFiClient g_espClient;
PubSubClient g_client(g_espClient);
Fan g_fan(BUILTIN_LED);

/**
 * 
 */
void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  DEBUG_PRINT("Connecting to "); DEBUG_PRNTLN(ssid);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DEBUG_PRINT(".");
  }
  randomSeed(micros());

  DEBUG_PRINTLN("");
  DEBUG_PRINT("WiFi connected! "); DEBUG_PRINT("IP address: "); DEBUG_PRNTLN(WiFi.localIP());
}

/**
 * Publish our stuff to MQTT broker
 */
bool publish_mqtt()
{
  // Once connected, publish an announcement...
  int value = analogRead(A0); 
  
  static char msg[50];
  snprintf(msg, 75, "A0=%d", value);
  DEBUG_PRINT("Publish message: "); DEBUG_PRNTLN(msg);
  g_client.publish(g_szOutTopic, msg);  
}
/**
 * Reconnect to MQTT broker
 */
bool reconnect_mqtt() 
{
  // Loop until we're reconnected
  for(int iRetries = 0; (iRetries < 10) && (!g_client.connected()); iRetries++)
  {
    DEBUG_PRINT("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if(g_client.connect(clientId.c_str())) 
    {
      DEBUG_PRINTLN("connected");      
      // Once connected, publish an announcement...
      publish_mqtt();
      // ... and resubscribe
      g_client.subscribe(g_szInTopic);
      return true;
    } 
    DEBUG_PRINT("failed, rc="); DEBUG_PRINT(g_client.state()); DEBUG_PRINTLN(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
  return g_client.connected();
}

/** 
 *  MQTT CallBack 
 */
void callback_mqtt(char* topic, byte* payload, unsigned int len) 
{
  // 0-terminated payload
  char pload[MQTT_MAX_PACKET_SIZE];
  if(len >= MQTT_MAX_PACKET_SIZE)
    return;
  memcpy(pload, payload, len);
  pload[len] = 0;
  DEBUG_PRINT("Message arrived ["); DEBUG_PRINT(topic); DEBUG_PRINT("] ");   DEBUG_PRINTLN(pload);
  // Switch on the LED if an 1 was received as first character
  int val = 0;
  sscanf((const char *)pload, "%d", &val);
  if(val != 0)
    g_fan.go(val);
}


void setup() 
{
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  //analogReadResolution(10);
  //analogWriteResolution(10);
  
  Serial.begin(115200);
  setup_wifi();
  g_client.setServer(mqtt_server, mqtt_port);
  g_client.setCallback(callback_mqtt);
}

void loop() 
{
  if(!g_client.connected()) 
  {
    if(!reconnect_mqtt())
      return;
  }
  g_client.loop();

  // has it been long enough (2 sec) since we talked?
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if(now - lastMsg < ulMQTTRefresh)
    return;  // not long enough
  // it has been long enough (2sec) since we talked!
  lastMsg = now;
  publish_mqtt();
}



