#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN  2
#include <Adafruit_Sensor.h>


ESP8266WebServer server;
 WiFiClient client;

//const char* ssid = "StartUP-Oasis";
//const char* password = "P@$$w0rd@1947";
 const char* ssid     = "Ad";
const char* password = "adverma200";
const char* Srver = "api.thingspeak.com";
String apiKey ="9K7HA3VFBM3B80WP";
//char* mySsid = "ESP_MESH@";
//
//IPAddress ip(192,168,44,191);
//IPAddress gateway(192,168,43,1);
//IPAddress subnet(255,255,255,0);

DHT dht(DHTPIN, DHTTYPE, 11);
float humidity;
String webString=""; 
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor
 
 
 void setup(void)
{
 
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v ttl cable
  dht.begin();
  
    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
   while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
   }
     Serial.println("");
  Serial.println("DHT Weather Reading Server");
// Serial.print("Connected ");
   if (client.connect(Srver, 80)) {
      Serial.println("WiFi Client connected ");
   
   String postStr ="?api_key="+apiKey;
   postStr += "&field1=";
   gettemperature();
   postStr += String(humidity);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   }
//  Serial.println(ssid);
//  Serial.print("IP address: ");
 // Serial.println(WiFi.localIP());
   // server.on("/",[](){server.send(200,"text/plain","Humidity is ");});
 // server.on("/", gettemperature);
  // server.on("/", [](){ 
    gettemperature();           // read sensor
//    webString="Humidity: "+String((int)humidity)+"%";
//    Serial.println("humidity is");
//      Serial.println(webString);
//    server.send(200, "text/plain",webString );               
//  });     
//   server.begin();
//     WiFi.softAPConfig(ip, gateway, subnet);
//  WiFi.softAP(mySsid, password);

  }
  
  void loop()
{
 
//  server.handleClient();
}


//  void gettemperature() {
// 
//  unsigned long currentMillis = millis();
// 
//  if(currentMillis - previousMillis >= interval) {
//    
//    previousMillis = currentMillis;   
//
//   
//    humidity = dht.readHumidity();          // Read humidity (percent)
//    if (isnan(humidity)) {
//      Serial.println("Failed to read from DHT sensor!");
//      return;
//    }
//    server.send(204,"");
//  }
//}
