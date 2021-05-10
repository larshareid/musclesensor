/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */
#include <WiFi.h>
#include <WiFiUdp.h>

unsigned long sampleSize = 100;

// WiFi network name and password:
const char * networkName = "BedreNettForAlle";
const char * networkPswd = "Spettasveien9";


//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "165.227.145.19";
const int udpPort = 4001;

//Are we currently connected?
boolean connected = false;
char query[5];
//The udp library class
WiFiUDP udp;
int i = 0;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  //configTime(0, 0, ntpServer);
  //setTime(hr,min,sec,day,mnth,yr);
}

void loop(){
  //only send data when connected
  //t = getTime();
  if(connected){
    //Send a packet
    char insert[] = "%d\n";
    sprintf(query, insert, random(10));
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(query);
    udp.endPacket();
    char insert2[] = "%d\n";
    sprintf(query, insert2, random(10));
    udp.beginPacket(udpAddress,4002);
    udp.printf(query);
    udp.endPacket();
    //delay(100);
}

 /* t = getTime() * 1000;
  char insert[] = "%d,%d,%d\n";
  if(connected){
    //Send a packet
    sprintf(query, insert, i, random(50), -t+millis());
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(query);
    udp.endPacket();
    i++;
}

  delay(1000);
  //Wait for 1 second
  //delay(1);*/
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}
