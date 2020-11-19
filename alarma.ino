// Alarma con sensor magnetico
//El relay se activa en LOW

#include <pt.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Inicializamos la conexion WIFI con el Router
char ssid[] = "arduino";     // el nombre de tu Red
char password[] = "123456789"; // la clave de tu Red

// Inicializar Telegram BOT
#define BOTtoken "934960872:AAEgQ2eoG3jMeT2gWEGioPvL0fw3ZOt3mOk"  // el token de tu BOT, lo obtenemos de BotFather
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
int Bot_mtbs = 15000; //Tiempo medio entre el escaneo de mensajes
long Bot_lasttime;   //exploracion de el ultimo mensaje
bool Start = false;
String chat_id2;
bool flag = false;

//Hilos
struct pt hiloSensores;
struct pt hiloTelegram;

//Configura inputs y outputs
#define SIRENA 5  //D1
#define MAGNETICO_1 14 //D5
#define MAGNETICO_2 12 //D6
#define MAGNETICO_3 13 //D7
#define MAGNETICO_4 4 //D2
#define MAGNETICO_5 16  //D0

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
 
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    chat_id2 = chat_id;
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 
    if (text == "/start") {
      String welcome = "Bienvenido a el BOT ALARMA , " + from_name + ".\n";
      welcome += "Escribe /opciones si quieres ver lo que puedo hacer.\n\n";
      //welcome += "/send_test_action : to send test chat action message\n";
      bot.sendMessage(chat_id, welcome);
    }
 
    if (text == "/opciones") {
      String keyboardJson = "[[\"/SIRENA_On\", \"/SIRENA_Off\"],[\"/SIRENA_ESTADO\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Selecciona una de las siguientes opciones:", "", keyboardJson, true);
    }
 
    if (text == "/SIRENA_Off") {
      digitalWrite(SIRENA, HIGH);
      bot.sendMessage(chat_id, "SIRENA Off");
    }
 
    if (text == "/SIRENA_On") {
      digitalWrite(SIRENA, LOW);
      bot.sendMessage(chat_id, "SIRENA On");
    }
 
    if (text == "/SIRENA_ESTADO") {
      if (digitalRead(SIRENA) == 0) {      
         bot.sendMessage(chat_id, "SIRENA On");
      } 
      else {
        bot.sendMessage(chat_id, "SIRENA Off");
      }
    }
  }
}


void setup() {
  
  
    Serial.begin(115200);
     Serial.println();
  Serial.println("Code has started!");
  // Establecer el modo WiFi y desconectarse de un AP si fue Anteriormente conectada
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
  // Intentar conectarse a la red
  Serial.print("Conectando al Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("");
  Serial.println("WiFi conectada");
  PT_INIT(&hiloSensores);
  PT_INIT(&hiloTelegram);
  pinMode(SIRENA, OUTPUT);  //Salida a la sirena D1
  pinMode(MAGNETICO_1,INPUT_PULLUP); //entrada sensor magnetico 1  D5
  pinMode(MAGNETICO_2,INPUT_PULLUP); //entrada sensor magnetico 2  D6
  pinMode(MAGNETICO_3,INPUT_PULLUP); //entrada sensor magnetico 3  D7
  pinMode(MAGNETICO_4,INPUT_PULLUP); //entrada sensor magnetico 4  D8
  pinMode(MAGNETICO_5,INPUT_PULLUP); //entrada sensor magnetico 5  D0 
  digitalWrite(SIRENA, HIGH);
   client.setInsecure();
}

void loop() {
  
  sensores(&hiloSensores);
   telegram(&hiloTelegram);
 

}
//funcion encargada del manejo de telegram
int telegram(struct pt *pt){
   PT_BEGIN(pt);
// do{
   if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while (numNewMessages) {
      Serial.println("Hilo Telegram");
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
  
 
    Bot_lasttime = millis();
  }
 // }while(true);
  
   PT_END(pt);
  return 0;
  
}
//funcion encargada del manejo de los sensores
int sensores(struct pt *pt) {
  PT_BEGIN(pt);
  // void setup() {
  static  int value_1 = 0;
  static  int value_2 = 0;
  static  int value_3 = 0;
  static  int value_4 = 0;
  static  int value_5 = 0;
  static long t = 0;
 
  // }
  
  do {
 
   value_1 =  digitalRead(MAGNETICO_1);
   value_2 =  digitalRead(MAGNETICO_2);
   value_3 =  digitalRead(MAGNETICO_3);
   value_4 =  digitalRead(MAGNETICO_4);
   value_5 =  digitalRead(MAGNETICO_5);
     

   if (value_1 == HIGH) // En HIGH se ACTIVA la alarma
   {
    digitalWrite(SIRENA, LOW);
     bot.sendMessage(chat_id2, "SE ACTIVO SENSOR 1");
   }
   
   if (value_2 == HIGH) // En HIGH se ACTIVA la alarma
   {
    digitalWrite(SIRENA, LOW);
     bot.sendMessage(chat_id2, "SE ACTIVO SENSOR 2");
   }
   
   if (value_3 == HIGH) // En HIGH se ACTIVA la alarma
   {
    digitalWrite(SIRENA, LOW);
     bot.sendMessage(chat_id2, "SE ACTIVO SENSOR 3");
   }
   if (value_4 == HIGH) // En HIGH se ACTIVA la alarma
   {
    digitalWrite(SIRENA, LOW);
     bot.sendMessage(chat_id2, "SE ACTIVO SENSOR 4");
   }
   if (value_5 == HIGH) // En HIGH se ACTIVA la alarma
   {
    digitalWrite(SIRENA, LOW);
     bot.sendMessage(chat_id2, "SE ACTIVO SENSOR 5");
   }
   
   //Se activo la alarma.
    if (digitalRead(SIRENA) == 0) {
     
         Serial.println("Se activo SIRENA !!!!");
        bot.sendMessage(chat_id2, "SE ACTIVO SIRENA");
      
      
        }
   
    
    t = millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=50);
   
 
  } while(true);
  PT_END(pt);
  return 0;
}
