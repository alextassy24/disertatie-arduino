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
  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
//  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
//  updateSerial();
//  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
//  updateSerial();
//  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
//  updateSerial();
//  mySerial.println("ATi"); //Returns the module name and revision
//  updateSerial();
//  mySerial.println("AT+COPS?"); //Checks which network you are connected to
//  updateSerial();
//  mySerial.println("AT+COPS=?"); //Returns the list of operators present in the network
//  updateSerial();
//  mySerial.println("AT+CBC"); //Returns Li-Po battery status. The second number is the battery level (in our case it is 93%)
//  //and the third number is the actual voltage in mV
//  updateSerial();
  // Configure GPRS connection
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"internet.vodafone.ro\"");
  updateSerial();
  mySerial.println("AT+SAPBR=1,1");
  updateSerial();
  mySerial.println("AT+SAPBR=2,1");
  updateSerial();
  mySerial.println("AT+HTTPINIT");
  updateSerial();
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  updateSerial();
  // Send HTTP GET request
  String apiURL = "https://disertatie-api.azurewebsites.net/check-connection";
  Serial.println("API URL: ");
  Serial.println(apiURL);
  // Send AT command to set URL
  mySerial.print("AT+HTTPPARA=\"URL\",\"");
  mySerial.print(apiURL);
  mySerial.println("\"");
  delay(100); // Add a small delay to ensure the command is processed properly

  // Send AT command to perform HTTP GET request
  mySerial.println("AT+HTTPACTION=0");
  delay(1000); // Wait for response

  // Read HTTP response
  mySerial.println("AT+HTTPREAD");
  delay(1000); // Wait for response

  // Terminate HTTP session
  mySerial.println("AT+HTTPTERM");
  delay(100); // Add a small delay before sending the next command

}

void loop()
{
  updateSerial();
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

void waitForResponse() {
  delay(1000); // Wait for response
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Forward response to Serial Monitor
  }
}
