#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server;          // creating referance to access the function of the webserver
#define PIRPIN  2                // connect pir to pin 2 of the esp

const char* password = "adverma200";    // passward of the ssid
char* mySsid = "ESP_MESH8";  /// Creating it's own server with this ssid
String json;
 
IPAddress ip(192,168,47,191);       // Creating it's ip for the server
IPAddress gateway(192,168,46,1);        // providing gateway
IPAddress subnet(255,255,255,0);       // Default subnet

int field4=0; 
String webString="";

////////////////////////////////////////////////////////////////////  setup    ////////////////////////////////////////////////////////////////////////////////////////////////////////

 void setup(void)
{
  pinMode(PIRPIN, INPUT);     // defining the mode of the pin
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v ttl cable
  
  WiFi.mode(WIFI_AP_STA);  
  delay(100);
  wifiMulti.addAP("ESP_MESH11", "adverma200");
  wifiMulti.addAP("ESP_MESH10", "adverma200");
   wifiMulti.addAP("ESP_MESH9", "adverma200");
 Networks();
 while(wifiMulti.run() !=WL_CONNECTED)
   {
    delay(1000);
    Serial.print("Connecting..");
   }
  Serial.println("");
  Serial.println("PIR sensor output");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(mySsid, password);
  }
 /////////////////////////////////////////////////////////////////////////// pir //////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void PIR()
{
      Serial.print("Person is :");
      if(digitalRead(PIRPIN)==LOW)
      {
          field4=0;      // 0 means CLOSED and 1 means OPEN 
           Serial.println(" No one");
      }
      else
      {
         field4= 1;    // 0 means CLOSED and 1 means OPEN 
         Serial.println(" present");  
      }
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
/////////////////////////////////////////////////////////////////////////////    json //////////////////////////////////////////////////////////////////////////////////////////////////////////

void JSONDATA(String payload)
{
   json="";
  
  json +="{";
  json +="\"field4\":";
  json +=String( field4);
  json += ",";
  json+= payload;
  
}

 ///////////////////////////////////////////////////////////////////////////   loop      ///////////////////////////////////////////////////////////////////////////////////////////////////////
 
  void loop()
{
 
  
  //Getting data from last node 
  server.handleClient();
    HTTPClient http;
    //    http.begin("http://192.168.44.135/");
     if(WiFi.SSID()=="ESP_MESH11")
  {
  http.begin("http://192.168.44.135/");
  }
  else if(WiFi.SSID()=="ESP_MESH10")
  {
   http.begin("http://192.168.45.191/");
  }
  else
  {
     http.begin("http://192.168.46.191/");
  }
           int httpCode = http.GET();                                                                  //Send the request
 Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload); 
      payload.replace("{","");
      PIR();
      JSONDATA(payload);
    
 //Sending combined data to next node  
       server.on("/", [](){ 
          webString=String(json);

        server.send(200, "text/plain",webString ); 
      });    
server.begin();
      
        }   
        
        //http connection ends here
         http.end(); 
 delay(1000);
}

//////////////////////////////////////////////////////////////////  end  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

