#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


int id= 2;   //id of each device, to be changed for every device
String url= "";   //url for all devices
String urlwithid= url + id + "/";    //url for a single device instance
String Data;   //sensor data to be sent to the server
float temperature;
int battery;
float latitude;
float longitude;
void setup()
{
  Serial.begin(9600); //starting serial for debugging
  
  //Connect esp8266 to wifi LAN
  WiFi.begin("Mubbie-Connect-X", "12345678910111213");
  Data= "{\"id\":" + String(id) + "}"; 
  
  //Check connection status to the wifi LAN
  while(WiFi.status() != WL_CONNECTED){delay(500);}

  //Reconnect esp8266 automatically when connection is lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  //Check connection status to the wifi LAN
  if(WiFi.status() == WL_CONNECTED){
     WiFiClient clients;
     HTTPClient http;
     http.begin(clients, urlwithid);
     
     //Geting the instance of the device
     int ResponseCode= http.GET();
     http.end();
     
     //Check if instance of the device is already available, incase the device has been switched off and turned on back or connection lost and reconnected 
     if(ResponseCode == 404){ 
          //instance of the device is not available
          http.begin(clients, url); 
          http.addHeader("Content-type","application/json");
          //Create a new instance of the device
          http.POST(Data);
          http.end();
        } 
      //instance is already available continue to updating the instance
      } 
    
}

void loop() {
     //Get sensors reading into the variables
     temperature= 50.2;
     battery= 98;
     longitude= 43.75;
     latitude= 15.63;
     
     //Sensor data to be sent to the server in JSON format
     Data= "{\"id\":" + String(id) + "," + "\"temperature\":" + String(temperature) + "," "\"battery\":" + String(battery) + "," "\"long\":" + String(longitude) + "," "\"lat\":" + String(latitude) + "}";

     //Check connection status to the wifi LAN
     if(WiFi.status() == WL_CONNECTED){
       WiFiClient clients;
       HTTPClient http;
       http.begin(clients, urlwithid);
       http.addHeader("Content-type","application/json");

       //Updating the instance of the device every 10secs
       int response= http.PUT(Data);
       Serial.print(response);
       http.end();
      }

     delay(10000);
  
  }