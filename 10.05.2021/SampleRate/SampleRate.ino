#include <WiFi.h>
#include <WiFiUdp.h>

unsigned long sampleSize = 100;

// WiFi network name and password:
const char * networkName = "227528@usn.no";
const char * networkPswd = "Ibt427zu";


//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "165.227.145.19";
const int udpPort = 4001;
const long int sampleNum=100000;

const int UDP_PACKET_SIZE= 48;
byte packetBuffer[ UDP_PACKET_SIZE];

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
  //displayMuskel();
  delay(5000);
  udpWrite();
  delay(5000);
  samplingRaw();
  delay(5000);
  udpTestRandom();
  delay(5000);
  udpTestSensor();
  
}

void samplingRaw() {
  Serial.println("Starting samplingRaw...");
  long int start = millis();
  for(int i=0;i<sampleNum;i++){
    int val = analogRead(4);
  }
 long int slutt = millis();
 long int interval = slutt - start;
 Serial.print("Sampling raw took ms: ");
 Serial.println(interval);
}

void udpWrite() {
  Serial.println("Starting udpWrite...");
  long int start = millis();
 for(int i=0;i<sampleNum;i++){
  memset(packetBuffer, 0, UDP_PACKET_SIZE);
  sprintf((char*)packetBuffer,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",random(10),random(10),random(10),random(10),random(10),random(10),random(10),random(10),random(10),random(10));
  udp.beginPacket(udpAddress,udpPort);
  udp.write(packetBuffer,UDP_PACKET_SIZE);
  udp.endPacket();
  }
  long int slutt = millis();
  long int interval = slutt - start;
  Serial.print("Sampling udp write took ms: ");
  Serial.println(interval);
 }

void udpBulk() {
    Serial.println("Starting udpBulk...");
  long int start = millis();
 for(int i=0;i<sampleNum;i++){
  memset(packetBuffer, 0, UDP_PACKET_SIZE);
  sprintf((char*)packetBuffer,"%d",random(10));
  udp.beginPacket(udpAddress,udpPort);
  udp.write(packetBuffer,UDP_PACKET_SIZE);
  udp.endPacket();
  }
  long int slutt = millis();
  long int interval = slutt - start;
  Serial.print("Sampling udp write took ms: ");
  Serial.println(interval);
}


void udpTestRandom(){
  //only send data when connected
  Serial.println("Starting udpTestRandom...");
  char insert[] = "%d\n";
  // insert[5] = analogRead(4);
  if(connected){
    //Send a packet
    long int start = millis();
    for(int i=0;i<sampleNum;i++){
    char insert[] = "%d\n";
    sprintf(query, insert, random(10));
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(query);
    udp.endPacket();
    //delayMicroseconds(5);
    }
    long int slutt = millis();
    long int interval = slutt - start;
    Serial.print("UDP test random took ms: ");
    Serial.println(interval);
  }
}

void udpTestSensor(){
  //only send data when connected
  Serial.println("Starting udpTestSensor...");
  long int start = millis();
  if(connected){
    //Send a packet
    for(int i=0;i<sampleNum;i++){
    char insert[] = "%d\n";
    sprintf(query, insert, analogRead(4));
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(query);
    udp.endPacket();
    }
    long int slutt = millis();
    long int interval = slutt - start;
    Serial.print("UDP test sensor took ms : ");
    Serial.println(interval);
  }
}

void displayMuskel() {
  int musk = analogRead(4);
  Serial.println(musk);
  delay(50);
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
