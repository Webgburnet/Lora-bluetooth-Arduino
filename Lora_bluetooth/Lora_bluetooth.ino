#include <SoftwareSerial.h>   //Software Serial Port
#include <RHReliableDatagram.h>
#include <RH_RF95.h>

#define RxD 10
#define TxD 11

SoftwareSerial SSerial(RxD, TxD); // RX, TX
#define COMSerial SSerial

#define vitesse_arduino 9600
#define vitesse_bluetooth 57600

RH_RF95<SoftwareSerial> driver(COMSerial);
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 4

RHReliableDatagram manager(driver, SERVER_ADDRESS);

String recvChar;
String emitChar;
String commande_bluetooth;

void setup()
{
  Serial.begin(vitesse_arduino);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  Serial.println("Debut setup");
  Serial3.begin(vitesse_bluetooth);

  if (!manager.init()) 
  {
        Serial.println("init failed");
  }
  else
  {
      Serial.println("init ok");
  }
  
  driver.setFrequency(868.0);
  
  Serial.println("Fin setup");
  delay(2000);
  Serial3.flush();

  
}

uint8_t data[] = "And hello back to you";
// Ne mettez pas cela sur la pile:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
String messages="";

void loop()
{
  
  if(Serial3.available()>0)
  {
    recvChar = Serial3.readString();
    Serial.println(recvChar);
    commande_bluetooth = recvChar;
    Serial.println(commande_bluetooth);
//    Serial.print(Serial3.read());
//    Serial.write(Serial3.read());
  }
  if(Serial.available()>0)
  {
    emitChar  = Serial.readString();
    Serial.println(emitChar);
    Serial3.print(emitChar);
//    Serial3.write(Serial.read());
//    Serial3.print(Serial.read());
  }

  if(commande_bluetooth=="ButtonRecuCapteur")
  {
    Serial.println("Emission vers le telephone");
    emitChar="&";
    emitChar=emitChar+"Valeur 1";
    emitChar=emitChar+"%";
    Serial3.print(emitChar);
    commande_bluetooth="";
  }
  if(commande_bluetooth=="Button1")
  {
    Serial.println("Actionde la commande Button 1");
    commande_bluetooth="";
  }
  if(commande_bluetooth=="Button2")
  {
    Serial.println("Actionde la commande Button 2");
    commande_bluetooth="";
  }
  if(commande_bluetooth=="Button3")
  {
    Serial.println("Actionde la commande Button 3");
    commande_bluetooth="";
  }
  if(commande_bluetooth=="Button4")
  {
    Serial.println("Actionde la commande Button 4");
    commande_bluetooth="";
  }
  
  if (Serial.available()>0)
    {
      messages=Serial.readString();
      Serial.println(messages);
      messages.toCharArray(data,sizeof(data));
      Serial.println((char*)data);
    }
    
    if (manager.available()) 
    {
        // Attendre un message qui nous est adressé par le client
        uint8_t len = sizeof(buf);
        uint8_t from;

        if (manager.recvfromAck(buf, &len, &from)) 
        {
            Serial.print("Requete du client 0x");
            Serial.print(from, HEX);
            Serial.print(": ");
            Serial.println((char*)buf);

            // Renvoie une réponse au client expéditeur
            if (!manager.sendtoWait(data, sizeof(data), from)) 
            {
                Serial.println("échec de l'envoi");
            }
        }
    }
}
