#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

ESP8266WebServer server;          // creating referance to access the function of the webserver
#define POWERPIN  2               // connect pir to pin 2 of the esp
 const char* ssid     = "ESP_MESH11";    // connected to the ssid
const char* password = "adverma200";    // passward of the ssid
char* mySsid = "ESP_MESH10";  /// Creating it's own server with this ssid
String json;
 
IPAddress ip(192,168,45,191);       // Creating it's ip for the server
IPAddress gateway(192,168,44,1);        // providing gateway
IPAddress subnet(255,255,255,0);       // Default subnet
 int field1=0; 
String webString="";
////////////////////////////////////////////////////////////////////  setup    ////////////////////////////////////////////////////////////////////////////////////////////////////////

 void setup(void)
{
  pinMode(POWERPIN, INPUT);      // defining the mode of the pin
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v ttl cable
  
  WiFi.mode(WIFI_AP_STA);      // setting the mode of the esp module i.e both in client as well as server
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print("Connecting..");
   }
  Serial.println("");
  Serial.println("Power sensor output");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(mySsid, password);
  }
 /////////////////////////////////////////////////////////////////////////// pir //////////////////////////////////////////////////////////////////////////////////////////////////////////
 
  void POWER()
{
 
      Serial.print("power is :");
      if(digitalRead(POWERPIN)==LOW)
      {
           power="OFF";      // 0 means CLOSED and 1 means OPEN 
           Serial.println(" Power off");
      }
      else
      {
         power="ON";    // 0 means CLOSED and 1 means OPEN 
         Serial.println(" Power is on");  
      }

}

/////////////////////////////////////////////////////////////////////////////    json //////////////////////////////////////////////////////////////////////////////////////////////////////////

void JSONDATA(String payload)
{
   json="";
  
  json +="{";
  json +="\"field1\":";
  json +=String( field1);
  json += ",";
  json+= payload;
  
}

 ///////////////////////////////////////////////////////////////////////////   loop      ///////////////////////////////////////////////////////////////////////////////////////////////////////
 
  void loop()
{
 
  
  //Getting data from last node 
  server.handleClient();
    HTTPClient http;
        http.begin("http://192.168.44.135/");
           int httpCode = http.GET();                                                                  //Send the request
 Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload); 
      payload.replace("{","");
      POWER();
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

