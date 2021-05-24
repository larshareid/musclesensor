#include <WiFi.h>
#include <WiFiUdp.h>

//Fill in the network the EPS32 should connect to
const char * networkName = "***";
const char * networkPswd = "***";

//Server address for hadling of data
const char * udpAddress = "165.227.145.19";

//Change port accordingly to name of the sensor. sensorX = port 400X,  ex sensor 7 should have port 4007
const int udpPort = 4001;
char query[10];

//Set correct inputs used for each compnent used on the EPS32
int sensor = 2;
int potmeter = 4;

boolean connected = false;
WiFiUDP udp;
int i = 0;

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

  for(;;){
    long int start = millis();
    if(check==0){
    for(int i = 0; i<100000; i++){
    int num = analogRead(sensor);
    int pot = analogRead(potmeter);
    //sends data to stack. If queue is full, the sensor will wait to send information until 
    //room is available.
    xQueueSend(Valqueue, &num, portMAX_DELAY);
    xQueueSend(Potqueue, &pot, portMAX_DELAY);
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

  for(;;){
    int val1,pot1;
    xQueueReceive(Valqueue, &val1, portMAX_DELAY);
    xQueueReceive(Potqueue, &pot1, portMAX_DELAY);

    char insert[] = "%d,%d\n";
    sprintf(query,insert,val1,pot1);
    udp.beginPacket(udpAddress,udpPort);
    udp.printf(query);
    udp.endPacket();
    
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
