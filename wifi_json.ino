#include <ArduinoJson.h>
#include <ESP8266WiFi.h>


const char* ssid     = "md-dmz";
const char* password = "catalyst!NMD1";

//const char* host = "data.sparkfun.com";
//const char* streamId   = "WGg4vXOKj7fbKRoGmpb7";
//const char* privateKey = "XR6qen2YM4H74pq0WY7e";
const char* host = "api.openweathermap.org";
const char* weatherKey = "770899aed214da5d7393ad92f4c7bce7";
const char* cityID = "4352539";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/data/2.5/weather?id=";
  url += cityID;
  url += "&";
  url += "appid";
  url += "=";
  url += weatherKey;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  delay(500);
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    StaticJsonBuffer<400> jsonBuffer;
    //char line[] = client.readStringUntil('\r').c_str();
   
    String string = client.readStringUntil('}');
    char newChar[string.length()+1];
    string.toCharArray(newChar, sizeof(newChar));
    Serial.println("Car output");
    Serial.println(newChar);
    JsonObject& root = jsonBuffer.parseObject(newChar);
    if (!root.success()){
      Serial.println("parseObject() failed");
      return;
    }
    const char* name = root["name"];
    Serial.print(name);
  }
  
  Serial.println();
  Serial.println("closing connection");
}






