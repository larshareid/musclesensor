#include <WiFi.h>
#include <WiFiUdp.h>
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

const char * networkName = "***";
const char * networkPswd = "***";
const char * udpAddress = "***";
const int udpPort = 4001;
char query[50];
boolean connected = false;
WiFiUDP udp;

TaskHandle_t Task1;
TaskHandle_t Task2;
QueueHandle_t Valqueue;
QueueHandle_t Potqueue;

void setup() {
  Serial.begin(115200); 
  connectToWiFi(networkName, networkPswd);
  Valqueue = xQueueCreate(100, sizeof(int));
  Potqueue = xQueueCreate(100, sizeof(int));

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}


void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  int check = 0;
  int interval = 99;
  int samples = 10000000;

  for(;;){
    long int start = millis();
    long int currTime = 0;
    if(check==0){
    for(int i = 0; i<samples;){
      if ( micros() >= currTime + interval){
        currTime = micros();
        int num = analogRead(34);
        int pot = analogRead(35);
        //Serial.println(pot);
        xQueueSend(Valqueue, &num, portMAX_DELAY);
        xQueueSend(Potqueue, &pot, portMAX_DELAY);
        i++;
      }
    }
    check=1;
    }
    long int slutt = millis();
    long int interval = slutt - start;
    Serial.print("Sampling took ms: ");
    Serial.println(interval);
    delay(5000);
  } 
}


void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  int j = 1;
  int k = 0;

  for(;;){
    int val1,pot1,val2,pot2,val3,pot3,val4,pot4,val5,pot5,val6,pot6,val7,pot7,val8,pot8;
    xQueueReceive(Valqueue, &val1, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot1, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val2, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot2, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val3, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot3, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val4, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot4, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val5, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot5, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val6, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot6, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val7, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot7, portMAX_DELAY);
    //delayMicroseconds(10);
    xQueueReceive(Valqueue, &val8, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot8, portMAX_DELAY);

    char insert[] = "%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n%d,%d,%d\n";
    sprintf(query,insert,j,val1,pot1,j+1,val2,pot2,j+2,val3,pot3,j+3,val4,pot4,j+4,val5,pot5,j+5,val6,pot6,
    j+6,val7,pot7,j+7,val8,pot8,j+6,val7,pot7,j+7,val8,pot8);
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(query);
    udp.endPacket();
    j=j+8;
    k++;
    if(k >= 2000){
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
    k = 0;
    }
  }
}

void loop() {
  vTaskDelete (NULL);
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
