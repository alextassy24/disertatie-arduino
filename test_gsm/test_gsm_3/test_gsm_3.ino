#include <SoftwareSerial.h>

// RX, TX
SoftwareSerial mySerial(3, 2); // Adjust pins according to your connections

const char* apn = "internet.vodafone.ro"; // Change to your APN
const char* url = "https://disertatie-api.azurewebsites.net/t"; // Change to your desired HTTP endpoint

void setup() {
  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  // Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing SIM800L");
  delay(1000); // Give time for module to settle


  sendATCommand("AT+CMEE=2", "OK", 1000);
  // Set module to full functionality
  sendATCommand("AT+CFUN=1", "OK", 2000);

  // Disable echo
  sendATCommand("ATE0", "OK", 1000);

  // Set APN
  setupGPRS();

  // Perform HTTP GET request
  httpGet(url);
}

void loop() {
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void sendATCommand(String command, const char* response, unsigned long timeout) {
  Serial.print("Sending: ");
  Serial.println(command);
  mySerial.println(command);

  long int time = millis();
  String data;

  while ((time + timeout) > millis()) {
    while (mySerial.available()) {
      char c = mySerial.read();  // Read response
      data += c;
    }

    if (data.indexOf(response) != -1) {
      Serial.println(data);
      return;
    }
  }

  Serial.print("Timed out: ");
  Serial.println(data);
}

void setupGPRS() {
  sendATCommand("AT+SAPBR=0,1", "OK", 2000);
  sendATCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
  sendATCommand("AT+SAPBR=3,1,\"APN\",\"" + String(apn) + "\"", "OK", 2000);
  sendATCommand("AT+SAPBR=2,1", "OK", 2000); // Query the GPRS context
  sendATCommand("AT+SAPBR=1,1", "OK", 2000); // Open the GPRS context
}

void httpGet(const char* url) {
  sendATCommand("AT+HTTPINIT", "OK", 2000);
  sendATCommand("AT+HTTPPARA=\"CID\",1", "OK", 2000);
  sendATCommand("AT+HTTPPARA=\"URL\",\"" + String(url) + "\"", "OK", 2000);
  sendATCommand("AT+HTTPACTION=0", "+HTTPACTION:", 15000); // Wait for the response code and data length

  // Parse response to find status and data length
  // Example expected response: +HTTPACTION: 0,200,1234 (method, status, data_length)
  String httpResponse = mySerial.readString();
  Serial.println(httpResponse); // Print full HTTP action response
  if (httpResponse.indexOf("200") > -1) { // Check if HTTP status code is OK (200)
    sendATCommand("AT+HTTPREAD", "+HTTPREAD:", 10000); // Read the HTTP response data
  } else {
    Serial.println("HTTP request failed or no data to read");
  }

  sendATCommand("AT+HTTPTERM", "OK", 2000); // Terminate HTTP service
}
