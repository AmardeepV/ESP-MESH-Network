#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WebServer server;
ESP8266WiFiMulti wifiMulti;
WiFiClient client;

 HTTPClient http;
 String load;
//https://thingspeak.com/channels/306911/private_show
 void setup(void)
{
      Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v ttl cable
      WiFi.mode(WIFI_STA);
      delay(100);
 
}
  
   void Networks(){
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(")");
     // Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
 
 }
 
  void loop()
{
    int  flag=0;    
     // WiFi.begin(ssid,password);
      wifiMulti.addAP("ESP_MESH10", "adverma200");
      wifiMulti.addAP("ESP_MESH9", "adverma200");
      wifiMulti.addAP("ESP_MESH8", "adverma200");
      wifiMulti.addAP("ESP_MESH7", "adverma200");
       Networks();
       
     while(wifiMulti.run() !=WL_CONNECTED)
       {
        delay(1000);
        Serial.print("Connecting..");
       }
     Serial.println("");
     Serial.print("Connected to ");
     Serial.println(WiFi.SSID());     
     if(WiFi.SSID()=="ESP_MESH8")
              {
                  http.begin("http://192.168.47.191/");
                  }
                  else if(WiFi.SSID()=="ESP_MESH10")
                  {
                   http.begin("http://192.168.45.191/");
                  }
                  else if(WiFi.SSID()=="ESP_MESH9")
                  {
                     http.begin("http://192.168.46.191/");
                  }
                  else
                  { 
                    http.begin("http://192.168.48.191/");
              }

    int httpCode = http.GET();                    
    Serial.println(httpCode);
    if (httpCode > 0)      //Check the returning code
    { 
       load = http.getString();   //Get the request response payload
     // Serial.println(load); 
      delay(1000);
    // Serial.println(load.length());
     //Serial.println(flag);
  if(load.length() >0 && flag == 0)
  {
  //  load="{\"field1\":\"1\",\"field2\":\"1\"}";
    Serial.println(load);
    
 char* sssid     = "Einstein Sir";
   char*  spassward = "21%charged";
     WiFi.begin(sssid,spassward);
         while (WiFi.status() != WL_CONNECTED)
         {
          delay(2000);
          Serial.print("Connecting again..");
         }
    if (http.begin("http://api.thingspeak.com/update?api_key=9K7HA3VFBM3B80WP"))
    {
  
//    http.begin(" http://api.thingspeak.com/update?api_key=9K7HA3VFBM3B80WP"); //Specify request destination
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(load);//Send the request
          Serial.println("data loaded on server ");
      Serial.println(httpCode); //Print HTTP return code
   
WiFi.disconnect(); 
     // delay(1000);
      http.end();
      delay(1000);
    }
  }
    }
}
  
  
  

