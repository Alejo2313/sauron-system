/*
  Sauron System sensor server.

  The sensor listens in port 80 and connects to 
  the SAURON-NODE Network, then it sends it´s IP
  adress to get registered in the system.

  Then, it waits to get instructions from the server.
  The explore instruction activater the Ultrasonic sensor
  and it scans the whole shelf for products
 */


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include "Ultrasonic.h"


#define ECHO_PIN 5 
#define TRIGGER_PIN 4
#define SERVO_PIN 0


#define LED_READY_PIN 12
#define LED_REGISTERED_PIN 14
#define LED_TOGGLE_PIN 16

#define ROUTE_ERROR -1
#define ROUTE_LED_TOGGLE 0
#define ROUTE_LED_TOGGLE_ON 1
#define ROUTE_LED_TOGGLE_OFF 2


const char* ssid = "SAURON-NODE";
const char* password = "raspberrypi";
int ledStatus = 0;
int objCount = 0;

String ipAddr; // stores sensor ip address

const int serverPort = 80;
const char* serverUrl = "http://172.24.1.1";


Servo mis;                 // servo
byte angle = 90;           // reference angle
boolean f;                 // determins if there is object or not (0->no 1 -> yeah)
byte cont = 0;             // counters: determine the numver of values taken and the turned angle (H-> wholw angle  O-> number of object data)
boolean preF;              // stores the previous value of f to determine where a gap or object begins and ends
unsigned comand;           // read buffer value
byte value = 0;            // read sensor value
double objSum = 0;         // sum of the values obtained from the object (along with much does the average value is calculated)
double lObj = 0;        // stores the average value of the distance
byte angRef = 20;          // Initial reference angle
byte angL = angRef;        // stores the first measured angle
double lValue;             // stores the last radius read
double bValue;             // Radio stores the first radius read
const double p = 3.1415;   // pi constant 




// TCP server at port 80 will respond to HTTP requests
ESP8266WebServer server(80);
  

// Pin setUp  and serial
void setUpHardware(){
  Serial.begin(115200);
  pinMode(LED_TOGGLE_PIN, OUTPUT);
  pinMode(LED_REGISTERED_PIN, OUTPUT);
  pinMode(LED_READY_PIN, OUTPUT);
  mis.attach(SERVO_PIN);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);


  digitalWrite(LED_TOGGLE_PIN, 0);
  digitalWrite(LED_REGISTERED_PIN, 0);
  digitalWrite(LED_READY_PIN, 0);
}

// Get distace from object
double getDistance(){
  double t;
  double dist;
  digitalWrite(TRIGGER_PIN,LOW); /*As a matter of stabilizing the sensor*/
  delayMicroseconds(5);
  digitalWrite(TRIGGER_PIN, HIGH); /* shipping ultrasonic pulse*/
  delayMicroseconds(30);
  t=pulseIn(ECHO_PIN, HIGH, 1500/0.017); /* Function to measure the length of the incoming pulse. Measures the time elapsed between sending
   the ultrasonic pulse and when the sensor receives the rebound, ie since the pin 12 begins to receive the bounce, HIGH until
   fails to do so, LOW, incoming pulse length*/
  dist = double(0.017*t); /*formula to calculate the distance obtaining an integer value*/  
  return dist;

}

// Activate sensor scan
void explore(){
  objCount = 0;
  Serial.println("--------------------- NEW MEASURE ---------------------");
  mis.write(constrain(angRef, 0, 180));
  delay(25);
  preF =  (getDistance() < 50);// (ultrasonic() < 50);       //the first value Pref is the first found
  delay(25);
  for(int i = angRef+1; i < 180 - angRef; i++) { //Rotates from (180-i) to (i)
     angle = i;
     angle = constrain(angle, 0, 180);
     mis.write(angle);
     delay(25);
     value =getDistance();// ultrasonic();
     Serial.print("Value = ");
     Serial.println(value);

     f = (value < 50);     // el valor maximo que da el sensor el 51, si es menos es que hay objeto

     if (f == 1) {         // if there object, high consecutive values which gives the sensor to calculate the mean value
        if (preF == 0) {   // if a gap passes an object the size of the hole is finished
           cont = 0;
           angL = angle;  // angle at which begins to detect object
           bValue =getDistance(); // ultrasonic();    // radius value where it begins to detect the object
        }
          preF = f;
          objSum = objSum + (value * sin(angle * p * 2 / 360 ));
          lValue = value;       // stores the last measured radius
          cont++;
     }
     if (f == 0) {            // calculate the total angle rotated
        if (preF == 1) {      // if you pass an object to a gap measurement object ends
          if (cont >= 5) {    // if the sweep angle in the object is less than 6 is not an object (prevents false measurements)
             lObj = objSum / cont;
             int tamano = abs(bValue * cos(2 * p * angL / 360)) - abs(lValue * cos((angle - 1) * 2 * p / 360));
             Serial.print("\n first angle "); Serial.print(angL); Serial.print(" second "); Serial.print(angle); Serial.print(" distance ");  Serial.print(lObj);
             Serial.print(" aprox size ");  Serial.println(tamano);
             objSum = 0;
             cont = 0;
             objCount = objCount + 1;
          }
        }
      preF = 0;
      cont++;
     }
     delay(25);

 }
 mis.write(angRef);
}




