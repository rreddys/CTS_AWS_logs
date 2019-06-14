
/**
 * LIDARLite I2C Example
 * Author: Garmin
 * Modified by: Shawn Hymel (SparkFun Electronics)
 * Date: June 29, 2017
 * 
 * Read distance from LIDAR-Lite v3 over I2C
 * 
 * See the Operation Manual for wiring diagrams and more information:
 * http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
 */

#include <Wire.h>
#include <MKRGSM.h>
#include <LIDARLite.h>
#include <ArduinoSort.h>
#include "ThingSpeak.h"
#include "secrets.h"

// PIN Number
const char PINNUMBER[]     = SECRET_PIN;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASS;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Globals
LIDARLite lidarLite;
GSMClient client;
GPRS gprs;
GSM gsmAccess;


int cal_cnt = MIN;
int j,a=MIN;
int lidar_val[ MAX ];
int lidar_dist;

void setup()
{
  SERIAL_CONFIGURATION // Initialize serial connection to display distance readings

  START_LIDARLITE // Set configuration to default and I2C to 400 kHz
  CONFIGURE_LIDARLITE // Change this number to try out alternate configurations
  CONNECT_NETWORK
  BEGIN_THINGSPEAK // Initialize ThingSpeak 
}

void loop()
{
  int avg_count = MAX;
  int sum = MIN;
    
  for (j = MIN; j<MAX ; j++)
    {
      lidar_val[ j ] = lidarLite.distance();
      while (lidar_val[ j ] == 0)
           {
            delay(20);
            lidar_val[ j ] = lidarLite.distance();
           }
      delay(1);
    }
    
  sortArray(lidar_val, MAX);
    
  for (int i =9;i;i--)
    {
      if((lidar_val[i] - lidar_val[0])< 5)
      break;
      lidar_val[i] = MIN;
      avg_count --;
    }
            
    for (j = MIN; j<MAX ; j++)
    {
      sum = sum + lidar_val[j] ;
    }
      
   lidar_dist = sum / avg_count;
   Serial.print("lidar_dist =");
   Serial.println(lidar_dist);
     
      
   int x = ThingSpeak.writeField(myChannelNumber, 2, lidar_dist, myWriteAPIKey);
   if(x == 200){
     Serial.println("Channel update successful.");
   }
   else{    
     Serial.println("Problem updating channel. HTTP error code " + String(x));
     if(a++ == 5)
     {
       a=MIN;
       CONNECT_NETWORK
       ThingSpeak.writeField(myChannelNumber, 2, lidar_dist, myWriteAPIKey);
    } 
  }
  delay(5000);   
 }
