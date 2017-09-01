#include <SoftwareSerial.h>

SoftwareSerial ESP8266(10, 11);

String NomduReseauWifi = "YOUR_SSID"; // Garder les guillements
String MotDePasse      = "YOUR_PASS"; // Garder les guillements
String TCP = "TCP";
String IP = "api.thingspeak.com";
String Port = "80";


/****************************************************************/
/*                             INIT                             */
/****************************************************************/

int pirPin = 3;    //the digital pin connected to the PIR sensor's output

void setup()
{

 /// ************** Sensor calibration
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  ESP8266.begin(115200);  
  initESP8266();

  pinMode(7,OUTPUT);
  digitalWrite(7, LOW);
}
/****************************************************************/
/*                        BOUCLE INFINIE                        */
/****************************************************************/
void loop()
{

if (digitalRead(pirPin) == HIGH) 
{

Serial.println("Motion");
Serial.println("********************** Start sending ***********************");
envoieAuESP8266("AT+CIPSTART=0,\""+ TCP + "\",\"" + IP + "\",80");


  recoitDuESP8266(500);
envoieAuESP8266("AT+CIPSEND=0,82");
recoitDuESP8266(500);
// GET /apps/thingtweet/1/statuses/update?api_key=API KEY&status=YOUR MESSAGE
envoieAuESP8266("GET /apps/thingtweet/1/statuses/update?api_key=YHJGGAQXER5YXPA1&status=Hamdoulah");
  recoitDuESP8266(500);

  Serial.println("************************ End sending *********************");
  digitalWrite(7, HIGH);
  delay(10000);
}
else
{Serial.println("No Motion");
digitalWrite(7, LOW);
delay(2000);

}
 
} // end loop
/****************************************************************/
/*                Fonction qui initialise l'ESP8266             */
/****************************************************************/
void initESP8266()
{  
//  Serial.println("**********************************************************");  

envoieAuESP8266("AT+RST");
recoitDuESP8266(2000);
  Serial.println("****************** Start Wifi connection ******************");
  envoieAuESP8266("AT+CWMODE=1");
  recoitDuESP8266(2000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CWJAP=\""+ NomduReseauWifi + "\",\"" + MotDePasse +"\"");
  recoitDuESP8266(2000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIFSR");
  recoitDuESP8266(2000);
    Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIPMODE=0");
  recoitDuESP8266(2000);
  Serial.println("**********************************************************");
   envoieAuESP8266("AT+CIPMUX=1");
  recoitDuESP8266(2000);

  Serial.println("**********************************************************");
  Serial.println("***************** INITIALISATION TERMINEE ****************");
  Serial.println("**********************************************************");
  Serial.println("");  
  delay(5000);
 
}

/****************************************************************/
/*        Fonction qui envoie une commande à l'ESP8266          */
/****************************************************************/
void envoieAuESP8266(String commande)
{  
  ESP8266.println(commande);
}
/****************************************************************/
/*Fonction qui lit et affiche les messages envoyés par l'ESP8266*/
/****************************************************************/
void recoitDuESP8266(const int timeout)
{
  String reponse = "";
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(ESP8266.available())
    {
      char c = ESP8266.read();
      reponse+=c;
    }
  }
  Serial.print(reponse);   
}