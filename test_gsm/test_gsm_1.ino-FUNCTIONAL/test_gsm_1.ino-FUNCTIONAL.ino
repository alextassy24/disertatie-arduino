

//#define TINY_GSM_MODEM_SIM800

//#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // SIM800L Tx & Rx Arduino #3 & #2
// portocaliu pin3, maro pin2
bool sendCommand(String command, unsigned long timeout = 5000);

bool connectionEstablished = false;
//
//char apn[]  = "internet";
//char user[] = "";
//char pass[] = "";
//
//const char server[] = "disertatie-api.azurewebsites.net";
//const char resource[] = "/t";
//const int  port = 80;

//TinyGsm modem(mySerial);
//TinyGsmClient client(modem);
//HttpClient http(modem, server, port);


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("INFO: Trying to connect to the network...");
  connectionEstablished = establishConnection();
  if (connectionEstablished) {
    Serial.println("INFO: Initializing complete!");
  } else {
    Serial.println("ERROR: Connection establishment failed.Entering debugging mode");
    while (true) {
      while (Serial.available())
      {
        mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
      }
      while (mySerial.available())
      {
        Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
      }
    }
  }
  //  Serial.println("Initializing modem...");
  //  modem.restart();
  //  Serial.println("Restart Over...........");
  //  String modemInfo = modem.getModemInfo();
  //  Serial.print("Modem: ");
  //  Serial.println(modemInfo);
  //  modem.init();
  //  if (modem.isGprsConnected()) {
  //    Serial.println("GPRS connected");
  //  }
}

void loop() {
  if (connectionEstablished) {
    static unsigned long lastSendTime = 0;
    unsigned long currentTime = millis();

    //    Send data every 1000 milliseconds
    if (currentTime - lastSendTime >= 10000) {
      sendData();
      lastSendTime = currentTime;
    }

  }
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

bool establishConnection() {
  if (!sendCommand("AT+CMEE=2")) return false;
  if (!sendCommand("AT")) return false;
  if (!sendCommand("AT+CSQ")) return false;
  if (!sendCommand("AT+CREG?")) return false;
  //  if (!sendCommand("AT+COPS=1,2, \"22605\"")) return false;
  //  delay(5000);
  //  if (!sendCommand("AT+CREG?")) return false;
  if (!sendCommand("AT+CGREG?")) return false;
//  if (!sendCommand("AT+SAPBR=2,1")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"APN\",\"live.vodafone.com\"")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"USER\",\"live\"")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"PWD\",\"\"")) return false;
  if (!sendCommand("AT+SAPBR=4,1")) return false;
  if (!sendCommand("AT+SAPBR=2,1")) return false;
  // Activate SAPBR context
  if (!sendCommand("AT+SAPBR=1,1")) {
    Serial.println("Failed to activate bearer. Trying again...");
    delay(5000); // Wait for a bit before retrying
    if (!sendCommand("AT+SAPBR=1,1")) {
      Serial.println("Failed to activate bearer on second attempt. Exiting...");
      return false;
    }
  }

  // Optionally, check the status again before deactivating
  sendCommand("AT+SAPBR=2,1");

  // Deactivate SAPBR context
//  if (!sendCommand("AT+SAPBR=0,1")) {
//    Serial.println("Deactivation failed, trying again...");
//    delay(1000); // Wait for a bit before retrying
//    if (!sendCommand("AT+SAPBR=0,1")) {
//      Serial.println("Still failed on deactivation. Check status or conditions.");
//    }
//  }
  return true;
}

void sendData() {
  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPSSL=1");

  // Send HTTP GET request
  String apiURL = "https://disertatie-api.azurewebsites.net/t";
  //  String apiURL = "http://minimalsites.com/";
  sendCommand("AT+HTTPPARA=\"URL\",\"" + apiURL + "\"");
  delay(1000); // Add a small delay to ensure the command is processed properly

  sendCommand("AT+HTTPPARA=\"CID\",1");
//  sendCommand("AT+HTTPSSL=1");
//  delay(1000);
  
  sendCommand("AT+HTTPACTION=0");
  delay(1000); // Wait for response

  sendCommand("AT+HTTPREAD");
  delay(1000); // Wait for response

  sendCommand("AT+HTTPTERM");
}

void clearSerialBuffer() {
  while (mySerial.available()) {
    mySerial.read();
  }
}


bool sendCommand(String command, unsigned long timeout) {
  clearSerialBuffer();  // Clear any residual data from previous commands
  Serial.print("Sending: ");
  Serial.println(command);
  mySerial.println(command);

  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < timeout) {
    while (mySerial.available()) {
      char c = mySerial.read();
      response += c;
      if (c == '\n') {
        Serial.print("Received: ");  // Make sure to output the received data for debugging
        Serial.print(response);
        if (response.indexOf("ERROR") != -1) {
          return false;
        }
        if (response.indexOf("OK") != -1) {
          return true;  // Command was successful
        }
        response = "";  // Reset response string for the next line
      }
    }
  }
  Serial.println("ERROR: Response timeout");
  return false;  // Timeout reached without "OK"
}
