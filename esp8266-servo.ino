#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo

#include "webpage.h"
#include "css.h"
#include "js.h"
#include "html.h"

//MODELO: PRIMEIRO COM 3 LEDS
//#define CENTER_SERVO_1 90
//#define CENTER_SERVO_2 96

//MODELO: DIRETO NO SUPORTE DE PILHAS
//#define CENTER_SERVO_1 94
//#define CENTER_SERVO_2 99

//MODELO: BARQUINHO
#define CENTER_SERVO_1 90
#define CENTER_SERVO_2 90

//LEDS SAO OPCIONAIS
#define LED1 4
#define LED2 0
#define LED3 2




WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

uint8_t pin_led = 16;
char* ssid = "YOUR_SSID";
char* password = "YOUR_PASSWORD";

IPAddress local_IP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

int delayTime = 0;

String command = "";

int pos = 0;    // variable to store the servo position

void setup()
{
  //WiFi.begin(ssid,password);
  //ATENÇAO: NAO PODE HABILITAR SERIAL POIS GPIO1 (TX) E GPIO3 (RX) SAO UTILIZADOS PARA CONTROLE DO MOTOR
  Serial.begin(9600); 
  myservo1.attach(16);
  myservo2.attach(05);
  STOP();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  WiFi.mode(WIFI_AP);  
  

  
  WiFi.softAPConfig(local_IP, gateway, subnet); //? "Ready" : "Failed!"
  WiFi.softAP("ROBOT-NETWORK-2");

  //Serial.println(WiFi.softAPIP());

  server.on("/nipplejs.js",[](){server.send_P(200,"application/javascript", js);}); 
  server.on("/styles.css",[](){server.send_P(200,"text/css", css);});
  server.on("/",[](){server.send_P(200,"text/html", html);});
  server.on("/joy2.html",[](){server.send_P(200,"text/html", webpage);});

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);    
  server.begin();
}
void loop() {  
    webSocket.loop();
    server.handleClient();
    //webSocket.broadcastTXT(c, sizeof(c));
    //Serial.println("TESTE");



}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   if (type == WStype_TEXT){
      String txt = "";
      for(int i = 0; i < length; i++)  {
        txt = txt + ((char)payload[i]);
      }

      command = txt;
         
      if (command == "up"){
        UP();        
      } else if (command == "down"){
        DOWN();
      } else if (command == "left"){
        LEFT();
      } else if (command == "right"){
        RIGHT();
      } else if (command == "end"){
        STOP();
      } else if (command == "right_up"){
        RIGHT_UP();
      } else if (command == "left_up"){
        LEFT_UP();
      } else if (command == "left_down"){
        LEFT_DOWN();
      } else if (command == "right_down"){
        RIGHT_DOWN();
      }
   }
}

 
void DOWN() {
  myservo1.write(0);
  myservo2.write(180);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
}

 
void LEFT_UP() {  
    LEFT();
    delay(1);
    UP();
    delay(1);
}

 
void RIGHT_UP() {
    RIGHT();
    delay(1);
    UP();
    delay(1);
}

void UP() {
  myservo1.write(180);
  myservo2.write(0);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
}

void LEFT_DOWN() {
    LEFT();
    delay(1);
    DOWN();
    delay(1);
}

void RIGHT_DOWN() {
    RIGHT();
    delay(1);
    DOWN();
    delay(1);
}

void RIGHT (void)
{
  myservo1.write(180);
  myservo2.write(180);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void LEFT (void)
{
  myservo1.write(0);
  myservo2.write(0);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
}

void STOP (void)
{
  myservo1.write(CENTER_SERVO_1);
  myservo2.write(CENTER_SERVO_2);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}
