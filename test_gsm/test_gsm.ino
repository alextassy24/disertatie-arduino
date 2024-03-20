#include <SoftwareSerial.h>
//#include <Sim800L.h>

//Sim800L sim800L(2, 3); // tx - 3, rx - 2
SoftwareSerial sim800L(3, 2); // rx - 3, tx - 2

void setup() {
  Serial.begin(9600);
  sim800L.begin(9600);

  Serial.println("Initializing SIM800L...");
//  delay(1000);
//   Configure SIM800L for GPRS connection
//   Check if SIM800L is responding
//    sim800L.println("AT");
//    delay(100);
//    while (sim800L.available()) {
//      Serial.write(sim800L.read());
//    }
//  
//    // Check if SIM800L responded with "OK"
//    if (sim800L.find("OK")) {
//      // Enable GPRS connection with appropriate APN settings
//      sim800L.println("AT+SAPBR=3,1,\"APN\",\"live.vodafone.com\""); // Set APN
//      delay(100);
//      while (sim800L.available()) {
//        Serial.write(sim800L.read());
//      }
//  
//      sim800L.println("AT+SAPBR=1,1"); // Enable GPRS connection
//      delay(100);
//      while (sim800L.available()) {
//        Serial.write(sim800L.read());
//      }
//  
//      // Check if GPRS connection is succemygpsfully enabled
//      if (sim800L.find("OK")) {
//        Serial.println("GPRS enabled!");
//        Serial.println("Setup Complete!");
//      }
//      else {
//        Serial.println("Failed to enable GPRS connection!");
//      }
//    }
//    else {
//      Serial.println("SIM800L initialization failed!");
//    }

//  sim800L.sendSms("+0728552655", "Test");
//  Serial.println(sim800L.getProductInfo());
//  Serial.println(sim800L.getOperatorsList());
//  Serial.println(sim800L.calculateLocation());
//  Serial.println(sim800L.getOperator());
//  Serial.println(sim800L.getLongitude());
//  Serial.println(sim800L.getLatitude());
//  Serial.println(sim800L.dateNet());
}

void loop()
{
//  while (sim800L.available()) {
//    Serial.println(sim800L.readString());
//  }
//  while (Serial.available())  {
//    sim800L.println(Serial.readString());
//  }

  while (sim800L.available()) {
    Serial.write(sim800L.read());
  }
  
  while (Serial.available())  {
    sim800L.write(Serial.read());
  }
}
