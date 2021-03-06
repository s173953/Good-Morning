/*
 * Remember to include libraries, including Adafruit Unified Sensors (search "Unified Sensors" in Library Management and chose the second to last)
 * Remeber to update wifi password
 * 
 * Hardware setup
 * Using an EDP8266 connect following
 * 3.3V to DHT
 * GND to DHT
 * D7 to DHT
 * D4 to red in RGB LED
 * D3 to green in RGB LED
 * D2 to blue in RGB LED
 * A0 to Aout in CO2 sensor
 * 3.3V to VCC in CO2 sensor
 * GND to GND in CO2 sensor
 * 3.3V to VCC in button
 * GND to resistor in button
 * D8 to output from putton
 * 
 * Software setup
 * Determine how long your sunrise will last in    setupDuration(min) (insert number of minutes)
 * 
 * 
 * 
 * 
 */



#include <ESP8266WiFi.h>
#include <time.h>
#include "DHTesp.h"
#include "ThingSpeak.h"

// for pins
const byte p_red = 2; // pin D4 = 2
const byte p_green = 0; // pin D3 = 0 
const byte p_blue = 4;  // pin D2 = 4
const byte p_button = 15; // pin D8 = 15


//for dht22 sensor
DHTesp dht;

float hum;  //Stores humidity value
float temp; //Stores temperature value


byte red = 255;
byte blue = 255;
byte green = 255;

//SunriseArray comes from printSunriseArray method. Remember not to declare rows (for some reason?) when declaring multidimensionel arrays
const byte SunriseArrayRed[]= {254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
const byte SunriseArrayGreen[] = {                                                                                                                                                          254,254,254,254,253,253,253,253,252,252,252,252,251,251,251,251,250,250,250,250,249,249,249,249,248,248,248,248,247,247,247,247,246,246,246,246,245,245,245,245,244,244,244,244,243,243,243,243,242,242,242,242,241,241,241,241,240,240,240,240,239,239,239,239,238,238,238,238,237,237,237,237,236,236,236,236,235,235,235,235,234,234,234,234,233,233,233,233,232,232,232,232,231,231,231,231,230,230,230,230,229,229,229,229,228,228,228,228,227,227,227,227,226,226,226,226,225,225,225,225,224,224,224,224,223,223,223,223,222,222,222,222,221,221,221,221,220,220,220,220,219,219,219,219,218,218,218,218,217,217,217,217,216,216,216,216,215,215,215,215,214,214,214,214,213,213,213,213,212,212,212,212,211,211,211,211,210,210,210,210,209,209,209,209,208,208,208,208,207,207,207,207,206,206,206,206,205,205,205,205,204,203,202,201,200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150};
const byte SunriseArrayBlue[] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,254,253,253,252,252,251,251,250,250,249,249,248,248,247,247,246,246,245,245,244,244,243,243,242,242,241,241,240,240,239,239,238,238,237,237,236,236,235,235,234,234,233,233,232,232,231,231,230,230,229,229,228,228,227};

int sunriseCounter = 0;

int t_sunrisedelay = 30 ; // default: sequence length 7.65 seconds
//for time handling
int timezone = 1 * 3600;
int dst = 0;

time_t now = time(nullptr);
struct tm* p_tm = localtime(&now);

// for wifi
//school network
const char* ssid = "TP-LINK_8E7A";
const char* password = "41308561";

//Forrest network
//const char* ssid = "TP-LINK_4182";
//const char* password = "80901785";

//const char* ssid= "Slaveslam";
//const char* password = "17091999";

WiFiClient  client;

// for alarm
byte inputHour, inputMin, oldHour = 0, oldMin= 0;
boolean colonRead = false;

// for Thing Speak
#define SECRET_CH_ID 677914           //Test channel for counting
#define SECRET_READ_APIKEY "BKS4IUOV3DV23RYE" //API Key for Test channel (READ)
#define SECRET_WRITE_APIKEY "0FTRUS9NJYG756BY" //API Key for Test channel WRITE

boolean runblindStatus = 0;

