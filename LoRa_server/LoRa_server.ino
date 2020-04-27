// Exemple d'esquisse montrant comment créer un client de messagerie simple et fiable
// avec la classe RHReliableDatagram, en utilisant le pilote RH_RF95 pour contrôler une radio RF95.
// Il est conçu pour fonctionner avec l'autre exemple rf95_reliable_datagram_server
// Testé avec Anarduino MiniWirelessLoRa

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(10, 11); // RX, TX
    #define COMSerial SSerial
    #define ShowSerial Serial

    RH_RF95<SoftwareSerial> driver(COMSerial);
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 4

// Singleton instance of the radio driver
//SoftwareSerial ss(10, 11);
//RH_RF95 driver(ss);

// Classe pour gérer la livraison et la réception des messages, en utilisant le pilote déclaré ci-dessus
RHReliableDatagram manager(driver, SERVER_ADDRESS);

void setup() {
    ShowSerial.begin(9600);
   
    if (!manager.init()) {
        ShowSerial.println("init failed");
    }
    else
    {
      ShowSerial.println("init ok");
    }

    // Les valeurs par défaut après l'initialisation sont 434,0 MHz, 13 dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128 puces / symbole, CRC activé
     // La puissance d'émission par défaut est de 13 dBm, en utilisant PA_BOOST.
     // Si vous utilisez des modules RFM95 / 96/97/98 qui utilisent la broche de l'émetteur PA_BOOST, alors
     // vous pouvez régler les puissances d'émission de 5 à 23 dBm:
     //driver.setTxPower(23, false);
     driver.setFrequency(868.0);
}

uint8_t data[] = "And hello back to you";
// Ne mettez pas cela sur la pile:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
String messages="";

void loop() {
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
            ShowSerial.print("got request from : 0x");
            ShowSerial.print(from, HEX);
            ShowSerial.print(": ");
            ShowSerial.println((char*)buf);

            // Renvoie une réponse au client expéditeur
            if (!manager.sendtoWait(data, sizeof(data), from)) 
            {
                ShowSerial.println("sendtoWait failed");
            }
        }
    }
}
