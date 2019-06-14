
#include <MKRGSM.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <LIDARLite.h>
#include <ArduinoSort.h>
#include "ArduinoLowPower.h"


// PIN Number
const char PINNUMBER[]     = SECRET_PIN;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASS;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

GSMClient client;
GPRS gprs;
GSM gsmAccess;
LIDARLite lidarLite;


long int dday,dmonth,dyear;
long int thours,tminutes,tseconds;

//int a,b,c,d,e,f,g,h,i,k;
int j;
int lidar_dist;
int val_5 = 0;
int lidar_val[ 10 ];
int lidar_5_val[ 5 ];
char string[] = "val1=";
char string1[] = "val2=";
char string2[] = "val3=";
char string3[] = "val4=";
char string4[] = "val5=";
char tank[] = ",tank=1,";
char seqn[] = "seq.no=";
int a =0;
int seq_no =1;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations
  lidarLite.write(0x65,0x01);
  gsmAccess.lowPowerMode();
 }

void loop() {
 
     int avg_count = 10;
     int sum = 0;
  
     LowPower.sleep(1000);

     if(a++>=60)
     {
      lidarLite.write(0x65,0x00);            //enable receiver circuit
      lidarLite.distance();
      delay(20);
     
    
     for (j = 0; j<10 ; j++)
     {
      lidar_val[ j ] = lidarLite.distance();
      while (lidar_val[ j ] == 0)
           {
            delay(20);
            lidar_val[ j ] = lidarLite.distance();
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
       Serial.println("connecting to the network");
       gsmAccess.noLowPowerMode();
       boolean connected = false;
      while (!connected) {
      if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
          (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
        connected = true;
      } else {
        delay(1000);
      }
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
    //Serial.println("updating cloud");
    /*for(val_5 = 0;val_5 < 5; val_5++)
    {*/
    val_5 = 0;
     String lidarval = String(lidar_5_val[ val_5++]);
       String lidarval1 = String(lidar_5_val[ val_5++]);
        String lidarval2 = String(lidar_5_val[ val_5++]);
         String lidarval3 = String(lidar_5_val[ val_5++]);
          String lidarval4 = String(lidar_5_val[ val_5++]);
          
      String nxtval = String(string + lidarval + string1 + lidarval1 + string2 + lidarval2 + string3 + lidarval3 + string4 + lidarval4 + tank);
      //String nxtval = String(string + lidarval + tank + seqn + seq_num);
      int x = ThingSpeak.writeField(myChannelNumber, 1, nxtval, myWriteAPIKey);
  if(x == 200){
    //Serial.println("Channel update successful.");
  }
  else{
   // Serial.println("Problem updating channel. HTTP error code " + String(x));
    boolean connected = false;
   while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      //Serial.println("Not connected");
      delay(1000);
    }
   }
    delay(12000);
    x = ThingSpeak.writeField(myChannelNumber, 1, nxtval, myWriteAPIKey);
      
    gsmAccess.lowPowerMode();
    val_5 = 0; 
    seq_no = 1;
   
   }
    a = 0;
   }}
    else
   {
      lidarLite.write(0x65,0x00);            //enable receiver circuit
      delay(100);
      lidarLite.distance();
      lidarLite.write(0x65,0x01);
   }
    
}