void setup() {
    
    pinMode(p_red, OUTPUT);
    pinMode(p_green, OUTPUT);
    pinMode(p_blue, OUTPUT);
    pinMode(p_button, INPUT);
    pinMode(A0,INPUT);
  
  //LED is active low, therefore setting all output high to turn off
    analogWrite(p_red,1024); //times four when using the ESP
    analogWrite(p_green,1024);
    analogWrite(p_blue, 1024);
  
    Serial.begin(115200);
  
    wifiSetup();
  
    // sunrise duration
    //setDuration(30);
  
    timeSetup();

    dht.setup(13, DHTesp::DHT22); //from ESP8266 example
    
    // init ThingSpeak
    ThingSpeak.begin(client);  // Initialize ThingSpeak
    resetAll();

    // to set alarm
    Serial.println();
    Serial.println("Please input alarm time (Separating hours and minutes with ':')");
    boolean runLoop = true;
    while(runLoop) // wait for input
    {
      if(Serial.available() > 0)
      {
        checkTime();
        if (inputHour >= 24 || inputHour < 0 || inputMin >= 60 || inputMin < 0) // checking if values are correct 
        {
          Serial.print("Time input must be inbetween boundaries (hour: 0-23, min: 0-59). Default time 06:30 has been chosen. Reset to change alarm time");
          inputHour = 6;
          inputMin = 30;
        }
        runLoop = false;
      }
    }
    Serial.print("Chosen alarm time is: ");
    Serial.print(inputHour);
    Serial.print(":");
    Serial.println(inputMin);

    runblindStatus = 0;
    ThingSpeak.writeField(SECRET_CH_ID,3,runblindStatus,SECRET_WRITE_APIKEY); // writing runBlindstatus to thingspeak
}

void loop() 
{
  if (digitalRead(p_button) == 1)
  {
    delay(20);
    if (digitalRead(p_button)== 0)
     {
       buttonPushed();
     }
  }
   now = time(nullptr); 
   p_tm = localtime(&now);
   if(p_tm->tm_sec == 0) // update every minute
   {
        Serial.println();
        if (p_tm ->tm_hour == inputHour && p_tm -> tm_min == inputMin)
         {
            Serial.println("Reading lightintensity from slave");
            long lightIntensity = ThingSpeak.readLongField(SECRET_CH_ID, 1, SECRET_READ_APIKEY);  
        
            // Check the status of the read operation to see if it was successful
            int statusCode = ThingSpeak.getLastReadStatus();
            
            if(statusCode == 200)
            {
                Serial.print("Reading completed. Lightintensity value is: ");
                Serial.println(lightIntensity);
                if (lightIntensity < 900) // if the sun haven't come up yet, run through SunriseArray
                {
                    Serial.println("Waking up via sunrise");
                    byte  oldMinValue = p_tm->tm_min; // in order to still update sensor every minute (does not update first minute)
                    
                    
                    while(sunriseCounter<255 && sunriseCounter >= 0)
                    {
                      runSunriseArray();
          
                      // update sensory input
                      now = time(nullptr); 
                      p_tm = localtime(&now);
                      if (p_tm->tm_min != oldMinValue) // this method will work as long as the duration of sunrise is no longer than 255 minutes ( ergo over 3.25 hours )
                      {
                        readSensors();
                        oldMinValue = p_tm->tm_min;
                        sendToThingspeak();
                      }
                     if (digitalRead(p_button) == 1)
                     {
                        delay(20);
                        if (digitalRead(p_button)== 0)
                         {
                           buttonPushed();
                           sunriseCounter = -1;
                           break;
                         }
                     }
                     
                    }
                    Serial.println("Light fully on");
                }
                else  //if the sun is already up, roll the curtains.
                {
                    Serial.println("Waking up via curtains");
                    readSensors();
                    runblindStatus = 1;
                    delay(15000);
                    ThingSpeak.setField(3, runblindStatus);
                    sendToThingspeak();
                }
            }
            else //if failed to connect write error code
            {
                Serial.println("Problem reading lightintensity. HTTP error code " + String(statusCode)); 
            }
        
        }
        else 
        {
            now = time(nullptr); 
            p_tm = localtime(&now);
            readSensors();
            printTime();
            sendToThingspeak();          
        }        
   }  
   else if(p_tm->tm_hour == 22 && p_tm->tm_min == 0) // once a day, at 22:00, reset all variables
   {
        Serial.println("Resetting all variables");
        resetAll();
        sendToThingspeak();
   } 
}

void generateSunriseArray()
{
    for(int i = 0; i < 200; i++)
    {
        analogWrite(p_blue,blue);
        if (i%4 == 0) // update every fourth time
        {
           analogWrite(p_green,green--);
        }
        analogWrite(p_red,red--);
        printSunriseArray();
        delay(t_sunrisedelay );
    }
    for(int i = 0; i < 55; i++)
    {
        if(i%2==0) // update every second time
        {
            analogWrite(p_blue,blue-=1);
        }
        analogWrite(p_red,red-=1);
        analogWrite(p_green,green-=1);
        
        printSunriseArray();
        delay(t_sunrisedelay);
    }
    delay(20000);
}

