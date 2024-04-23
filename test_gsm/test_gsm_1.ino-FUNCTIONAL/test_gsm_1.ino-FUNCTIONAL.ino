

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // SIM800L Tx & Rx Arduino #3 & #2
// portocaliu pin3, maro pin2

bool connectionEstablished = false;

char apn[]  = "internet";
char user[] = "";
char pass[] = "";

const char server[]   = "disertatie-api.azurewebsites.net";
const int  port       = 80;

TinyGsm modem(mySerial);
TinyGsmClient client(modem);
HttpClient http(modem, server, port);


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  //  Serial.println("INFO: Trying to connect to the network...");
  //  connectionEstablished = establishConnection();
  //  if (connectionEstablished) {
  //    Serial.println("INFO: Initializing complete!");
  //  } else {
  //    Serial.println("ERROR: Connection establishment failed.");
  //    while (true) {
  //      while (Serial.available())
  //      {
  //        mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  //      }
  //      while (mySerial.available())
  //      {
  //        Serial.write(mySerial.read())s;//Forward what Software Serial received to Serial Port
  //      }
  //    }
  //  }
  Serial.println("Initializing modem...");
  modem.restart();
  Serial.println("Restart Over...........");
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
  modem.init();
  if (modem.isGprsConnected()) {
    Serial.println("GPRS connected");
  }
}

void loop() {
  if (connectionEstablished) {
    static unsigned long lastSendTime = 0;
    unsigned long currentTime = millis();

    // Send data every 1000 milliseconds
    if (currentTime - lastSendTime >= 1000) {
      sendData();
      lastSendTime = currentTime;
    }
  }
}

bool establishConnection() {
  if (!sendCommand("AT")) return false;
  if (!sendCommand("AT+CSQ")) return false;
  if (!sendCommand("AT+CREG?")) return false;
  if (!sendCommand("AT+COPS=1,2, \"22605\"")) return false;
  delay(5000);
  if (!sendCommand("AT+CREG?")) return false;
  if (!sendCommand("AT+CGREG?")) return false;
  if (!sendCommand("AT+SAPBR=2,1")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"USER\",\"\"")) return false;
  if (!sendCommand("AT+SAPBR=3,1,\"PWD\",\"\"")) return false;
  if (!sendCommand("AT+SAPBR=4,1")) return false;
  if (!sendCommand("AT+SAPBR=2,1")) return false;
  //  if (!sendCommand("AT+SAPBR=0,1")) return false;
  if (!sendCommand("AT+SAPBR=1,1")) return false;
  return true;
}

void sendData() {
  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPSSL=0");
  sendCommand("AT+HTTPPARA=\"CID\",1");

  // Send HTTP GET request
  //  String apiURL = "https://disertatie-api.azurewebsites.net/t";
  String apiURL = "http://minimalsites.com/";
  sendCommand("AT+HTTPPARA=\"URL\",\"" + apiURL + "\"");
  delay(100); // Add a small delay to ensure the command is processed properly

  sendCommand("AT+HTTPACTION=0");
  delay(1000); // Wait for response

  sendCommand("AT+HTTPREAD");
  delay(1000); // Wait for response

  sendCommand("AT+HTTPTERM");
}

bool sendCommand(String command) {
  mySerial.println(command);
  delay(500); // Adjust delay if needed
  while (mySerial.available()) {
    String response = mySerial.readStringUntil('\n');
    Serial.println(response);
    if (response.indexOf("ERROR") != -1) {
      return false;
    }
  }
  return true;
}
