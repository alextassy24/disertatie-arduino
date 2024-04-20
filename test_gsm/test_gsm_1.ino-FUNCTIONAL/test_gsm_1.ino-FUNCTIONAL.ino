#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
  Serial.println("");
  establishConnection();
  delay(1000);
  Serial.println("Initializing complete!");

  //  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  //  updateSerial();
  //  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  //  updateSerial();
  //  mySerial.println("ATi"); //Returns the module name and revision
  //  updateSerial();
  //  mySerial.println("AT+COPS=?"); //Returns the list of operators present in the network
  //  updateSerial();
  //  mySerial.println("AT+CBC"); //Returns Li-Po battery status. The second number is the battery level (in our case it is 93%)
  //  //and the third number is the actual voltage in mV
  //  updateSerial();
  // Configure GPRS connection
  //  mySerial.println("AT+SAPBR=3,1,\"APN\",\"internet.vodafone.ro\"");
  //  updateSerial();
}

void loop()
{
  delay(1000);
  sendData();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void establishConnection() {
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("AT+COPS?"); //Checks which network you are connected to
  updateSerial();
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  updateSerial();
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  updateSerial();
  mySerial.println("AT+SAPBR=1,1");
  updateSerial();
  mySerial.println("AT+SAPBR=2,1");
  updateSerial();
}
void sendData() {
  mySerial.println("AT+HTTPINIT");
  updateSerial();
  mySerial.println("AT+HTTPSSL=0");
  updateSerial();
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  updateSerial();

  // Send HTTP GET request
  String apiURL = "https://disertatie-api.azurewebsites.net/t";
  // Send AT command to set URL
  mySerial.print("AT+HTTPPARA=\"URL\",\"");
  mySerial.print(apiURL);
  mySerial.println("\"");
  delay(100); // Add a small delay to ensure the command is processed properly
  updateSerial();

  // Wait for response before sending HTTPACTION command
  delay(500); // Adjust delay as needed
  mySerial.println("AT+HTTPACTION=0");
  delay(1000); // Wait for response
  updateSerial();

  // Read HTTP response
  mySerial.println("AT+HTTPREAD");
  delay(1000); // Wait for response
  updateSerial();

  // Terminate HTTP session
  mySerial.println("AT+HTTPTERM");
  delay(100); // Add a small delay before sending the next command

  Serial.println("Data sent successfully");
}