void printSunriseArray() // function to print the entire array (RGB)
{
    Serial.print("{");
    Serial.print(red);
    Serial.print(","); 
    Serial.print(green);
    Serial.print(",");
    Serial.print(blue);
    Serial.println("},");
}

// functions to extract R, G and B arrays 
    void printRed()
    {
        Serial.print(red);
        Serial.print(",");
    }
    
    void printGreen()
    {
        Serial.print(green);
        Serial.print(",");
    }
    
    void printBlue()
    {
        Serial.print(blue);
        Serial.print(",");
    }

void runSunriseArray()
{
    if (sunriseCounter < 255 && sunriseCounter >= 0 ) // max. limit is the length of the SunriseArray generated
    {
        analogWrite(p_red,SunriseArrayRed[sunriseCounter]*4); // times four when using the ESP, since the analogwrite outputs number from 0-1024
        analogWrite(p_green,SunriseArrayGreen[sunriseCounter]*4);
        analogWrite(p_blue,SunriseArrayBlue[sunriseCounter]*4);
        delay(t_sunrisedelay);
        sunriseCounter++;
    }
}

void setDuration(byte inputDuration) // inputDuration is in min
{
    int inputDuration_mili =((int)inputDuration *60*1000);
    t_sunrisedelay = inputDuration_mili/255;
}

void printTime()
{
    Serial.print(p_tm->tm_mday);
    Serial.print("/");
    Serial.print(p_tm->tm_mon + 1);
    Serial.print("/");
    Serial.print(p_tm->tm_year + 1900);
    
    Serial.print(" ");
    
    Serial.print(p_tm->tm_hour);
    Serial.print(":");
    Serial.print(p_tm->tm_min);
    Serial.print(":");
    Serial.println(p_tm->tm_sec);
}

void wifiSetup()
{
   // wifi setup
    WiFi.begin(ssid,password);
    Serial.print("Connecting");
  
    while( WiFi.status() != WL_CONNECTED ){
        delay(500);
        Serial.print(".");        
    }
    Serial.println();
  
    Serial.println("Wifi Connected Success!");
}

void timeSetup()
{
  // time config
    configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
    Serial.println("\nWaiting for Internet time");
  
    while(!time(nullptr))
    {
         Serial.print("*");
         delay(1000);
    }
    Serial.println("\nTime response....OK");   
  
    now = time(nullptr); 
    p_tm = localtime(&now);
}

void readSensors()
{
  if(dht.getStatusString()=="OK");
  {
    temp= dht.getTemperature();
    ThingSpeak.setField(4,temp);
    
    hum = dht.getHumidity();
    ThingSpeak.setField(5,hum);
  }

  // co2 calcutations
  float readOut = analogRead(A0); //value from co2 sensor
  float co2 = pow(10, (readOut + 158.631)/62.877);
  ThingSpeak.setField(6,co2);
  
}

void resetAll()
{
    // reset runblindstatus 
    runblindStatus = 0;
    int x = ThingSpeak.setField(3, runblindStatus);
    oldHour = 0; oldMin = 0; colonRead = false;
}

void sendToThingspeak()
{
  int x = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_WRITE_APIKEY);
   if(x == 200){
     Serial.println("Channel update successful.");
   }
   else{
     Serial.println("Problem updating channel. HTTP error code " + String(x));
   }
   Serial.println();
}

void checkTime()
// saves time if the characters are valid
{
  while(true)
  {
      signed int character = Serial.read()-48; // -48 to convert to decimal from ASCII
      if (character < 10 && character >= 0 && colonRead == false) // if the input is a number and is hour
      {
          inputHour = oldHour*10 + character;
          oldHour = inputHour;
      }
      else if (character < 10 && character >= 0&& colonRead == true) // if the input is a number and is hour
      {
          inputMin = oldMin*10 + character;
          oldMin = inputMin;
      }
      else if (character == 10) // ascii code 58 = :
      {
          colonRead = true; 
      }
      else if (character == -38) // ascii code 10 = enter (done reading time)
      {
          break;
      }
      else if (character == -49) // error code -1
      {
        //do nothing
      }
      else
      {
          Serial.println("Please enter valid timestamp with ':' seperating hour and minutes ");
          break;
      }
  }
  
}

void buttonPushed()
{ 
    analogWrite(p_red,1024);
    analogWrite(p_green,1024);
    analogWrite(p_blue,1024);
    Serial.println("Button pushed");
}