// Connect to WiFi network
void connectToWIFI(){
   WiFi.begin(ssid, password);
  Serial.println("");  
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  IPAddress ip = WiFi.localIP();
  ipAddr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  Serial.println(ipAddr);
  digitalWrite(LED_READY_PIN, 1);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  ledStatus = 0; // turn off led
}

// Get raw HTML in a sigle string
String getView(){
    String ledStr = (ledStatus == 0) ? "ON" : "OFF";
    String s;
    s ="<!DOCTYPE html>";
    s += "<html><body>";
    s += "<h1>SAURON NODE LED DEMO</h1>";
    s += "<html><body>";
    s += "<a href=\'http://";
    s += ipAddr;
    s += "/led/";
    s +=  abs(ledStatus-1);
    s += "\'><button>TURN LED ";
    s += ledStr;
    s += "</button></a>";
    s += "</body></html>";
    return s;
}

// Send to sever the number of objects found
void sendObjects(){
  
  // Use WiFiClient class to create TCP connections
  HTTPClient http;
  String url = serverUrl;
  url += "/objects?n=";
  url += objCount;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  http.begin(url); //HTTP
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
          digitalWrite(LED_REGISTERED_PIN, 1);
      }
      
   } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
   }
   
   http.end();
}

// Routing between paths
void route(int code){
  digitalWrite(LED_READY_PIN, 0);
  Serial.println(code);
  switch (code){
    case ROUTE_ERROR:
    handleNotFound();
    break;
    case ROUTE_LED_TOGGLE:
        ledStatus = abs(ledStatus-1);
        server.send(200, "text/html",getView());
        explore();
        sendObjects();
    break;
    case ROUTE_LED_TOGGLE_ON:
        ledStatus = 1;
        server.send(200, "text/html",getView());
    break;
    case ROUTE_LED_TOGGLE_OFF:
        ledStatus = 0;
        server.send(200, "text/html",getView());
    break;
    default:
    handleNotFound();
    break;
  }
  digitalWrite(LED_TOGGLE_PIN, ledStatus);
  digitalWrite(LED_READY_PIN, 1);
}

// Start TCP (HTTP) server
void setUpServer(){
  
  server.on("/", [](){
    route(ROUTE_LED_TOGGLE);
  });

  server.on("/led/0", [](){
    route(ROUTE_LED_TOGGLE_OFF);
  });

  server.on("/led/1", [](){
    route(ROUTE_LED_TOGGLE_ON);
  });

  server.onNotFound([](){
    route(ROUTE_LED_TOGGLE_ON);
  });
  
  server.begin();
  Serial.println("TCP server started");  
}


// re
void registerInServer(){
  
  // Use WiFiClient class to create TCP connections
  HTTPClient http;
  String url = serverUrl;
  url += "/register?ip=";
  url += ipAddr;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  http.begin(url); //HTTP
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
          digitalWrite(LED_REGISTERED_PIN, 1);
      }
      
   } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
   }
   
   http.end();
}

void setup(void)
{  
  Serial.println("setUpHardware");
  setUpHardware();
  connectToWIFI();
  

  // Set up mDNS responder:
  // - first argument is the domain name
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("sensor")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  
  setUpServer();
  
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  registerInServer();
}

void loop(void)
{
  server.handleClient();
}

