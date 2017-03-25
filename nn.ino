#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "hackatour";
const char* password = "";

const char* host = "crum-cidesi.cloudapp.net";

IPAddress ip(192, 168, 1, 222);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
int ledPin = 16; //Lo conectamos a D7, que es el gpio13
int pre=0;
int fle=0;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  //WiFi.config(ip, gateway, subnet);

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "pianomecano" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.begin();
}

void loop() {

  delay(5000);
 
  Serial.print("connecting to ");
  Serial.println(host);
 
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 3001;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  //Recieve data from arduino uno
if(Serial.available()){
  int s1Byte = Serial.read();
  int s2Byte = Serial.read();
  pre = Serial.read();
  fle = Serial.read();
  
  
  Serial.write(s1Byte);
  Serial.write(s2Byte);
  Serial.write(pre);
  
}
  
  // We now create a URI for the request
  String url = "?var1=""&var2=";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  Serial.print(fle);
  Serial.print(pre);

  client.print(String("Flexion: ")+fle);
  client.print(String("Presion: ")+ pre);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

