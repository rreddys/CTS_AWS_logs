// Use this file to store all of the private credentials 
// and connection details

#define SECRET_PIN ""
//#define SECRET_GPRS_APN "imis/internet"
#define SECRET_GPRS_APN "airtelgprs.com"
#define SECRET_GPRS_LOGIN ""
#define SECRET_GPRS_PASS ""

#define SECRET_CH_ID 767587      // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "I3E8VORWS2ZXN0H9"   // replace XYZ with your channel write API Key 

#define MIN  0
#define MAX  10

#define SERIAL_CONFIGURATION    Serial.begin(9600);
#define START_LIDARLITE         lidarLite.begin(0, true);
#define CONFIGURE_LIDARLITE     lidarLite.configure(0);
