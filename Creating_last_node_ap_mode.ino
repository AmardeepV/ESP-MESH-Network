
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
#define REEDPIN  2
char* mySsid = "ESP_MESH11";
char* password = "adverma200";

IPAddress ip(192, 168, 44, 135); // by connecting to this server that is ESP_MESH11 you can check the sensor value at this ip
IPAddress gateway(192, 168, 44, 1); //   gateway ip through which the data is going .The main thing in this is the frame 43 this should be the same as of your esp8266
IPAddress subnet(255, 255, 255, 0); // Use this by default
int field2 = 0;
String webString = "";
String json;
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 500;
/////////////////////////////////////////////////////////////////////////// Setup  //////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void)
{
  pinMode(REEDPIN, INPUT);
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v ttl cable

  WiFi.mode(WIFI_AP);     // Setting the esp at access point mode

  //  while (WiFi.status() != WL_CONNECTED)
  //  {
  //    delay(1000);
  //    Serial.print("Connecting..");
  //   }
  delay(1000);
  Serial.println("Access point created ");
  Serial.print("check Reed sensor output at this ip::");
  Serial.print("\t");
  Serial.println(mySsid);
  Serial.println(ip);
  //  Serial.print("IP address: ");
  //  Serial.println(WiFi.localIP());


  server.on("/", []() {
    REED();
    JSONDATA();
    webString = String(json);

    server.send(200, "text/plain", webString );
  });
  server.begin();
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(mySsid, password);
}

///////////////////////////////////////////////////////////////////////////////////REED Function //////////////////////////////////////////////////////////////////////////
void REED()
{
//  unsigned long currentMillis = millis();
//  if ( previousMillis < currentMillis)
//  {
//
//    previousMillis = currentMillis;
// unsigned long currentMillis = millis();
// 
//  if(currentMillis - previousMillis >= interval) {
//    

    Serial.print("Door :");
    if (digitalRead(REEDPIN) == LOW)
    {
      field2 = 0;    // 0 means CLOSED and 1 means OPEN
      Serial.println(" closed");
    }
    else
    {
      field2 = 1;  // 0 means CLOSED and 1 means OPEN
      Serial.println(" OPEN");
    }
    //server.send(204, "");
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**************************************************************************************************************************************************************************************
//////////////////////////////////////////////////////////////////////////////JSON FORMATE  /////////////////////////////////////////////////////////////////////////////////////////

void JSONDATA()
{
  json = "";

  json += "{";
  json += "\"field2\":";
  json += String(field2);
  json += "}";
}
/////////////////////////////////////////////////////////////////////////// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{

  server.handleClient();

}
///////////////////////////////////////////////////////////////////////////// finished /////////////////////////////////////////////////////////////////////////////////////////////////



