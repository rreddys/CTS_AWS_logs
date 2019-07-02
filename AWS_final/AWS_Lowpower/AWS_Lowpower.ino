/*
  AWS IoT GSM

  This sketch securely connects to an AWS IoT using MQTT over GSM/3G.
  It uses a private key stored in the ATECC508A and a public
  certificate for SSL/TLS authetication.

  It publishes a message every 5 seconds to arduino/outgoing
  topic and subscribes to messages on the arduino/incoming
  topic.

  The circuit:
  - MKR GSM 1400 board
  - Antenna
  - SIM card with a data plan
  - LiPo battery

  This example code is in the public domain.
*/

#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <MKRGSM.h>
#include <LIDARLite.h>
#include <ArduinoSort.h>

#include "ArduinoLowPower.h"
#include "arduino_secrets.h"

/////// Enter your sensitive data in arduino_secrets.h
const char pinnumber[]     = SECRET_PINNUMBER;
const char gprs_apn[]      = SECRET_GPRS_APN;
const char gprs_login[]    = SECRET_GPRS_LOGIN;
const char gprs_password[] = SECRET_GPRS_PASSWORD;
const char broker[]        = SECRET_BROKER;
const char* certificate    = SECRET_CERTIFICATE;

GSM gsmAccess;
GPRS gprs;

GSMClient     gsmClient;            // Used for the TCP socket connection
BearSSLClient sslClient(gsmClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);
LIDARLite lidarLite;

unsigned long lastMillis = 0;
int lidar_val[ MAX ];
int lidar_dist;
int cal_cnt = MIN;
int j,a=MIN;
String nxtval;
int lidar_5_val[ 5 ];
int val_5 = 0;
char string[] = "val1=";
char string1[] = "val2=";
char string2[] = "val3=";
char string3[] = "val4=";
char string4[] = "val5=";
char tank[] = ",tank=1,";

int tue=1;

void setup() {
  /*Serial.begin(115200);
  while (!Serial);*/
  pinMode(LED_BUILTIN, OUTPUT);
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations
  lidarLite.write(0x65,0x01);
  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);

  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  //
  // mqttClient.setId("clientId");

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);
  gsmAccess.lowPowerMode();
}

void loop() {

  int avg_count = 10;
     int sum = 0;
  
     LowPower.sleep(1000);

     if(a++>=30)
     {
      lidarLite.write(0x65,0x00);            //enable receiver circuit
     // lidarLite.distance();
      delay(20);
     
    
     for (j = 0; j<10 ; j++)
     {
      lidar_val[ j ] = tue++;//lidarLite.distance();
      while (lidar_val[ j ] == 0)
           {
            delay(20);
            lidar_val[ j ] = tue++;//lidarLite.distance();
           }
      delay(1);
     }
     lidarLite.write(0x65,0x01);            //disable receiver circuit
   
     sortArray(lidar_val, 10);
      
      for (int i =9;i;i--)
      {
        if((lidar_val[i] - lidar_val[0])< 5)
        break;
        lidar_val[i] = 0;
        avg_count --;
      }

      
      for (j = 0; j<10 ; j++)
      {
        sum = sum + lidar_val[j] ;
      }
      
     lidar_dist = sum / avg_count;
    
     lidar_5_val[ val_5++ ] = lidar_dist;
     if (val_5 == 5)
     {
      if (gsmAccess.status() != GSM_READY || gprs.status() != GPRS_READY) {
    connectGSM();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();
       
    val_5 = 0;
     String lidarval = String(lidar_5_val[ val_5++]);
     String lidarval1 = String(lidar_5_val[ val_5++]);
     String lidarval2 = String(lidar_5_val[ val_5++]);
     String lidarval3 = String(lidar_5_val[ val_5++]);
     String lidarval4 = String(lidar_5_val[ val_5++]);
          
     nxtval = String(string + lidarval + string1 + lidarval1 + string2 + lidarval2 + string3 + lidarval3 + string4 + lidarval4 + tank);
      
     publishMessage(); 
    gsmAccess.lowPowerMode();
    val_5 = 0; 
    //seq_no = 1;
   
   }
    a = 0;
   }
    else
   {
      lidarLite.write(0x65,0x00);            //enable receiver circuit
      delay(100);
      lidarLite.distance();
      lidarLite.write(0x65,0x01);
   }
}

unsigned long getTime() {
  // get the current time from the GSM module
  return gsmAccess.getTime();
}

void connectGSM() {
  Serial.println("Attempting to connect to the cellular network");

  while ((gsmAccess.begin(pinnumber) != GSM_READY) ||
         (gprs.attachGPRS(gprs_apn, gprs_login, gprs_password) != GPRS_READY)) {
    // failed, retry
    Serial.print(".");
    delay(1000);
  }

  Serial.println("You're connected to the cellular network");
  Serial.println();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe("arduino/incoming");
}

void publishMessage() {
  Serial.println("Publishing message");
  
  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage("arduino/outgoing");
  mqttClient.print("Lidar distance in cm = ");
  mqttClient.print(nxtval);
  mqttClient.endMessage();
}

void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();

  Serial.println();
}
